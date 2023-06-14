/*
 * Copyright (C) 2012, 2017 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CyberKitWebPage.h"

#include "APIArray.h"
#include "APIDictionary.h"
#include "APIError.h"
#include "APINumber.h"
#include "APIString.h"
#include "APIURLRequest.h"
#include "APIURLResponse.h"
#include "ImageOptions.h"
#include "InjectedBundle.h"
#include "WebContextMenuItem.h"
#include "WebImage.h"
#include "CyberKitConsoleMessagePrivate.h"
#include "CyberKitContextMenuPrivate.h"
#include "CyberKitDOMDocumentPrivate.h"
#include "CyberKitDOMElementPrivate.h"
#include "CyberKitDOMNodePrivate.h"
#include "CyberKitFramePrivate.h"
#include "CyberKitPrivate.h"
#include "CyberKitScriptWorldPrivate.h"
#include "CyberKitURIRequestPrivate.h"
#include "CyberKitURIResponsePrivate.h"
#include "CyberKitWebEditorPrivate.h"
#include "CyberKitWebHitTestResultPrivate.h"
#include "CyberKitWebPagePrivate.h"
#include "CyberKitWebProcessEnumTypes.h"
#include "WebProcess.h"
#include <CyberCore/Document.h>
#include <CyberCore/DocumentLoader.h>
#include <CyberCore/Frame.h>
#include <CyberCore/FrameDestructionObserver.h>
#include <CyberCore/FrameLoader.h>
#include <CyberCore/FrameView.h>
#include <CyberCore/HTMLFormElement.h>
#include <glib/gi18n-lib.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>
#include <wtf/text/StringBuilder.h>

using namespace CyberKit;
using namespace CyberCore;

enum {
    DOCUMENT_LOADED,
    SEND_REQUEST,
    CONTEXT_MENU,
    CONSOLE_MESSAGE_SENT,
    FORM_CONTROLS_ASSOCIATED,
    WILL_SUBMIT_FORM,

    LAST_SIGNAL
};

enum {
    PROP_0,

    PROP_URI
};

struct _CyberKitWebPagePrivate {
    WebPage* webPage;

    CString uri;

    GRefPtr<CyberKitWebEditor> webEditor;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_TYPE(CyberKitWebPage, webkit_web_page, G_TYPE_OBJECT)

static void webFrameDestroyed(WebFrame*);

class CyberKitFrameWrapper final: public FrameDestructionObserver {
public:
    CyberKitFrameWrapper(WebFrame& webFrame)
        : FrameDestructionObserver(webFrame.coreFrame())
        , m_webkitFrame(adoptGRef(webkitFrameCreate(&webFrame)))
    {
    }

    CyberKitFrame* webkitFrame() const { return m_webkitFrame.get(); }

private:
    void frameDestroyed() override
    {
        FrameDestructionObserver::frameDestroyed();
        webFrameDestroyed(webkitFrameGetWebFrame(m_webkitFrame.get()));
    }

    GRefPtr<CyberKitFrame> m_webkitFrame;
};

typedef HashMap<WebFrame*, std::unique_ptr<CyberKitFrameWrapper>> WebFrameMap;

static WebFrameMap& webFrameMap()
{
    static NeverDestroyed<WebFrameMap> map;
    return map;
}

static CyberKitFrame* webkitFrameGetOrCreate(WebFrame* webFrame)
{
    auto wrapperPtr = webFrameMap().get(webFrame);
    if (wrapperPtr)
        return wrapperPtr->webkitFrame();

    std::unique_ptr<CyberKitFrameWrapper> wrapper = std::make_unique<CyberKitFrameWrapper>(*webFrame);
    wrapperPtr = wrapper.get();
    webFrameMap().set(webFrame, WTFMove(wrapper));
    return wrapperPtr->webkitFrame();
}

static void webFrameDestroyed(WebFrame* webFrame)
{
    webFrameMap().remove(webFrame);
}

static void webkitWebPageSetURI(CyberKitWebPage* webPage, const CString& uri)
{
    if (webPage->priv->uri == uri)
        return;

    webPage->priv->uri = uri;
    g_object_notify(G_OBJECT(webPage), "uri");
}

static void webkitWebPageDidSendConsoleMessage(CyberKitWebPage* webPage, MessageSource source, MessageLevel level, const String& message, unsigned lineNumber, const String& sourceID)
{
    CyberKitConsoleMessage consoleMessage(source, level, message, lineNumber, sourceID);
    g_signal_emit(webPage, signals[CONSOLE_MESSAGE_SENT], 0, &consoleMessage);
}

class PageLoaderClient final : public API::InjectedBundle::PageLoaderClient {
public:
    explicit PageLoaderClient(CyberKitWebPage* webPage)
        : m_webPage(webPage)
    {
    }

private:
    static CString getDocumentLoaderURL(DocumentLoader* documentLoader)
    {
        ASSERT(documentLoader);
        if (!documentLoader->unreachableURL().isEmpty())
            return documentLoader->unreachableURL().string().utf8();

        return documentLoader->url().string().utf8();
    }

    void didStartProvisionalLoadForFrame(WebPage&, WebFrame& frame, RefPtr<API::Object>&) override
    {
        if (!frame.isMainFrame())
            return;
        webkitWebPageSetURI(m_webPage, getDocumentLoaderURL(frame.coreFrame()->loader().provisionalDocumentLoader()));
    }

    void didReceiveServerRedirectForProvisionalLoadForFrame(WebPage&, WebFrame& frame, RefPtr<API::Object>&) override
    {
        if (!frame.isMainFrame())
            return;
        webkitWebPageSetURI(m_webPage, getDocumentLoaderURL(frame.coreFrame()->loader().provisionalDocumentLoader()));
    }

    void didSameDocumentNavigationForFrame(WebPage&, WebFrame& frame, SameDocumentNavigationType, RefPtr<API::Object>&) override
    {
        if (!frame.isMainFrame())
            return;
        webkitWebPageSetURI(m_webPage, frame.coreFrame()->document()->url().string().utf8());
    }

    void didCommitLoadForFrame(WebPage&, WebFrame& frame, RefPtr<API::Object>&) override
    {
        if (!frame.isMainFrame())
            return;
        webkitWebPageSetURI(m_webPage, getDocumentLoaderURL(frame.coreFrame()->loader().documentLoader()));
    }

    void didFinishDocumentLoadForFrame(WebPage&, WebFrame& frame, RefPtr<API::Object>&) override
    {
        if (!frame.isMainFrame())
            return;
        g_signal_emit(m_webPage, signals[DOCUMENT_LOADED], 0);
    }

    void didClearWindowObjectForFrame(WebPage&, WebFrame& frame, DOMWrapperWorld& world) override
    {
        auto injectedWorld = InjectedBundleScriptWorld::getOrCreate(world);
        if (auto* wkWorld = webkitScriptWorldGet(injectedWorld.ptr()))
            webkitScriptWorldWindowObjectCleared(wkWorld, m_webPage, webkitFrameGetOrCreate(&frame));
    }

    CyberKitWebPage* m_webPage;
};


class PageResourceLoadClient final : public API::InjectedBundle::ResourceLoadClient {
public:
    explicit PageResourceLoadClient(CyberKitWebPage* webPage)
        : m_webPage(webPage)
    {
    }

private:
    void didInitiateLoadForResource(WebPage& page, WebFrame& frame, uint64_t identifier, const ResourceRequest& request, bool) override
    {
        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Frame"), &frame);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        message.set(String::fromUTF8("Request"), API::URLRequest::create(request));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidInitiateLoadForResource"), API::Dictionary::create(WTFMove(message)).ptr());
    }

    void willSendRequestForFrame(WebPage& page, WebFrame&, uint64_t identifier, ResourceRequest& resourceRequest, const ResourceResponse& redirectResourceResponse) override
    {
        GRefPtr<CyberKitURIRequest> request = adoptGRef(webkitURIRequestCreateForResourceRequest(resourceRequest));
        GRefPtr<CyberKitURIResponse> redirectResponse = !redirectResourceResponse.isNull() ? adoptGRef(webkitURIResponseCreateForResourceResponse(redirectResourceResponse)) : nullptr;

        gboolean returnValue;
        g_signal_emit(m_webPage, signals[SEND_REQUEST], 0, request.get(), redirectResponse.get(), &returnValue);
        if (returnValue) {
            resourceRequest = { };
            return;
        }

        webkitURIRequestGetResourceRequest(request.get(), resourceRequest);
        resourceRequest.setInitiatingPageID(page.pageID());

        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        message.set(String::fromUTF8("Request"), API::URLRequest::create(resourceRequest));
        if (!redirectResourceResponse.isNull())
            message.set(String::fromUTF8("RedirectResponse"), API::URLResponse::create(redirectResourceResponse));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidSendRequestForResource"), API::Dictionary::create(WTFMove(message)).ptr());
    }

    void didReceiveResponseForResource(WebPage& page, WebFrame&, uint64_t identifier, const ResourceResponse& response) override
    {
        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        message.set(String::fromUTF8("Response"), API::URLResponse::create(response));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidReceiveResponseForResource"), API::Dictionary::create(WTFMove(message)).ptr());

        // Post on the console as well to be consistent with the inspector.
        if (response.httpStatusCode() >= 400) {
            StringBuilder errorMessage;
            errorMessage.appendLiteral("Failed to load resource: the server responded with a status of ");
            errorMessage.appendNumber(response.httpStatusCode());
            errorMessage.appendLiteral(" (");
            errorMessage.append(response.httpStatusText());
            errorMessage.append(')');
            webkitWebPageDidSendConsoleMessage(m_webPage, MessageSource::Network, MessageLevel::Error, errorMessage.toString(), 0, response.url().string());
        }
    }

    void didReceiveContentLengthForResource(WebPage& page, WebFrame&, uint64_t identifier, uint64_t contentLength) override
    {
        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        message.set(String::fromUTF8("ContentLength"), API::UInt64::create(contentLength));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidReceiveContentLengthForResource"), API::Dictionary::create(WTFMove(message)).ptr());
    }

    void didFinishLoadForResource(WebPage& page, WebFrame&, uint64_t identifier) override
    {
        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidFinishLoadForResource"), API::Dictionary::create(WTFMove(message)).ptr());
    }

    void didFailLoadForResource(WebPage& page, WebFrame&, uint64_t identifier, const ResourceError& error) override
    {
        API::Dictionary::MapType message;
        message.set(String::fromUTF8("Page"), &page);
        message.set(String::fromUTF8("Identifier"), API::UInt64::create(identifier));
        message.set(String::fromUTF8("Error"), API::Error::create(error));
        WebProcess::singleton().injectedBundle()->postMessage(String::fromUTF8("WebPage.DidFailLoadForResource"), API::Dictionary::create(WTFMove(message)).ptr());

        // Post on the console as well to be consistent with the inspector.
        if (!error.isCancellation()) {
            StringBuilder errorMessage;
            errorMessage.appendLiteral("Failed to load resource");
            if (!error.localizedDescription().isEmpty()) {
                errorMessage.appendLiteral(": ");
                errorMessage.append(error.localizedDescription());
            }
            webkitWebPageDidSendConsoleMessage(m_webPage, MessageSource::Network, MessageLevel::Error, errorMessage.toString(), 0, error.failingURL());
        }
    }

    CyberKitWebPage* m_webPage;
};

class PageContextMenuClient final : public API::InjectedBundle::PageContextMenuClient {
public:
    explicit PageContextMenuClient(CyberKitWebPage* webPage)
        : m_webPage(webPage)
    {
    }

private:
    bool getCustomMenuFromDefaultItems(WebPage&, const CyberCore::HitTestResult& hitTestResult, const Vector<CyberCore::ContextMenuItem>& defaultMenu, Vector<WebContextMenuItemData>& newMenu, RefPtr<API::Object>& userData) override
    {
        GRefPtr<CyberKitContextMenu> contextMenu = adoptGRef(webkitContextMenuCreate(kitItems(defaultMenu)));
        GRefPtr<CyberKitWebHitTestResult> webHitTestResult = adoptGRef(webkitWebHitTestResultCreate(hitTestResult));
        gboolean returnValue;
        g_signal_emit(m_webPage, signals[CONTEXT_MENU], 0, contextMenu.get(), webHitTestResult.get(), &returnValue);
        if (GVariant* variant = webkit_context_menu_get_user_data(contextMenu.get())) {
            GUniquePtr<gchar> dataString(g_variant_print(variant, TRUE));
            userData = API::String::create(String::fromUTF8(dataString.get()));
        }

        if (!returnValue)
            return false;

        webkitContextMenuPopulate(contextMenu.get(), newMenu);
        return true;
    }

    CyberKitWebPage* m_webPage;
};

class PageUIClient final : public API::InjectedBundle::PageUIClient {
public:
    explicit PageUIClient(CyberKitWebPage* webPage)
        : m_webPage(webPage)
    {
    }

private:
    void willAddMessageToConsole(WebPage*, MessageSource source, MessageLevel level, const String& message, unsigned lineNumber, unsigned /*columnNumber*/, const String& sourceID) override
    {
        webkitWebPageDidSendConsoleMessage(m_webPage, source, level, message, lineNumber, sourceID);
    }

    CyberKitWebPage* m_webPage;
};

