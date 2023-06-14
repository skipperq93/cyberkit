/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#include "config.h"

#if WK_HAVE_C_SPI

#include "JavaScriptTest.h"
#include "PlatformUtilities.h"
#include "PlatformWebView.h"

namespace TestCyberKitAPI {

static bool didFinishLoad;

static void didFinishNavigation(WKPageRef, WKNavigationRef, WKTypeRef, const void*)
{
    didFinishLoad = true;
}

static void setPageLoaderClient(WKPageRef page)
{
    WKPageNavigationClientV0 loaderClient;
    memset(&loaderClient, 0, sizeof(loaderClient));

    loaderClient.base.version = 0;
    loaderClient.didFinishNavigation = didFinishNavigation;

    WKPageSetPageNavigationClient(page, &loaderClient.base);
}

static void buildAndPerformTest(WKEventMouseButton button, WKEventModifiers modifiers, const char* expectedButton, const char* expectedMenuType)
{
    WKRetainPtr<WKContextRef> context(AdoptWK, WKContextCreateWithConfiguration(nullptr));
    PlatformWebView webView(context.get());
    setPageLoaderClient(webView.page());

    WKRetainPtr<WKURLRef> url(AdoptWK, Util::createURLForResource("mouse-button-listener", "html"));
    WKPageLoadURL(webView.page(), url.get());
    Util::run(&didFinishLoad);

    didFinishLoad = false;

    webView.simulateButtonClick(button, 10, 10, modifiers);

    EXPECT_JS_EQ(webView.page(), "pressedMouseButton()", expectedButton);
    EXPECT_JS_EQ(webView.page(), "displayedMenu()", expectedMenuType);
}

TEST(CyberKit, MenuAndButtonForNormalLeftClick)
{
    buildAndPerformTest(kWKEventMouseButtonLeftButton, 0, "0", "none");
}

TEST(CyberKit, MenuAndButtonForNormalRightClick)
{
    buildAndPerformTest(kWKEventMouseButtonRightButton, 0, "2", "context");
}

TEST(CyberKit, MenuAndButtonForNormalMiddleClick)
{
    buildAndPerformTest(kWKEventMouseButtonMiddleButton, 0, "1", "none");
}

TEST(CyberKit, MenuAndButtonForControlLeftClick)
{
    buildAndPerformTest(kWKEventMouseButtonLeftButton, kWKEventModifiersControlKey, "0", "context");
}

TEST(CyberKit, MenuAndButtonForControlRightClick)
{
    buildAndPerformTest(kWKEventMouseButtonRightButton, kWKEventModifiersControlKey, "2", "context");
}

TEST(CyberKit, MenuAndButtonForControlMiddleClick)
{
    buildAndPerformTest(kWKEventMouseButtonMiddleButton, kWKEventModifiersControlKey, "1", "none");
}

TEST(CyberKit, MenuAndButtonForShiftLeftClick)
{
    buildAndPerformTest(kWKEventMouseButtonLeftButton, kWKEventModifiersShiftKey, "0", "none");
}

TEST(CyberKit, MenuAndButtonForShiftRightClick)
{
    buildAndPerformTest(kWKEventMouseButtonRightButton, kWKEventModifiersShiftKey, "2", "context");
}

TEST(CyberKit, MenuAndButtonForShiftMiddleClick)
{
    buildAndPerformTest(kWKEventMouseButtonMiddleButton, kWKEventModifiersShiftKey, "1", "none");
}

TEST(CyberKit, MenuAndButtonForCommandLeftClick)
{
    buildAndPerformTest(kWKEventMouseButtonLeftButton, kWKEventModifiersMetaKey, "0", "none");
}

TEST(CyberKit, MenuAndButtonForCommandRightClick)
{
    buildAndPerformTest(kWKEventMouseButtonRightButton, kWKEventModifiersMetaKey, "2", "context");
}

TEST(CyberKit, MenuAndButtonForCommandMiddleClick)
{
    buildAndPerformTest(kWKEventMouseButtonMiddleButton, kWKEventModifiersMetaKey, "1", "none");
}

TEST(CyberKit, MenuAndButtonForAltLeftClick)
{
    buildAndPerformTest(kWKEventMouseButtonLeftButton, kWKEventModifiersAltKey, "0", "none");
}

TEST(CyberKit, MenuAndButtonForAltRightClick)
{
    buildAndPerformTest(kWKEventMouseButtonRightButton, kWKEventModifiersAltKey, "2", "context");
}

TEST(CyberKit, MenuAndButtonForAltMiddleClick)
{
    buildAndPerformTest(kWKEventMouseButtonMiddleButton, kWKEventModifiersAltKey, "1", "none");
}
    
} // namespace TestCyberKitAPI

#endif
