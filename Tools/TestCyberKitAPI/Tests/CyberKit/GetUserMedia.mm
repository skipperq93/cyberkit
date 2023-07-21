/*
 * Copyright (C) 2019 Apple Inc. All rights reserved.
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

#if ENABLE(MEDIA_STREAM)

#import "PlatformUtilities.h"
#import "Test.h"
#import "TestWKWebView.h"
#import "WKWebViewConfigurationExtras.h"
#import <CyberKit/WKPreferencesPrivate.h>
#import <CyberKit/WKProcessPoolPrivate.h>
#import <CyberKit/WKUIDelegatePrivate.h>
#import <CyberKit/WKWebView.h>
#import <CyberKit/WKWebViewConfiguration.h>
#import <CyberKit/_WKProcessPoolConfiguration.h>

static bool done;

@interface GetUserMediaCaptureUIDelegate : NSObject<WKUIDelegate>
- (void)_webView:(WKWebView *)webView requestMediaCaptureAuthorization: (_WKCaptureDevices)devices decisionHandler:(void (^)(BOOL))decisionHandler;
- (void)_webView:(WKWebView *)webView checkUserMediaPermissionForURL:(NSURL *)url mainFrameURL:(NSURL *)mainFrameURL frameIdentifier:(NSUInteger)frameIdentifier decisionHandler:(void (^)(NSString *salt, BOOL authorized))decisionHandler;
@end

@implementation GetUserMediaCaptureUIDelegate
- (void)_webView:(WKWebView *)webView requestMediaCaptureAuthorization: (_WKCaptureDevices)devices decisionHandler:(void (^)(BOOL))decisionHandler
{
    decisionHandler(YES);
}

- (void)_webView:(WKWebView *)webView checkUserMediaPermissionForURL:(NSURL *)url mainFrameURL:(NSURL *)mainFrameURL frameIdentifier:(NSUInteger)frameIdentifier decisionHandler:(void (^)(NSString *salt, BOOL authorized))decisionHandler
{
    decisionHandler(@"0x9876543210", YES);
}
@end

@interface GUMMessageHandler : NSObject <WKScriptMessageHandler>
@end

@implementation GUMMessageHandler
- (void)userContentController:(WKUserContentController *)userContentController didReceiveScriptMessage:(WKScriptMessage *)message
{
    EXPECT_WK_STREQ(@"PASS", [message body]);
    done = true;
}
@end

namespace TestCyberKitAPI {

void waitUntilCaptureState(WKWebView *webView, _WKMediaCaptureState expectedState)
{
    do {
        auto state = [webView _mediaCaptureState];
        if (state == expectedState)
            return;
        TestCyberKitAPI::Util::spinRunLoop(1);
    } while (true);
}

TEST(CyberKit2, CaptureMute)
{
    auto configuration = adoptNS([[WKWebViewConfiguration alloc] init]);
    auto processPoolConfig = adoptNS([[_WKProcessPoolConfiguration alloc] init]);
    auto preferences = [configuration preferences];
    preferences._mediaCaptureRequiresSecureConnection = NO;
    preferences._mediaDevicesEnabled = YES;
    preferences._mockCaptureDevicesEnabled = YES;
    auto webView = [[TestWKWebView alloc] initWithFrame:CGRectMake(0, 0, 320, 500) configuration:configuration.get() processPoolConfiguration:processPoolConfig.get()];
    auto delegate = adoptNS([[GetUserMediaCaptureUIDelegate alloc] init]);
    webView.UIDelegate = delegate.get();

    [webView loadTestPageNamed:@"getUserMedia"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveCamera);

    [webView _setPageMuted: _WKMediaCaptureDevicesMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateMutedCamera);
    [webView _setPageMuted: _WKMediaNoneMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveCamera);

    [webView stringByEvaluatingJavaScript:@"stop()"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateNone);

    [webView stringByEvaluatingJavaScript:@"captureAudio()"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveMicrophone);
    [webView _setPageMuted: _WKMediaCaptureDevicesMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateMutedMicrophone);
    [webView _setPageMuted: _WKMediaNoneMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveMicrophone);

    [webView _setPageMuted: _WKMediaCaptureDevicesMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateMutedMicrophone);

    [webView stringByEvaluatingJavaScript:@"stop()"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateNone);

    [webView stringByEvaluatingJavaScript:@"captureAudioAndVideo()"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveCamera | _WKMediaCaptureStateActiveMicrophone);
    [webView _setPageMuted: _WKMediaCaptureDevicesMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateMutedCamera | _WKMediaCaptureStateMutedMicrophone);
    [webView _setPageMuted: _WKMediaNoneMuted];
    waitUntilCaptureState(webView, _WKMediaCaptureStateActiveCamera | _WKMediaCaptureStateActiveMicrophone);

    [webView stringByEvaluatingJavaScript:@"stop()"];
    waitUntilCaptureState(webView, _WKMediaCaptureStateNone);
}

#if WK_HAVE_C_SPI
TEST(CyberKit, WebAudioAndGetUserMedia)
{
    done = false;

    auto configuration = adoptNS([[WKWebViewConfiguration alloc] init]);
    auto context = adoptWK(TestCyberKitAPI::Util::createContextForInjectedBundleTest("InternalsInjectedBundleTest"));
    configuration.get().processPool = (WKProcessPool *)context.get();
    configuration.get().processPool._configuration.shouldCaptureAudioInUIProcess = NO;

    auto preferences = [configuration preferences];
    preferences._mediaCaptureRequiresSecureConnection = NO;
    preferences._mediaDevicesEnabled = YES;
    preferences._mockCaptureDevicesEnabled = YES;

    auto messageHandler = adoptNS([[GUMMessageHandler alloc] init]);
    [[configuration.get() userContentController] addScriptMessageHandler:messageHandler.get() name:@"gum"];

    auto webView = [[TestWKWebView alloc] initWithFrame:CGRectMake(0, 0, 320, 500) configuration:configuration.get()];

    auto delegate = adoptNS([[GetUserMediaCaptureUIDelegate alloc] init]);
    webView.UIDelegate = delegate.get();

    auto url = adoptWK(Util::createURLForResource("getUserMedia-webaudio", "html"));
    [webView loadTestPageNamed:@"getUserMedia-webaudio"];

    TestCyberKitAPI::Util::run(&done);
    done = false;
}
#endif

} // namespace TestCyberKitAPI

#endif // ENABLE(MEDIA_STREAM)