class PageFormClient final : public API::InjectedBundle::FormClient {
public:
    explicit PageFormClient(CyberKitWebPage* webPage)
        : m_webPage(webPage)
    {
    }

    void willSubmitForm(WebPage*, HTMLFormElement* formElement, WebFrame* frame, WebFrame* sourceFrame, const Vector<std::pair<String, String>>& values, RefPtr<API::Object>&) override
    {
        fireFormSubmissionEvent(WEBKIT_FORM_SUBMISSION_WILL_COMPLETE, formElement, frame, sourceFrame, values);
    }

    void willSendSubmitEvent(WebPage*, HTMLFormElement* formElement, WebFrame* frame, WebFrame* sourceFrame, const Vector<std::pair<String, String>>& values) override
    {
        fireFormSubmissionEvent(WEBKIT_FORM_SUBMISSION_WILL_SEND_DOM_EVENT, formElement, frame, sourceFrame, values);
    }

    void didAssociateFormControls(WebPage*, const Vector<RefPtr<Element>>& elements) override
    {
        GRefPtr<GPtrArray> formElements = adoptGRef(g_ptr_array_sized_new(elements.size()));
        for (size_t i = 0; i < elements.size(); ++i)
            g_ptr_array_add(formElements.get(), CyberKit::kit(elements[i].get()));

        g_signal_emit(m_webPage, signals[FORM_CONTROLS_ASSOCIATED], 0, formElements.get());
    }

