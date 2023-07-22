/*
 * Copyright (C) 2012 Igalia S.L.
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
#include "CyberKitFindController.h"

#include "APIFindClient.h"
#include "CyberKitEnumTypes.h"
#include "CyberKitWebViewPrivate.h"
#include <glib/gi18n-lib.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/WTFGType.h>
#include <wtf/text/CString.h>

using namespace CyberKit;
using namespace CyberCore;

/**
 * SECTION: CyberKitFindController
 * @Short_description: Controls text search in a #CyberKitWebView
 * @Title: CyberKitFindController
 *
 * A #CyberKitFindController is used to search text in a #CyberKitWebView. You
 * can get a #CyberKitWebView<!-- -->'s #CyberKitFindController with
 * webkit_web_view_get_find_controller(), and later use it to search
 * for text using webkit_find_controller_search(), or get the
 * number of matches using webkit_find_controller_count_matches(). The
 * operations are asynchronous and trigger signals when ready, such as
 * #CyberKitFindController::found-text,
 * #CyberKitFindController::failed-to-find-text or
 * #CyberKitFindController::counted-matches<!-- -->.
 *
 */

enum {
    FOUND_TEXT,
    FAILED_TO_FIND_TEXT,
    COUNTED_MATCHES,

    LAST_SIGNAL
};

enum {
    PROP_0,

    PROP_TEXT,
    PROP_OPTIONS,
    PROP_MAX_MATCH_COUNT,
    PROP_WEB_VIEW
};

typedef enum {
    FindOperation,
    FindNextPrevOperation,
    CountOperation
} CyberKitFindControllerOperation;

struct _CyberKitFindControllerPrivate {
    CString searchText;
    // Interpreted as CyberKit::FindOptions.
    uint32_t findOptions;
    unsigned maxMatchCount;
    CyberKitWebView* webView;
};

static guint signals[LAST_SIGNAL] = { 0, };

WEBKIT_DEFINE_TYPE(CyberKitFindController, webkit_find_controller, G_TYPE_OBJECT)

static inline CyberKit::FindOptions toWebFindOptions(uint32_t findOptions)
{
    return static_cast<CyberKit::FindOptions>((findOptions & WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE ? FindOptionsCaseInsensitive : 0)
        | (findOptions & WEBKIT_FIND_OPTIONS_AT_WORD_STARTS ? FindOptionsAtWordStarts : 0)
        | (findOptions & WEBKIT_FIND_OPTIONS_TREAT_MEDIAL_CAPITAL_AS_WORD_START ? FindOptionsTreatMedialCapitalAsWordStart : 0)
        | (findOptions & WEBKIT_FIND_OPTIONS_BACKWARDS ? FindOptionsBackwards : 0)
        | (findOptions & WEBKIT_FIND_OPTIONS_WRAP_AROUND ? FindOptionsWrapAround : 0));
}

static inline CyberKitFindOptions toCyberKitFindOptions(uint32_t findOptions)
{
    return static_cast<CyberKitFindOptions>((findOptions & FindOptionsCaseInsensitive ? WEBKIT_FIND_OPTIONS_CASE_INSENSITIVE : 0)
        | (findOptions & FindOptionsAtWordStarts ? WEBKIT_FIND_OPTIONS_AT_WORD_STARTS : 0)
        | (findOptions & FindOptionsTreatMedialCapitalAsWordStart ? WEBKIT_FIND_OPTIONS_TREAT_MEDIAL_CAPITAL_AS_WORD_START : 0)
        | (findOptions & FindOptionsBackwards ? WEBKIT_FIND_OPTIONS_BACKWARDS : 0)
        | (findOptions & FindOptionsWrapAround ? WEBKIT_FIND_OPTIONS_WRAP_AROUND : 0));
}

static inline WebPageProxy& getPage(CyberKitFindController* findController)
{
    return webkitWebViewGetPage(findController->priv->webView);
}

class FindClient final : public API::FindClient {
public:
    explicit FindClient(CyberKitFindController* findController)
        : m_findController(findController)
    {
    }

private:
    void didCountStringMatches(WebPageProxy*, const String&, uint32_t matchCount) override
    {
        g_signal_emit(m_findController, signals[COUNTED_MATCHES], 0, matchCount);
    }

    void didFindString(WebPageProxy*, const String&, const Vector<IntRect>&, uint32_t matchCount, int32_t, bool /*didWrapAround*/) override
    {
        g_signal_emit(m_findController, signals[FOUND_TEXT], 0, matchCount);
    }

