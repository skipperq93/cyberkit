/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#pragma once

#import "AuthenticationServicesCoreSPI.h"
#import <wtf/SoftLinking.h>

SOFT_LINK_FRAMEWORK_FOR_HEADER(CyberKit, AuthenticationServicesCore);

SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCCyberKitSPISupport);

#if HAVE(ASC_AUTH_UI) || HAVE(UNIFIED_ASC_AUTH_UI)

SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCAgentProxy);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCAppleIDCredential);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCAuthorizationPresentationContext);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCAuthorizationPresenter);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCAuthorizationRemotePresenter);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCCredentialRequestContext);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCWebAuthenticationExtensionsClientInputs);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPlatformPublicKeyCredentialAssertion);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPlatformPublicKeyCredentialLoginChoice);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPlatformPublicKeyCredentialRegistration);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPublicKeyCredentialAssertionOptions);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPublicKeyCredentialCreationOptions);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCPublicKeyCredentialDescriptor);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCSecurityKeyPublicKeyCredentialAssertion);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCSecurityKeyPublicKeyCredentialLoginChoice);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASCSecurityKeyPublicKeyCredentialRegistration);
SOFT_LINK_CLASS_FOR_HEADER(CyberKit, ASGlobalFrameIdentifier);

SOFT_LINK_CONSTANT_FOR_HEADER(CyberKit, AuthenticationServicesCore, ASCAuthorizationErrorDomain, NSErrorDomain);
#define ASCAuthorizationErrorDomain CyberKit::get_AuthenticationServicesCore_ASCAuthorizationErrorDomain()

SOFT_LINK_CONSTANT_FOR_HEADER(CyberKit, AuthenticationServicesCore, ASCPINValidationResultKey, NSString*);
#define ASCPINValidationResultKey CyberKit::get_AuthenticationServicesCore_ASCPINValidationResultKey()

#endif // HAVE(ASC_AUTH_UI)