    bool shouldNotifyOnFormChanges(WebPage*) override { return true; }

private:
    void fireFormSubmissionEvent(CyberKitFormSubmissionStep step, HTMLFormElement* formElement, WebFrame* frame, WebFrame* sourceFrame, const Vector<std::pair<String, String>>& values)
    {
        CyberKitFrame* webkitTargetFrame = webkitFrameGetOrCreate(frame);
        CyberKitFrame* webkitSourceFrame = webkitFrameGetOrCreate(sourceFrame);

        GRefPtr<GPtrArray> textFieldNames = adoptGRef(g_ptr_array_new_full(values.size(), g_free));
        GRefPtr<GPtrArray> textFieldValues = adoptGRef(g_ptr_array_new_full(values.size(), g_free));
        for (auto& pair : values) {
            g_ptr_array_add(textFieldNames.get(), g_strdup(pair.first.utf8().data()));
            g_ptr_array_add(textFieldValues.get(), g_strdup(pair.second.utf8().data()));
        }

        g_signal_emit(m_webPage, signals[WILL_SUBMIT_FORM], 0, WEBKIT_DOM_ELEMENT(CyberKit::kit(static_cast<Node*>(formElement))), step, webkitSourceFrame, webkitTargetFrame, textFieldNames.get(), textFieldValues.get());
    }

