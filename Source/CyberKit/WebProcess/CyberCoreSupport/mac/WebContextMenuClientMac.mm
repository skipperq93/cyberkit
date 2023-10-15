/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "WebContextMenuClient.h"

#if ENABLE(CONTEXT_MENUS)

#import "CyberCoreArgumentCoders.h"
#import "WebPage.h"
#import "WebPageProxyMessages.h"
#import <CyberCore/DictionaryLookup.h>
#import <CyberCore/Editor.h>
#import <CyberCore/LocalFrame.h>
#import <CyberCore/LocalFrameView.h>
#import <CyberCore/Page.h>
#import <CyberCore/TextIndicator.h>
#import <CyberCore/TranslationContextMenuInfo.h>
#import <wtf/text/WTFString.h>

namespace CyberKit {
using namespace CyberCore;

void WebContextMenuClient::lookUpInDictionary(LocalFrame* frame)
{
    m_page->performDictionaryLookupForSelection(*frame, frame->selection().selection(), TextIndicatorPresentationTransition::BounceAndCrossfade);
}

bool WebContextMenuClient::isSpeaking() const
{
    return m_page->isSpeaking();
}

void WebContextMenuClient::speak(const String& string)
{
    m_page->speak(string);
}

void WebContextMenuClient::stopSpeaking()
{
    m_page->stopSpeaking();
}

void WebContextMenuClient::searchWithGoogle(const LocalFrame* frame)
{
    String searchString = frame->editor().selectedText().stripWhiteSpace();
    m_page->send(Messages::WebPageProxy::SearchTheWeb(searchString));
}

void WebContextMenuClient::searchWithSpotlight()
{
    // FIXME: Why do we need to search all the frames like this?
    // Isn't there any function in CyberCore that can do this?
    // If not, can we find a place in CyberCore to put this?
    auto* localMainFrame = dynamicDowncast<LocalFrame>(m_page->corePage()->mainFrame());
    if (!localMainFrame)
        return;

    auto* selectionFrame = [&] () -> LocalFrame* {
        for (Frame* selectionFrame = localMainFrame; selectionFrame; selectionFrame = selectionFrame->tree().traverseNext()) {
            auto* localFrame = dynamicDowncast<LocalFrame>(selectionFrame);
            if (!localFrame)
                continue;
            if (localFrame->selection().isRange())
                return localFrame;
        }
        return nullptr;
    }();
    if (!selectionFrame)
        selectionFrame = localMainFrame;

    String selectedString = selectionFrame->displayStringModifiedByEncoding(selectionFrame->editor().selectedText());

    if (selectedString.isEmpty())
        return;

    m_page->send(Messages::WebPageProxy::SearchWithSpotlight(selectedString));
}

#if HAVE(TRANSLATION_UI_SERVICES)

void WebContextMenuClient::handleTranslation(const CyberCore::TranslationContextMenuInfo& info)
{
    m_page->send(Messages::WebPageProxy::HandleContextMenuTranslation(info));
}

#endif // HAVE(TRANSLATION_UI_SERVICES)

} // namespace CyberKit

#endif // ENABLE(CONTEXT_MENUS)
