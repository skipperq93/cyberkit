/*
 * Copyright (C) 2016-2019 Apple Inc. All rights reserved.
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
#import "WebPaymentCoordinatorProxyCocoa.h"

#if ENABLE(APPLE_PAY)

#import "WKPaymentAuthorizationDelegate.h"
#import "WebPaymentCoordinatorProxy.h"
#import <CyberCore/PaymentAuthorizationStatus.h>
#import <CyberCore/PaymentHeaders.h>
#import <pal/cocoa/PassKitSoftLink.h>
#import <wtf/BlockPtr.h>
#import <wtf/RunLoop.h>
#import <wtf/URL.h>

#if USE(APPLE_INTERNAL_SDK)
#import <CyberKitAdditions/WebPaymentCoordinatorProxyCocoaAdditions.mm>
#else
namespace CyberKit {
static void finishCreating(PKPaymentRequest *, const CyberCore::ApplePaySessionPaymentRequest&) { }
}
#endif

// FIXME: We don't support any platforms without -setThumbnailURLs:, so this can be removed.
@interface PKPaymentRequest ()
@property (nonatomic, strong) NSURL *thumbnailURL;
@end

#if HAVE(PASSKIT_BOUND_INTERFACE_IDENTIFIER)
// FIXME: Remove once rdar://problem/48041516 is widely available in SDKs.
@interface PKPaymentRequest (Staging)
@property (nonatomic, copy) NSString *boundInterfaceIdentifier;
@end
#endif

namespace CyberKit {

void WebPaymentCoordinatorProxy::platformCanMakePaymentsWithActiveCard(const String& merchantIdentifier, const String& domainName, WTF::Function<void(bool)>&& completionHandler)
{
#if PLATFORM(MAC)
    if (!PAL::isPassKitFrameworkAvailable())
        return completionHandler(false);
#endif

    PKCanMakePaymentsWithMerchantIdentifierDomainAndSourceApplication(merchantIdentifier, domainName, m_client.paymentCoordinatorSourceApplicationSecondaryIdentifier(*this), makeBlockPtr([completionHandler = WTFMove(completionHandler)](BOOL canMakePayments, NSError *error) mutable {
        if (error)
            LOG_ERROR("PKCanMakePaymentsWithMerchantIdentifierAndDomain error %@", error);

        RunLoop::main().dispatch([completionHandler = WTFMove(completionHandler), canMakePayments] {
            completionHandler(canMakePayments);
        });
    }).get());
}

void WebPaymentCoordinatorProxy::platformOpenPaymentSetup(const String& merchantIdentifier, const String& domainName, WTF::Function<void(bool)>&& completionHandler)
{
#if PLATFORM(MAC)
    if (!PAL::isPassKitFrameworkAvailable())
        return completionHandler(false);
#endif

    auto passLibrary = adoptNS([PAL::allocPKPassLibraryInstance() init]);
    [passLibrary openPaymentSetupForMerchantIdentifier:merchantIdentifier domain:domainName completion:makeBlockPtr([completionHandler = WTFMove(completionHandler)](BOOL result) mutable {
        RunLoop::main().dispatch([completionHandler = WTFMove(completionHandler), result] {
            completionHandler(result);
        });
    }).get()];
}

static RetainPtr<NSSet> toPKContactFields(const CyberCore::ApplePaySessionPaymentRequest::ContactFields& contactFields)
{
    Vector<NSString *> result;

    if (contactFields.postalAddress)
        result.append(PAL::get_PassKit_PKContactFieldPostalAddress());
    if (contactFields.phone)
        result.append(PAL::get_PassKit_PKContactFieldPhoneNumber());
    if (contactFields.email)
        result.append(PAL::get_PassKit_PKContactFieldEmailAddress());
    if (contactFields.name)
        result.append(PAL::get_PassKit_PKContactFieldName());
    if (contactFields.phoneticName)
        result.append(PAL::get_PassKit_PKContactFieldPhoneticName());

    return adoptNS([[NSSet alloc] initWithObjects:result.data() count:result.size()]);
}

NSDecimalNumber *toDecimalNumber(const String& amount)
{
    if (!amount)
        return [NSDecimalNumber zero];
    return [NSDecimalNumber decimalNumberWithString:amount locale:@{ NSLocaleDecimalSeparator : @"." }];
}

static PKMerchantCapability toPKMerchantCapabilities(const CyberCore::ApplePaySessionPaymentRequest::MerchantCapabilities& merchantCapabilities)
{
    PKMerchantCapability result = 0;
    if (merchantCapabilities.supports3DS)
        result |= PKMerchantCapability3DS;
    if (merchantCapabilities.supportsEMV)
        result |= PKMerchantCapabilityEMV;
    if (merchantCapabilities.supportsCredit)
        result |= PKMerchantCapabilityCredit;
    if (merchantCapabilities.supportsDebit)
        result |= PKMerchantCapabilityDebit;

    return result;
}

static RetainPtr<NSArray> toSupportedNetworks(const Vector<String>& supportedNetworks)
{
    auto result = adoptNS([[NSMutableArray alloc] initWithCapacity:supportedNetworks.size()]);
    for (auto& supportedNetwork : supportedNetworks)
        [result addObject:supportedNetwork];
    return result;
}

static PKShippingType toPKShippingType(CyberCore::ApplePaySessionPaymentRequest::ShippingType shippingType)
{
    switch (shippingType) {
    case CyberCore::ApplePaySessionPaymentRequest::ShippingType::Shipping:
        return PKShippingTypeShipping;

    case CyberCore::ApplePaySessionPaymentRequest::ShippingType::Delivery:
        return PKShippingTypeDelivery;

    case CyberCore::ApplePaySessionPaymentRequest::ShippingType::StorePickup:
        return PKShippingTypeStorePickup;

    case CyberCore::ApplePaySessionPaymentRequest::ShippingType::ServicePickup:
        return PKShippingTypeServicePickup;
    }
}

PKShippingMethod *toPKShippingMethod(const CyberCore::ApplePaySessionPaymentRequest::ShippingMethod& shippingMethod)
{
    PKShippingMethod *result = [PAL::getPKShippingMethodClass() summaryItemWithLabel:shippingMethod.label amount:toDecimalNumber(shippingMethod.amount)];
    [result setIdentifier:shippingMethod.identifier];
    [result setDetail:shippingMethod.detail];

    return result;
}
    
static RetainPtr<NSSet> toNSSet(const Vector<String>& strings)
{
    if (strings.isEmpty())
        return nil;

    auto mutableSet = adoptNS([[NSMutableSet alloc] initWithCapacity:strings.size()]);
    for (auto& string : strings)
        [mutableSet addObject:string];

    return WTFMove(mutableSet);
}

#if HAVE(PASSKIT_API_TYPE)
static PKPaymentRequestAPIType toAPIType(CyberCore::ApplePaySessionPaymentRequest::Requester requester)
{
    switch (requester) {
    case CyberCore::ApplePaySessionPaymentRequest::Requester::ApplePayJS:
        return PKPaymentRequestAPITypeWebJS;
    case CyberCore::ApplePaySessionPaymentRequest::Requester::PaymentRequest:
        return PKPaymentRequestAPITypeWebPaymentRequest;
    }
}
#endif

RetainPtr<PKPaymentRequest> WebPaymentCoordinatorProxy::platformPaymentRequest(const URL& originatingURL, const Vector<URL>& linkIconURLs, const CyberCore::ApplePaySessionPaymentRequest& paymentRequest)
{
    auto result = adoptNS([PAL::allocPKPaymentRequestInstance() init]);

    [result setOriginatingURL:originatingURL];

    // FIXME: We don't support any platforms without -setThumbnailURLs:, so this can be simplified.
    if ([result respondsToSelector:@selector(setThumbnailURLs:)]) {
        auto thumbnailURLs = adoptNS([[NSMutableArray alloc] init]);
        for (auto& linkIconURL : linkIconURLs)
            [thumbnailURLs addObject:static_cast<NSURL *>(linkIconURL)];

        [result setThumbnailURLs:thumbnailURLs.get()];
    } else if (!linkIconURLs.isEmpty())
        [result setThumbnailURL:linkIconURLs[0]];

#if HAVE(PASSKIT_API_TYPE)
    [result setAPIType:toAPIType(paymentRequest.requester())];
#endif

    [result setCountryCode:paymentRequest.countryCode()];
    [result setCurrencyCode:paymentRequest.currencyCode()];
    [result setBillingContact:paymentRequest.billingContact().pkContact()];
    [result setShippingContact:paymentRequest.shippingContact().pkContact()];
    [result setRequiredBillingContactFields:toPKContactFields(paymentRequest.requiredBillingContactFields()).get()];
    [result setRequiredShippingContactFields:toPKContactFields(paymentRequest.requiredShippingContactFields()).get()];

    [result setSupportedNetworks:toSupportedNetworks(paymentRequest.supportedNetworks()).get()];
    [result setMerchantCapabilities:toPKMerchantCapabilities(paymentRequest.merchantCapabilities())];

    [result setShippingType:toPKShippingType(paymentRequest.shippingType())];

    auto shippingMethods = adoptNS([[NSMutableArray alloc] init]);
    for (auto& shippingMethod : paymentRequest.shippingMethods())
        [shippingMethods addObject:toPKShippingMethod(shippingMethod)];
    [result setShippingMethods:shippingMethods.get()];

    [result setPaymentSummaryItems:CyberCore::platformSummaryItems(paymentRequest.total(), paymentRequest.lineItems())];

    [result setExpectsMerchantSession:YES];

    if (!paymentRequest.applicationData().isNull()) {
        auto applicationData = adoptNS([[NSData alloc] initWithBase64EncodedString:paymentRequest.applicationData() options:0]);
        [result setApplicationData:applicationData.get()];
    }

    [result setSupportedCountries:toNSSet(paymentRequest.supportedCountries()).get()];

#if HAVE(PASSKIT_BOUND_INTERFACE_IDENTIFIER)
    // FIXME: Remove -respondsToSelector: check once rdar://problem/48041516 is widely available in SDKs.
    auto& boundInterfaceIdentifier = m_client.paymentCoordinatorBoundInterfaceIdentifier(*this);
    if (!boundInterfaceIdentifier.isEmpty() && [result respondsToSelector:@selector(setBoundInterfaceIdentifier:)])
        [result setBoundInterfaceIdentifier:boundInterfaceIdentifier];
#endif

    // FIXME: Instead of using respondsToSelector, this should use a proper #if version check.
    auto& bundleIdentifier = m_client.paymentCoordinatorSourceApplicationBundleIdentifier(*this);
    if (!bundleIdentifier.isEmpty() && [result respondsToSelector:@selector(setSourceApplicationBundleIdentifier:)])
        [result setSourceApplicationBundleIdentifier:bundleIdentifier];

    auto& secondaryIdentifier = m_client.paymentCoordinatorSourceApplicationSecondaryIdentifier(*this);
    if (!secondaryIdentifier.isEmpty() && [result respondsToSelector:@selector(setSourceApplicationSecondaryIdentifier:)])
        [result setSourceApplicationSecondaryIdentifier:secondaryIdentifier];

#if PLATFORM(IOS_FAMILY)
    auto& serviceType = m_client.paymentCoordinatorCTDataConnectionServiceType(*this);
    if (!serviceType.isEmpty() && [result respondsToSelector:@selector(setCTDataConnectionServiceType:)])
        [result setCTDataConnectionServiceType:serviceType];
#endif

    finishCreating(result.get(), paymentRequest);

    return result;
}

void WebPaymentCoordinatorProxy::platformCompletePaymentSession(const Optional<CyberCore::PaymentAuthorizationResult>& result)
{
    m_authorizationPresenter->completePaymentSession(result);
}

void WebPaymentCoordinatorProxy::platformCompleteMerchantValidation(const CyberCore::PaymentMerchantSession& paymentMerchantSession)
{
    m_authorizationPresenter->completeMerchantValidation(paymentMerchantSession);
}

void WebPaymentCoordinatorProxy::platformCompleteShippingMethodSelection(const Optional<CyberCore::ShippingMethodUpdate>& update)
{
    m_authorizationPresenter->completeShippingMethodSelection(update);
}

void WebPaymentCoordinatorProxy::platformCompleteShippingContactSelection(const Optional<CyberCore::ShippingContactUpdate>& update)
{
    m_authorizationPresenter->completeShippingContactSelection(update);
}

void WebPaymentCoordinatorProxy::platformCompletePaymentMethodSelection(const Optional<CyberCore::PaymentMethodUpdate>& update)
{
    m_authorizationPresenter->completePaymentMethodSelection(update);
}

} // namespace CyberKit

#endif // ENABLE(APPLE_PAY)