    void didFailToFindString(WebPageProxy*, const String&) override
    {
        g_signal_emit(m_findController, signals[FAILED_TO_FIND_TEXT], 0);
    }

    CyberKitFindController* m_findController;
};

static void webkitFindControllerConstructed(GObject* object)
{
    G_OBJECT_CLASS(webkit_find_controller_parent_class)->constructed(object);

    CyberKitFindController* findController = WEBKIT_FIND_CONTROLLER(object);
    getPage(findController).setFindClient(makeUnique<FindClient>(findController));
}

static void webkitFindControllerGetProperty(GObject* object, guint propId, GValue* value, GParamSpec* paramSpec)
{
    CyberKitFindController* findController = WEBKIT_FIND_CONTROLLER(object);

    switch (propId) {
    case PROP_TEXT:
        g_value_set_string(value, webkit_find_controller_get_search_text(findController));
        break;
    case PROP_OPTIONS:
        g_value_set_uint(value, webkit_find_controller_get_options(findController));
        break;
    case PROP_MAX_MATCH_COUNT:
        g_value_set_uint(value, webkit_find_controller_get_max_match_count(findController));
        break;
    case PROP_WEB_VIEW:
        g_value_set_object(value, webkit_find_controller_get_web_view(findController));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkitFindControllerSetProperty(GObject* object, guint propId, const GValue* value, GParamSpec* paramSpec)
{
    CyberKitFindController* findController = WEBKIT_FIND_CONTROLLER(object);

    switch (propId) {
    case PROP_WEB_VIEW:
        findController->priv->webView = WEBKIT_WEB_VIEW(g_value_get_object(value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, propId, paramSpec);
    }
}

static void webkit_find_controller_class_init(CyberKitFindControllerClass* findClass)
{
    GObjectClass* gObjectClass = G_OBJECT_CLASS(findClass);
    gObjectClass->constructed = webkitFindControllerConstructed;
    gObjectClass->get_property = webkitFindControllerGetProperty;
    gObjectClass->set_property = webkitFindControllerSetProperty;

    /**
     * CyberKitFindController:text:
     *
     * The current search text for this #CyberKitFindController.
     */
    g_object_class_install_property(gObjectClass,
                                    PROP_TEXT,
                                    g_param_spec_string("text",
                                                        _("Search text"),
                                                        _("Text to search for in the view"),
                                                        0,
                                                        WEBKIT_PARAM_READABLE));

    /**
     * CyberKitFindController:options:
     *
     * The options to be used in the search operation.
     */
    g_object_class_install_property(gObjectClass,
                                    PROP_OPTIONS,
                                    g_param_spec_flags("options",
                                                       _("Search Options"),
                                                       _("Search options to be used in the search operation"),
                                                       WEBKIT_TYPE_FIND_OPTIONS,
                                                       WEBKIT_FIND_OPTIONS_NONE,
                                                       WEBKIT_PARAM_READABLE));

    /**
     * CyberKitFindController:max-match-count:
     *
     * The maximum number of matches to report for a given search.
     */
    g_object_class_install_property(gObjectClass,
                                    PROP_MAX_MATCH_COUNT,
                                    g_param_spec_uint("max-match-count",
                                                      _("Maximum matches count"),
                                                      _("The maximum number of matches in a given text to report"),
                                                      0, G_MAXUINT, 0,
                                                      WEBKIT_PARAM_READABLE));

    /**
     * CyberKitFindController:web-view:
     *
     * The #CyberKitWebView this controller is associated to.
     */
    g_object_class_install_property(gObjectClass,
                                    PROP_WEB_VIEW,
                                    g_param_spec_object("web-view",
                                                        _("WebView"),
                                                        _("The WebView associated with this find controller"),
                                                        WEBKIT_TYPE_WEB_VIEW,
                                                        static_cast<GParamFlags>(WEBKIT_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY)));

    /**
     * CyberKitFindController::found-text:
     * @find_controller: the #CyberKitFindController
     * @match_count: the number of matches found of the search text
     *
     * This signal is emitted when a given text is found in the web
     * page text. It will be issued if the text is found
     * asynchronously after a call to webkit_find_controller_search(),
     * webkit_find_controller_search_next() or
     * webkit_find_controller_search_previous().
     */
    signals[FOUND_TEXT] =
        g_signal_new("found-text",
                     G_TYPE_FROM_CLASS(gObjectClass),
                     G_SIGNAL_RUN_LAST,
                     0, 0, 0,
                     g_cclosure_marshal_VOID__UINT,
                     G_TYPE_NONE, 1, G_TYPE_UINT);

    /**
     * CyberKitFindController::failed-to-find-text:
     * @find_controller: the #CyberKitFindController
     *
     * This signal is emitted when a search operation does not find
     * any result for the given text. It will be issued if the text
     * is not found asynchronously after a call to
     * webkit_find_controller_search(), webkit_find_controller_search_next()
     * or webkit_find_controller_search_previous().
     */
    signals[FAILED_TO_FIND_TEXT] =
        g_signal_new("failed-to-find-text",
                     G_TYPE_FROM_CLASS(gObjectClass),
                     G_SIGNAL_RUN_LAST,
                     0, 0, 0,
                     g_cclosure_marshal_VOID__VOID,
                     G_TYPE_NONE, 0);

    /**
     * CyberKitFindController::counted-matches:
     * @find_controller: the #CyberKitFindController
     * @match_count: the number of matches of the search text
     *
     * This signal is emitted when the #CyberKitFindController has
     * counted the number of matches for a given text after a call
     * to webkit_find_controller_count_matches().
     */
    signals[COUNTED_MATCHES] =
        g_signal_new("counted-matches",
                     G_TYPE_FROM_CLASS(gObjectClass),
                     G_SIGNAL_RUN_LAST,
                     0, 0, 0,
                     g_cclosure_marshal_VOID__UINT,
                     G_TYPE_NONE, 1, G_TYPE_UINT);
}

/**
 * webkit_find_controller_get_search_text:
 * @find_controller: the #CyberKitFindController
 *
 * Gets the text that @find_controller is currently searching
 * for. This text is passed to either
 * webkit_find_controller_search() or
 * webkit_find_controller_count_matches().
 *
 * Returns: the text to look for in the #CyberKitWebView.
 */
const char* webkit_find_controller_get_search_text(CyberKitFindController* findController)
{
    g_return_val_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController), 0);

    return findController->priv->searchText.data();
}

/**
 * webkit_find_controller_get_options:
 * @find_controller: the #CyberKitFindController
 *
 * Gets a bitmask containing the #CyberKitFindOptions associated with
 * the current search.
 *
 * Returns: a bitmask containing the #CyberKitFindOptions associated
 * with the current search.
 */
guint32 webkit_find_controller_get_options(CyberKitFindController* findController)
{
    g_return_val_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController), WEBKIT_FIND_OPTIONS_NONE);

    return toCyberKitFindOptions(findController->priv->findOptions);
}

