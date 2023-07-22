/*
 * Copyright (C) 2012 Apple Inc. All rights reserved.
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
#import "WKGeolocationProviderIOS.h"

#if PLATFORM(IOS_FAMILY)

#import "UIKitSPI.h"
#import <Foundation/NSURL.h>
#import <CyberCore/SecurityOrigin.h>
#import <wtf/RetainPtr.h>
#import <wtf/text/WTFString.h>

// FIXME: Remove use of CyberKit1 from CyberKit2
#import <CyberKit/WebAllowDenyPolicyListener.h>
#import <CyberKit/WebSecurityOriginPrivate.h>

@interface WebSecurityOrigin (WebInternal)
- (id)_initWithCyberCoreSecurityOrigin:(CyberCore::SecurityOrigin *)origin;
@end

namespace CyberKit {

void decidePolicyForGeolocationRequestFromOrigin(CyberCore::SecurityOrigin*, const String& urlString, id<WebAllowDenyPolicyListener>, UIView *);

void decidePolicyForGeolocationRequestFromOrigin(CyberCore::SecurityOrigin* origin, const String& urlString, id<WebAllowDenyPolicyListener> listener, UIView *view)
{
    RetainPtr<WebSecurityOrigin> securityOrigin = adoptNS([[WebSecurityOrigin alloc] _initWithCyberCoreSecurityOrigin:origin]);
    RetainPtr<NSURL> requestUrl = adoptNS([[NSURL alloc] initWithString:urlString]);
    RetainPtr<UIWebGeolocationPolicyDecider> decider = [UIWebGeolocationPolicyDecider sharedPolicyDecider];
    if ([decider respondsToSelector:@selector(decidePolicyForGeolocationRequestFromOrigin:requestingURL:view:listener:)])
        [decider decidePolicyForGeolocationRequestFromOrigin:securityOrigin.get() requestingURL:requestUrl.get() view:view listener:listener];
    else
        [decider decidePolicyForGeolocationRequestFromOrigin:securityOrigin.get() requestingURL:requestUrl.get() window:view.window listener:listener];
}

} // namespace CyberKit

#endif // PLATFORM(IOS_FAMILY)