    CyberKitWebPage* m_webPage;
};

static void webkitWebPageGetProperty(GObject* object, guint propId, GValue* value, GParamSpec* paramSpec)
{
    CyberKitWebPage* webPage = WEBKIT_WEB_PAGE(object);

    switch (propId) {
    case PROP_URI:
        g_value_set_string(value, webkit_web_page_get_uri(webPage));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkit_web_page_class_init(CyberKitWebPageClass* klass)
{
    GObjectClass* gObjectClass = G_OBJECT_CLASS(klass);

    gObjectClass->get_property = webkitWebPageGetProperty;

    /**
     * CyberKitWebPage:uri:
     *
     * The current active URI of the #CyberKitWebPage.
     */
    g_object_class_install_property(
        gObjectClass,
        PROP_URI,
        g_param_spec_string(
            "uri",
            _("URI"),
            _("The current active URI of the web page"),
            0,
            WEBKIT_PARAM_READABLE));

    /**
     * CyberKitWebPage::document-loaded:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     *
     * This signal is emitted when the DOM document of a #CyberKitWebPage has been
     * loaded.
     *
     * You can wait for this signal to get the DOM document with
     * webkit_web_page_get_dom_document().
     */
    signals[DOCUMENT_LOADED] = g_signal_new(
        "document-loaded",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0, 0, 0,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE, 0);

    /**
     * CyberKitWebPage::send-request:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     * @request: a #CyberKitURIRequest
     * @redirected_response: a #CyberKitURIResponse, or %NULL
     *
     * This signal is emitted when @request is about to be sent to
     * the server. This signal can be used to modify the #CyberKitURIRequest
     * that will be sent to the server. You can also cancel the resource load
     * operation by connecting to this signal and returning %TRUE.
     *
     * In case of a server redirection this signal is
     * emitted again with the @request argument containing the new
     * request to be sent to the server due to the redirection and the
     * @redirected_response parameter containing the response
     * received by the server for the initial request.
     *
     * Modifications to the #CyberKitURIRequest and its associated
     * #SoupMessageHeaders will be taken into account when the request
     * is sent over the network.
     *
     * Returns: %TRUE to stop other handlers from being invoked for the event.
     *    %FALSE to continue emission of the event.
     */
    signals[SEND_REQUEST] = g_signal_new(
        "send-request",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0,
        g_signal_accumulator_true_handled, nullptr,
        g_cclosure_marshal_generic,
        G_TYPE_BOOLEAN, 2,
        WEBKIT_TYPE_URI_REQUEST,
        WEBKIT_TYPE_URI_RESPONSE);

    /**
     * CyberKitWebPage::context-menu:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     * @context_menu: the proposed #CyberKitContextMenu
     * @hit_test_result: a #CyberKitWebHitTestResult
     *
     * Emitted before a context menu is displayed in the UI Process to
     * give the application a chance to customize the proposed menu,
     * build its own context menu or pass user data to the UI Process.
     * This signal is useful when the information available in the UI Process
     * is not enough to build or customize the context menu, for example, to
     * add menu entries depending on the #CyberKitDOMNode at the coordinates of the
     * @hit_test_result. Otherwise, it's recommended to use #CyberKitWebView::context-menu
     * signal instead.
     *
     * Returns: %TRUE if the proposed @context_menu has been modified, or %FALSE otherwise.
     *
     * Since: 2.8
     */
    signals[CONTEXT_MENU] = g_signal_new(
        "context-menu",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0,
        g_signal_accumulator_true_handled, nullptr,
        g_cclosure_marshal_generic,
        G_TYPE_BOOLEAN, 2,
        WEBKIT_TYPE_CONTEXT_MENU,
        WEBKIT_TYPE_WEB_HIT_TEST_RESULT);

    /**
     * CyberKitWebPage::console-message-sent:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     * @console_message: the #CyberKitConsoleMessage
     *
     * Emitted when a message is sent to the console. This can be a message
     * produced by the use of JavaScript console API, a JavaScript exception,
     * a security error or other errors, warnings, debug or log messages.
     * The @console_message contains information of the message.
     *
     * Since: 2.12
     */
    signals[CONSOLE_MESSAGE_SENT] = g_signal_new(
        "console-message-sent",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0, 0, nullptr,
        g_cclosure_marshal_VOID__BOXED,
        G_TYPE_NONE, 1,
        WEBKIT_TYPE_CONSOLE_MESSAGE | G_SIGNAL_TYPE_STATIC_SCOPE);

    /**
     * CyberKitWebPage::form-controls-associated:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     * @elements: (element-type CyberKitDOMElement) (transfer none): a #GPtrArray of
     *     #CyberKitDOMElement with the list of forms in the page
     *
     * Emitted after form elements (or form associated elements) are associated to a particular web
     * page. This is useful to implement form auto filling for web pages where form fields are added
     * dynamically. This signal might be emitted multiple times for the same web page.
     *
     * Note that this signal could be also emitted when form controls are moved between forms. In
     * that case, the @elements array carries the list of those elements which have moved.
     *
     * Clients should take a reference to the members of the @elements array if it is desired to
     * keep them alive after the signal handler returns.
     *
     * Since: 2.16
     */
    signals[FORM_CONTROLS_ASSOCIATED] = g_signal_new(
        "form-controls-associated",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0, 0, nullptr,
        g_cclosure_marshal_VOID__BOXED,
        G_TYPE_NONE, 1,
        G_TYPE_PTR_ARRAY);

    /**
     * CyberKitWebPage::will-submit-form:
     * @web_page: the #CyberKitWebPage on which the signal is emitted
     * @form: the #CyberKitDOMElement to be submitted, which will always correspond to an HTMLFormElement
     * @step: a #CyberKitFormSubmissionEventType indicating the current
     * stage of form submission
     * @source_frame: the #CyberKitFrame containing the form to be
     * submitted
     * @target_frame: the #CyberKitFrame containing the form's target,
     * which may be the same as @source_frame if no target was specified
     * @text_field_names: (element-type utf8) (transfer none): names of
     * the form's text fields
     * @text_field_values: (element-type utf8) (transfer none): values
     * of the form's text fields
     *
     * This signal is emitted to indicate various points during form
     * submission. @step indicates the current stage of form submission.
     *
     * If this signal is emitted with %WEBKIT_FORM_SUBMISSION_WILL_SEND_DOM_EVENT,
     * then the DOM submit event is about to be emitted. JavaScript code
     * may rely on the submit event to detect that the user has clicked
     * on a submit button, and to possibly cancel the form submission
     * before %WEBKIT_FORM_SUBMISSION_WILL_COMPLETE. However, beware
     * that, for historical reasons, the submit event is not emitted at
     * all if the form submission is triggered by JavaScript. For these
     * reasons, %WEBKIT_FORM_SUBMISSION_WILL_SEND_DOM_EVENT may not
     * be used to reliably detect whether a form will be submitted.
     * Instead, use it to detect if a user has clicked on a form's
     * submit button even if JavaScript later cancels the form
     * submission, or to read the values of the form's fields even if
     * JavaScript later clears certain fields before submitting. This
     * may be needed, for example, to implement a robust browser
     * password manager, as some misguided websites may use such
     * techniques to attempt to thwart password managers.
     *
     * If this signal is emitted with %WEBKIT_FORM_SUBMISSION_WILL_COMPLETE,
     * the form will imminently be submitted. It can no longer be
     * cancelled. This event always occurs immediately before a form is
     * submitted to its target, so use this event to reliably detect
     * when a form is submitted. This event occurs after
     * %WEBKIT_FORM_SUBMISSION_WILL_SEND_DOM_EVENT if that event is
     * emitted.
     *
     * Since: 2.20
     */
    signals[WILL_SUBMIT_FORM] = g_signal_new(
        "will-submit-form",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_LAST,
        0, 0, nullptr,
        g_cclosure_marshal_generic,
        G_TYPE_NONE, 6,
        WEBKIT_DOM_TYPE_ELEMENT,
        WEBKIT_TYPE_FORM_SUBMISSION_STEP,
        WEBKIT_TYPE_FRAME,
        WEBKIT_TYPE_FRAME,
        G_TYPE_PTR_ARRAY,
        G_TYPE_PTR_ARRAY);
}

WebPage* webkitWebPageGetPage(CyberKitWebPage *webPage)
{
    return webPage->priv->webPage;
}

CyberKitWebPage* webkitWebPageCreate(WebPage* webPage)
{
    CyberKitWebPage* page = WEBKIT_WEB_PAGE(g_object_new(WEBKIT_TYPE_WEB_PAGE, NULL));
    page->priv->webPage = webPage;

    webPage->setInjectedBundleResourceLoadClient(std::make_unique<PageResourceLoadClient>(page));
    webPage->setInjectedBundlePageLoaderClient(std::make_unique<PageLoaderClient>(page));
    webPage->setInjectedBundleContextMenuClient(std::make_unique<PageContextMenuClient>(page));
    webPage->setInjectedBundleUIClient(std::make_unique<PageUIClient>(page));
    webPage->setInjectedBundleFormClient(std::make_unique<PageFormClient>(page));

    return page;
}

void webkitWebPageDidReceiveMessage(CyberKitWebPage* page, const String& messageName, API::Dictionary& message)
{
#if PLATFORM(GTK)
    if (messageName == String("GetSnapshot")) {
        SnapshotOptions snapshotOptions = static_cast<SnapshotOptions>(static_cast<API::UInt64*>(message.get("SnapshotOptions"))->value());
        uint64_t callbackID = static_cast<API::UInt64*>(message.get("CallbackID"))->value();
        SnapshotRegion region = static_cast<SnapshotRegion>(static_cast<API::UInt64*>(message.get("SnapshotRegion"))->value());
        bool transparentBackground = static_cast<API::Boolean*>(message.get("TransparentBackground"))->value();

        RefPtr<WebImage> snapshotImage;
        WebPage* webPage = page->priv->webPage;
        if (CyberCore::FrameView* frameView = webPage->mainFrameView()) {
            CyberCore::IntRect snapshotRect;
            switch (region) {
            case SnapshotRegionVisible:
                snapshotRect = frameView->visibleContentRect();
                break;
            case SnapshotRegionFullDocument:
                snapshotRect = CyberCore::IntRect(CyberCore::IntPoint(0, 0), frameView->contentsSize());
                break;
            default:
                ASSERT_NOT_REACHED();
            }
            if (!snapshotRect.isEmpty()) {
                Color savedBackgroundColor;
                if (transparentBackground) {
                    savedBackgroundColor = frameView->baseBackgroundColor();
                    frameView->setBaseBackgroundColor(Color::transparent);
                }
                snapshotImage = webPage->scaledSnapshotWithOptions(snapshotRect, 1, snapshotOptions | SnapshotOptionsShareable);
                if (transparentBackground)
                    frameView->setBaseBackgroundColor(savedBackgroundColor);
            }
        }

        API::Dictionary::MapType messageReply;
        messageReply.set("Page", webPage);
        messageReply.set("CallbackID", API::UInt64::create(callbackID));
        messageReply.set("Snapshot", snapshotImage);
        WebProcess::singleton().injectedBundle()->postMessage("WebPage.DidGetSnapshot", API::Dictionary::create(WTFMove(messageReply)).ptr());
    } else
#endif
        ASSERT_NOT_REACHED();
}

/**
 * webkit_web_page_get_dom_document:
 * @web_page: a #CyberKitWebPage
 *
 * Get the #CyberKitDOMDocument currently loaded in @web_page
 *
 * Returns: (transfer none): the #CyberKitDOMDocument currently loaded, or %NULL
 *    if no document is currently loaded.
 */
CyberKitDOMDocument* webkit_web_page_get_dom_document(CyberKitWebPage* webPage)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_PAGE(webPage), nullptr);

    if (auto* coreFrame = webPage->priv->webPage->mainFrame())
        return kit(coreFrame->document());

    return nullptr;
}