/**
 * webkit_find_controller_get_max_match_count:
 * @find_controller: the #CyberKitFindController
 *
 * Gets the maximum number of matches to report during a text
 * lookup. This number is passed as the last argument of
 * webkit_find_controller_search() or
 * webkit_find_controller_count_matches().
 *
 * Returns: the maximum number of matches to report.
 */
guint webkit_find_controller_get_max_match_count(CyberKitFindController* findController)
{
    g_return_val_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController), 0);

    return findController->priv->maxMatchCount;
}

/**
 * webkit_find_controller_get_web_view:
 * @find_controller: the #CyberKitFindController
 *
 * Gets the #CyberKitWebView this find controller is associated to. Do
 * not dereference the returned instance as it belongs to the
 * #CyberKitFindController.
 *
 * Returns: (transfer none): the #CyberKitWebView.
 */
CyberKitWebView* webkit_find_controller_get_web_view(CyberKitFindController* findController)
{
    g_return_val_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController), 0);

    return findController->priv->webView;
}

static void webKitFindControllerPerform(CyberKitFindController* findController, CyberKitFindControllerOperation operation)
{
    CyberKitFindControllerPrivate* priv = findController->priv;
    if (operation == CountOperation) {
        getPage(findController).countStringMatches(String::fromUTF8(priv->searchText.data()),
            static_cast<CyberKit::FindOptions>(priv->findOptions), priv->maxMatchCount);
        return;
    }

    uint32_t findOptions = priv->findOptions;
    if (operation == FindOperation)
        // Unconditionally highlight text matches when the search
        // starts. WK1 API was forcing clients to enable/disable
        // highlighting. Since most of them (all?) where using that
        // feature we decided to simplify the WK2 API and
        // unconditionally show highlights. Both search_next() and
        // search_prev() should not enable highlighting to avoid an
        // extra unmarkAllTextMatches() + markAllTextMatches()
        findOptions |= FindOptionsShowHighlight;

    getPage(findController).findString(String::fromUTF8(priv->searchText.data()), static_cast<CyberKit::FindOptions>(findOptions), priv->maxMatchCount);
}

