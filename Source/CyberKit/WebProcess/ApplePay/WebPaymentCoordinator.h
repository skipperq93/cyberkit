/*
 * Copyright (C) 2015-2019 Apple Inc. All rights reserved.
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

#if ENABLE(APPLE_PAY)

#include "MessageReceiver.h"
#include <CyberCore/PaymentCoordinatorClient.h>
#include <CyberCore/PaymentHeaders.h>
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/text/StringHash.h>

namespace IPC {
class DataReference;
}

namespace CyberCore {
class PaymentCoordinator;
class PaymentContact;
}

namespace CyberKit {

class WebPage;

class WebPaymentCoordinator final : public CyberCore::PaymentCoordinatorClient, private IPC::MessageReceiver {
public:
    explicit WebPaymentCoordinator(WebPage&);
    ~WebPaymentCoordinator();

private:
    // CyberCore::PaymentCoordinatorClient.
    Optional<String> validatedPaymentNetwork(const String&) override;
    bool canMakePayments() override;
    void canMakePaymentsWithActiveCard(const String& merchantIdentifier, const String& domainName, WTF::Function<void (bool)>&& completionHandler) override;
    void openPaymentSetup(const String& merchantIdentifier, const String& domainName, WTF::Function<void (bool)>&& completionHandler) override;
    bool showPaymentUI(const URL& originatingURL, const Vector<URL>& linkIconURLs, const CyberCore::ApplePaySessionPaymentRequest&) override;
    void completeMerchantValidation(const CyberCore::PaymentMerchantSession&) override;
    void completeShippingMethodSelection(Optional<CyberCore::ShippingMethodUpdate>&&) override;
    void completeShippingContactSelection(Optional<CyberCore::ShippingContactUpdate>&&) override;
    void completePaymentMethodSelection(Optional<CyberCore::PaymentMethodUpdate>&&) override;
    void completePaymentSession(Optional<CyberCore::PaymentAuthorizationResult>&&) override;

    void abortPaymentSession() override;
    void cancelPaymentSession() override;

    void paymentCoordinatorDestroyed() override;

    // IPC::MessageReceiver.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // Message handlers.
    void validateMerchant(const String& validationURLString);
    void didAuthorizePayment(const CyberCore::Payment&);
    void didSelectShippingMethod(const CyberCore::ApplePaySessionPaymentRequest::ShippingMethod&);
    void didSelectShippingContact(const CyberCore::PaymentContact&);
    void didSelectPaymentMethod(const CyberCore::PaymentMethod&);
    void didCancelPaymentSession();
    void canMakePaymentsWithActiveCardReply(uint64_t requestID, bool canMakePayments);
    void openPaymentSetupReply(uint64_t requestID, bool result);

    CyberCore::PaymentCoordinator& paymentCoordinator();
    
    using AvailablePaymentNetworksSet = HashSet<String, ASCIICaseInsensitiveHash>;
    const AvailablePaymentNetworksSet& availablePaymentNetworks();

    WebPage& m_webPage;

    HashMap<uint64_t, WTF::Function<void (bool)>> m_pendingCanMakePaymentsWithActiveCardCallbacks;
    HashMap<uint64_t, WTF::Function<void (bool)>> m_pendingOpenPaymentSetupCallbacks;

    Optional<AvailablePaymentNetworksSet> m_availablePaymentNetworks;

#if USE(APPLE_INTERNAL_SDK)
#import <CyberKitAdditions/WebPaymentCoordinatorAdditions.h>
#endif
};

}
#endif