/**
 * webkit_web_page_get_id:
 * @web_page: a #CyberKitWebPage
 *
 * Get the identifier of the #CyberKitWebPage
 *
 * Returns: the identifier of @web_page
 */
guint64 webkit_web_page_get_id(CyberKitWebPage* webPage)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_PAGE(webPage), 0);

    return webPage->priv->webPage->pageID();
}

/**
 * webkit_web_page_get_uri:
 * @web_page: a #CyberKitWebPage
 *
 * Returns the current active URI of @web_page.
 *
 * You can monitor the active URI by connecting to the notify::uri
 * signal of @web_page.
 *
 * Returns: the current active URI of @web_view or %NULL if nothing has been
 *    loaded yet.
 */
const gchar* webkit_web_page_get_uri(CyberKitWebPage* webPage)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_PAGE(webPage), 0);

    return webPage->priv->uri.data();
}

/**
 * webkit_web_page_get_main_frame:
 * @web_page: a #CyberKitWebPage
 *
 * Returns the main frame of a #CyberKitWebPage.
 *
 * Returns: (transfer none): the #CyberKitFrame that is the main frame of @web_page
 *
 * Since: 2.2
 */
CyberKitFrame* webkit_web_page_get_main_frame(CyberKitWebPage* webPage)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_PAGE(webPage), 0);

    return webkitFrameGetOrCreate(webPage->priv->webPage->mainWebFrame());
}

/**
 * webkit_web_page_get_editor:
 * @web_page: a #CyberKitWebPage
 *
 * Gets the #CyberKitWebEditor of a #CyberKitWebPage.
 *
 * Returns: (transfer none): the #CyberKitWebEditor
 *
 * Since: 2.10
 */
CyberKitWebEditor* webkit_web_page_get_editor(CyberKitWebPage* webPage)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_PAGE(webPage), nullptr);

    if (!webPage->priv->webEditor)
        webPage->priv->webEditor = adoptGRef(webkitWebEditorCreate(webPage));

    return webPage->priv->webEditor.get();
}