static inline void webKitFindControllerSetSearchData(CyberKitFindController* findController, const gchar* searchText, guint32 findOptions, guint maxMatchCount)
{
    findController->priv->searchText = searchText;
    findController->priv->findOptions = findOptions;
    findController->priv->maxMatchCount = maxMatchCount;
}

/**
 * webkit_find_controller_search:
 * @find_controller: the #CyberKitFindController
 * @search_text: the text to look for
 * @find_options: a bitmask with the #CyberKitFindOptions used in the search
 * @max_match_count: the maximum number of matches allowed in the search
 *
 * Looks for @search_text in the #CyberKitWebView associated with
 * @find_controller since the beginning of the document highlighting
 * up to @max_match_count matches. The outcome of the search will be
 * asynchronously provided by the #CyberKitFindController::found-text
 * and #CyberKitFindController::failed-to-find-text signals.
 *
 * To look for the next or previous occurrences of the same text
 * with the same find options use webkit_find_controller_search_next()
 * and/or webkit_find_controller_search_previous(). The
 * #CyberKitFindController will use the same text and options for the
 * following searches unless they are modified by another call to this
 * method.
 *
 * Note that if the number of matches is higher than @max_match_count
 * then #CyberKitFindController::found-text will report %G_MAXUINT matches
 * instead of the actual number.
 *
 * Callers should call webkit_find_controller_search_finish() to
 * finish the current search operation.
 */
void webkit_find_controller_search(CyberKitFindController* findController, const gchar* searchText, guint findOptions, guint maxMatchCount)
{
    g_return_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController));
    g_return_if_fail(searchText);
    webKitFindControllerSetSearchData(findController, searchText, toWebFindOptions(findOptions), maxMatchCount);
    webKitFindControllerPerform(findController, FindOperation);
}

/**
 * webkit_find_controller_search_next:
 * @find_controller: the #CyberKitFindController
 *
 * Looks for the next occurrence of the search text.
 *
 * Calling this method before webkit_find_controller_search() or
 * webkit_find_controller_count_matches() is a programming error.
 */
void webkit_find_controller_search_next(CyberKitFindController* findController)
{
    g_return_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController));

    findController->priv->findOptions &= ~FindOptionsBackwards;
    findController->priv->findOptions &= ~FindOptionsShowHighlight;
    webKitFindControllerPerform(findController, FindNextPrevOperation);
}

/**
 * webkit_find_controller_search_previous:
 * @find_controller: the #CyberKitFindController
 *
 * Looks for the previous occurrence of the search text.
 *
 * Calling this method before webkit_find_controller_search() or
 * webkit_find_controller_count_matches() is a programming error.
 */
void webkit_find_controller_search_previous(CyberKitFindController* findController)
{
    g_return_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController));

    findController->priv->findOptions |= FindOptionsBackwards;
    findController->priv->findOptions &= ~FindOptionsShowHighlight;
    webKitFindControllerPerform(findController, FindNextPrevOperation);
}

/**
 * webkit_find_controller_count_matches:
 * @find_controller: the #CyberKitFindController
 * @search_text: the text to look for
 * @find_options: a bitmask with the #CyberKitFindOptions used in the search
 * @max_match_count: the maximum number of matches allowed in the search
 *
 * Counts the number of matches for @search_text found in the
 * #CyberKitWebView with the provided @find_options. The number of
 * matches will be provided by the
 * #CyberKitFindController::counted-matches signal.
 */
void webkit_find_controller_count_matches(CyberKitFindController* findController, const gchar* searchText, guint32 findOptions, guint maxMatchCount)
{
    g_return_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController));
    g_return_if_fail(searchText);

    webKitFindControllerSetSearchData(findController, searchText, toWebFindOptions(findOptions), maxMatchCount);
    webKitFindControllerPerform(findController, CountOperation);
}

/**
 * webkit_find_controller_search_finish:
 * @find_controller: a #CyberKitFindController
 *
 * Finishes a find operation started by
 * webkit_find_controller_search(). It will basically unhighlight
 * every text match found.
 *
 * This method will be typically called when the search UI is
 * closed/hidden by the client application.
 */
void webkit_find_controller_search_finish(CyberKitFindController* findController)
{
    g_return_if_fail(WEBKIT_IS_FIND_CONTROLLER(findController));

    getPage(findController).hideFindUI();
}
