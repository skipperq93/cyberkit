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
#import "PaymentAuthorizationPresenter.h"

#if USE(PASSKIT)

#import "WKPaymentAuthorizationDelegate.h"
#import "WebPaymentCoordinatorProxyCocoa.h"
#import <CyberCore/PaymentAuthorizationStatus.h>
#import <CyberCore/PaymentMerchantSession.h>
#import <CyberCore/PaymentMethodUpdate.h>
#import <CyberCore/PaymentSummaryItems.h>
#import <pal/cocoa/PassKitSoftLink.h>

#if HAVE(PASSKIT_GRANULAR_ERRORS)
SOFT_LINK_FRAMEWORK(Contacts);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressCityKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressCountryKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressISOCountryCodeKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressPostalCodeKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressStateKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressStreetKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressSubAdministrativeAreaKey, NSString *);
SOFT_LINK_CONSTANT(Contacts, CNPostalAddressSubLocalityKey, NSString *);
#endif

namespace CyberKit {

// FIXME: Rather than having these free functions scattered about, Apple Pay data types should know
// how to convert themselves to and from their platform representations.

#if HAVE(PASSKIT_GRANULAR_ERRORS)

static PKPaymentAuthorizationStatus toPKPaymentAuthorizationStatus(CyberCore::PaymentAuthorizationStatus status)
{
    switch (status) {
    case CyberCore::PaymentAuthorizationStatus::Success:
        return PKPaymentAuthorizationStatusSuccess;
    case CyberCore::PaymentAuthorizationStatus::Failure:
        return PKPaymentAuthorizationStatusFailure;
    case CyberCore::PaymentAuthorizationStatus::PINRequired:
        return PKPaymentAuthorizationStatusPINRequired;
    case CyberCore::PaymentAuthorizationStatus::PINIncorrect:
        return PKPaymentAuthorizationStatusPINIncorrect;
    case CyberCore::PaymentAuthorizationStatus::PINLockout:
        return PKPaymentAuthorizationStatusPINLockout;
    }
}

static PKPaymentErrorCode toPKPaymentErrorCode(CyberCore::PaymentError::Code code)
{
    switch (code) {
    case CyberCore::PaymentError::Code::Unknown:
        return PKPaymentUnknownError;
    case CyberCore::PaymentError::Code::ShippingContactInvalid:
        return PKPaymentShippingContactInvalidError;
    case CyberCore::PaymentError::Code::BillingContactInvalid:
        return PKPaymentBillingContactInvalidError;
    case CyberCore::PaymentError::Code::AddressUnserviceable:
        return PKPaymentShippingAddressUnserviceableError;
    }
}

static NSError *toNSError(const CyberCore::PaymentError& error)
{
    auto userInfo = adoptNS([[NSMutableDictionary alloc] init]);
    [userInfo setObject:error.message forKey:NSLocalizedDescriptionKey];

    if (error.contactField) {
        NSString *pkContactField = nil;
        NSString *postalAddressKey = nil;

        switch (*error.contactField) {
        case CyberCore::PaymentError::ContactField::PhoneNumber:
            pkContactField = PAL::get_PassKit_PKContactFieldPhoneNumber();
            break;
            
        case CyberCore::PaymentError::ContactField::EmailAddress:
            pkContactField = PAL::get_PassKit_PKContactFieldEmailAddress();
            break;
            
        case CyberCore::PaymentError::ContactField::Name:
            pkContactField = PAL::get_PassKit_PKContactFieldName();
            break;
            
        case CyberCore::PaymentError::ContactField::PhoneticName:
            pkContactField = PAL::get_PassKit_PKContactFieldPhoneticName();
            break;
            
        case CyberCore::PaymentError::ContactField::PostalAddress:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            break;
            
        case CyberCore::PaymentError::ContactField::AddressLines:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressStreetKey();
            break;
            
        case CyberCore::PaymentError::ContactField::SubLocality:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressSubLocalityKey();
            break;
            
        case CyberCore::PaymentError::ContactField::Locality:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressCityKey();
            break;
            
        case CyberCore::PaymentError::ContactField::PostalCode:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressPostalCodeKey();
            break;
            
        case CyberCore::PaymentError::ContactField::SubAdministrativeArea:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressSubAdministrativeAreaKey();
            break;
            
        case CyberCore::PaymentError::ContactField::AdministrativeArea:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressStateKey();
            break;
            
        case CyberCore::PaymentError::ContactField::Country:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressCountryKey();
            break;
            
        case CyberCore::PaymentError::ContactField::CountryCode:
            pkContactField = PAL::get_PassKit_PKContactFieldPostalAddress();
            postalAddressKey = getCNPostalAddressISOCountryCodeKey();
            break;
        }

        [userInfo setObject:pkContactField forKey:PAL::get_PassKit_PKPaymentErrorContactFieldUserInfoKey()];
        if (postalAddressKey)
            [userInfo setObject:postalAddressKey forKey:PAL::get_PassKit_PKPaymentErrorPostalAddressUserInfoKey()];
    }

    return [NSError errorWithDomain:PAL::get_PassKit_PKPaymentErrorDomain() code:toPKPaymentErrorCode(error.code) userInfo:userInfo.get()];
}

static NSArray *toNSErrors(const Vector<CyberCore::PaymentError>& errors)
{
    NSMutableArray *result = [NSMutableArray arrayWithCapacity:errors.size()];
    for (const auto& error : errors) {
        if (NSError *nsError = toNSError(error))
            [result addObject:nsError];
    }
    return result;
}

#endif // HAVE(PASSKIT_GRANULAR_ERRORS)

#if !HAVE(PASSKIT_GRANULAR_ERRORS)

static PKPaymentAuthorizationStatus toPKPaymentAuthorizationStatus(const Optional<CyberCore::PaymentAuthorizationResult>& result)
{
    if (!result)
        return PKPaymentAuthorizationStatusSuccess;

    if (result->errors.size() == 1) {
        auto& error = result->errors[0];
        switch (error.code) {
        case CyberCore::PaymentError::Code::Unknown:
        case CyberCore::PaymentError::Code::AddressUnserviceable:
            return PKPaymentAuthorizationStatusFailure;

        case CyberCore::PaymentError::Code::BillingContactInvalid:
            return PKPaymentAuthorizationStatusInvalidBillingPostalAddress;

        case CyberCore::PaymentError::Code::ShippingContactInvalid:
            if (error.contactField && error.contactField == CyberCore::PaymentError::ContactField::PostalAddress)
                return PKPaymentAuthorizationStatusInvalidShippingPostalAddress;

            return PKPaymentAuthorizationStatusInvalidShippingContact;
        }
    }

    switch (result->status) {
    case CyberCore::PaymentAuthorizationStatus::Success:
        return PKPaymentAuthorizationStatusSuccess;
    case CyberCore::PaymentAuthorizationStatus::Failure:
        return PKPaymentAuthorizationStatusFailure;
    case CyberCore::PaymentAuthorizationStatus::PINRequired:
        return PKPaymentAuthorizationStatusPINRequired;
    case CyberCore::PaymentAuthorizationStatus::PINIncorrect:
        return PKPaymentAuthorizationStatusPINIncorrect;
    case CyberCore::PaymentAuthorizationStatus::PINLockout:
        return PKPaymentAuthorizationStatusPINLockout;
    }

    return PKPaymentAuthorizationStatusFailure;
}

static PKPaymentAuthorizationStatus toPKPaymentAuthorizationStatus(const Optional<CyberCore::ShippingContactUpdate>& update)
{
    if (!update || update->errors.isEmpty())
        return PKPaymentAuthorizationStatusSuccess;

    if (update->errors.size() == 1) {
        auto& error = update->errors[0];
        switch (error.code) {
        case CyberCore::PaymentError::Code::Unknown:
        case CyberCore::PaymentError::Code::AddressUnserviceable:
            return PKPaymentAuthorizationStatusFailure;

        case CyberCore::PaymentError::Code::BillingContactInvalid:
            return PKPaymentAuthorizationStatusInvalidBillingPostalAddress;

        case CyberCore::PaymentError::Code::ShippingContactInvalid:
            if (error.contactField && error.contactField == CyberCore::PaymentError::ContactField::PostalAddress)
                return PKPaymentAuthorizationStatusInvalidShippingPostalAddress;

            return PKPaymentAuthorizationStatusInvalidShippingContact;
        }
    }

    return PKPaymentAuthorizationStatusFailure;
}

#endif // !HAVE(PASSKIT_GRANULAR_ERRORS)

static NSArray *toPKShippingMethods(const Vector<CyberCore::ApplePaySessionPaymentRequest::ShippingMethod>& shippingMethods)
{
    NSMutableArray *pkShippingMethods = [NSMutableArray arrayWithCapacity:shippingMethods.size()];
    for (auto& shippingMethod : shippingMethods)
        [pkShippingMethods addObject:toPKShippingMethod(shippingMethod)];
    return pkShippingMethods;
}

void PaymentAuthorizationPresenter::completeMerchantValidation(const CyberCore::PaymentMerchantSession& merchantSession)
{
    ASSERT(platformDelegate());
    [platformDelegate() completeMerchantValidation:merchantSession.pkPaymentMerchantSession() error:nil];
}

void PaymentAuthorizationPresenter::completePaymentMethodSelection(const Optional<CyberCore::PaymentMethodUpdate>& update)
{
    ASSERT(platformDelegate());
    if (!update) {
        [platformDelegate() completePaymentMethodSelection:nil];
        return;
    }

    [platformDelegate() completePaymentMethodSelection:update->platformUpdate()];
}

void PaymentAuthorizationPresenter::completePaymentSession(const Optional<CyberCore::PaymentAuthorizationResult>& result)
{
    ASSERT(platformDelegate());
#if HAVE(PASSKIT_GRANULAR_ERRORS)
    auto status = result ? toPKPaymentAuthorizationStatus(result->status) : PKPaymentAuthorizationStatusSuccess;
    NSArray *errors = result ? toNSErrors(result->errors) : @[ ];
#else
    auto status = toPKPaymentAuthorizationStatus(result);
    NSArray *errors = @[ ];
#endif
    [platformDelegate() completePaymentSession:status errors:errors didReachFinalState:CyberCore::isFinalStateResult(result)];
}

void PaymentAuthorizationPresenter::completeShippingContactSelection(const Optional<CyberCore::ShippingContactUpdate>& update)
{
    ASSERT(platformDelegate());
    if (!update) {
        [platformDelegate() completeShippingContactSelection:nil];
        return;
    }

#if HAVE(PASSKIT_GRANULAR_ERRORS)
    NSArray *errors = toNSErrors(update->errors);
#else
    NSArray *errors = @[ ];
#endif

    // FIXME: CyberCore::ShippingContactUpdate should know how to convert itself to a PKPaymentRequestShippingContactUpdate.
    auto shippingContactUpdate = adoptNS([PAL::allocPKPaymentRequestShippingContactUpdateInstance() initWithErrors:errors paymentSummaryItems:CyberCore::platformSummaryItems(update->newTotalAndLineItems) shippingMethods:toPKShippingMethods(update->newShippingMethods)]);
    [platformDelegate() completeShippingContactSelection:shippingContactUpdate.get()];
}

void PaymentAuthorizationPresenter::completeShippingMethodSelection(const Optional<CyberCore::ShippingMethodUpdate>& update)
{
    ASSERT(platformDelegate());
    if (!update) {
        [platformDelegate() completeShippingMethodSelection:nil];
        return;
    }

    // FIXME: CyberCore::ShippingMethodUpdate should know how to convert itself to a PKPaymentRequestShippingMethodUpdate.
    auto shippingMethodUpdate = adoptNS([PAL::allocPKPaymentRequestShippingMethodUpdateInstance() initWithPaymentSummaryItems:CyberCore::platformSummaryItems(update->newTotalAndLineItems)]);
    [platformDelegate() completeShippingMethodSelection:shippingMethodUpdate.get()];
}

} // namespace CyberKit

#endif // USE(PASSKIT)
