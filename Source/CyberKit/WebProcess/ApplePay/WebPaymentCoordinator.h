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
#include "MessageSender.h"
#include <CyberCore/PaymentCoordinatorClient.h>
#include <CyberCore/PaymentHeaders.h>
#include <wtf/Forward.h>
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/MonotonicTime.h>
#include <wtf/text/StringHash.h>

namespace CyberCore {
class PaymentCoordinator;
class PaymentContact;
class PaymentSessionError;
struct ApplePayShippingMethod;
}

namespace WebKit {

class NetworkProcessConnection;
class WebPage;

class WebPaymentCoordinator final : public CyberCore::PaymentCoordinatorClient, private IPC::MessageReceiver, private IPC::MessageSender {
    WTF_MAKE_FAST_ALLOCATED;
public:
    friend class NetworkProcessConnection;
    explicit WebPaymentCoordinator(WebPage&);
    ~WebPaymentCoordinator();

    void networkProcessConnectionClosed();

private:
    // CyberCore::PaymentCoordinatorClient.
    std::optional<String> validatedPaymentNetwork(const String&) override;
    bool canMakePayments() override;
    void canMakePaymentsWithActiveCard(const String& merchantIdentifier, const String& domainName, CompletionHandler<void(bool)>&&) override;
    void openPaymentSetup(const String& merchantIdentifier, const String& domainName, CompletionHandler<void(bool)>&&) override;
    bool showPaymentUI(const URL& originatingURL, const Vector<URL>& linkIconURLs, const CyberCore::ApplePaySessionPaymentRequest&) override;
    void completeMerchantValidation(const CyberCore::PaymentMerchantSession&) override;
    void completeShippingMethodSelection(std::optional<CyberCore::ApplePayShippingMethodUpdate>&&) override;
    void completeShippingContactSelection(std::optional<CyberCore::ApplePayShippingContactUpdate>&&) override;
    void completePaymentMethodSelection(std::optional<CyberCore::ApplePayPaymentMethodUpdate>&&) override;
#if ENABLE(APPLE_PAY_COUPON_CODE)
    void completeCouponCodeChange(std::optional<CyberCore::ApplePayCouponCodeUpdate>&&) override;
#endif
    void completePaymentSession(CyberCore::ApplePayPaymentAuthorizationResult&&) override;

    void abortPaymentSession() override;
    void cancelPaymentSession() override;

    void paymentCoordinatorDestroyed() override;

    bool isWebPaymentCoordinator() const override { return true; }

    void getSetupFeatures(const CyberCore::ApplePaySetupConfiguration&, const URL&, CompletionHandler<void(Vector<Ref<CyberCore::ApplePaySetupFeature>>&&)>&&) final;
    void beginApplePaySetup(const CyberCore::ApplePaySetupConfiguration&, const URL&, Vector<RefPtr<CyberCore::ApplePaySetupFeature>>&&, CompletionHandler<void(bool)>&&) final;
    void endApplePaySetup() final;

    // IPC::MessageReceiver.
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) override;

    // IPC::MessageSender.
    IPC::Connection* messageSenderConnection() const final;
    uint64_t messageSenderDestinationID() const final;

    // Message handlers.
    void validateMerchant(const String& validationURLString);
    void didAuthorizePayment(const CyberCore::Payment&);
    void didSelectShippingMethod(const CyberCore::ApplePayShippingMethod&);
    void didSelectShippingContact(const CyberCore::PaymentContact&);
    void didSelectPaymentMethod(const CyberCore::PaymentMethod&);
#if ENABLE(APPLE_PAY_COUPON_CODE)
    void didChangeCouponCode(String&& couponCode);
#endif
    void didCancelPaymentSession(CyberCore::PaymentSessionError&&);

    CyberCore::PaymentCoordinator& paymentCoordinator();

    using AvailablePaymentNetworksSet = HashSet<String, ASCIICaseInsensitiveHash>;
    static AvailablePaymentNetworksSet platformAvailablePaymentNetworks();

    WebPage& m_webPage;

    std::optional<AvailablePaymentNetworksSet> m_availablePaymentNetworks;

    MonotonicTime m_timestampOfLastCanMakePaymentsRequest;
    std::optional<bool> m_lastCanMakePaymentsResult;
};

} // namespace WebKit

SPECIALIZE_TYPE_TRAITS_BEGIN(WebKit::WebPaymentCoordinator)
static bool isType(const CyberCore::PaymentCoordinatorClient& paymentCoordinatorClient) { return paymentCoordinatorClient.isWebPaymentCoordinator(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif
