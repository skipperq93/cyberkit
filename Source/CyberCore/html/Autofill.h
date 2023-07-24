/*
 * Copyright (C) 2016-2020 Apple Inc. All rights reserved.
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

#include <wtf/EnumTraits.h>
#include <wtf/text/AtomString.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {

enum class AutofillMantle {
    Expectation,
    Anchor
};

enum class AutofillFieldName {
    None,
    Name,
    HonorificPrefix,
    GivenName,
    AdditionalName,
    FamilyName,
    HonorificSuffix,
    Nickname,
    Username,
    NewPassword,
    CurrentPassword,
    OrganizationTitle,
    Organization,
    StreetAddress,
    AddressLine1,
    AddressLine2,
    AddressLine3,
    AddressLevel4,
    AddressLevel3,
    AddressLevel2,
    AddressLevel1,
    Country,
    CountryName,
    PostalCode,
    CcName,
    CcGivenName,
    CcAdditionalName,
    CcFamilyName,
    CcNumber,
    CcExp,
    CcExpMonth,
    CcExpYear,
    CcCsc,
    CcType,
    TransactionCurrency,
    TransactionAmount,
    Language,
    Bday,
    BdayDay,
    BdayMonth,
    BdayYear,
    Sex,
    URL,
    Photo,
    Tel,
    TelCountryCode,
    TelNational,
    TelAreaCode,
    TelLocal,
    TelLocalPrefix,
    TelLocalSuffix,
    TelExtension,
    Email,
    Impp
};

WEBCORE_EXPORT AutofillFieldName toAutofillFieldName(const AtomString&);

class HTMLFormControlElement;

class AutofillData {
public:
    static AutofillData createFromHTMLFormControlElement(const HTMLFormControlElement&);

    AutofillData(const AtomString& fieldName, const String& idlExposedValue)
        : fieldName(fieldName)
        , idlExposedValue(idlExposedValue)
    {
    }

    // We could add support for hint tokens and scope tokens if those ever became useful to anyone.

    AtomString fieldName;
    String idlExposedValue;
};

} // namespace CyberCore

namespace WTF {

template<> struct EnumTraits<CyberCore::AutofillFieldName> {
    using values = EnumValues<
        CyberCore::AutofillFieldName,
        CyberCore::AutofillFieldName::None,
        CyberCore::AutofillFieldName::Name,
        CyberCore::AutofillFieldName::HonorificPrefix,
        CyberCore::AutofillFieldName::GivenName,
        CyberCore::AutofillFieldName::AdditionalName,
        CyberCore::AutofillFieldName::FamilyName,
        CyberCore::AutofillFieldName::HonorificSuffix,
        CyberCore::AutofillFieldName::Nickname,
        CyberCore::AutofillFieldName::Username,
        CyberCore::AutofillFieldName::NewPassword,
        CyberCore::AutofillFieldName::CurrentPassword,
        CyberCore::AutofillFieldName::OrganizationTitle,
        CyberCore::AutofillFieldName::Organization,
        CyberCore::AutofillFieldName::StreetAddress,
        CyberCore::AutofillFieldName::AddressLine1,
        CyberCore::AutofillFieldName::AddressLine2,
        CyberCore::AutofillFieldName::AddressLine3,
        CyberCore::AutofillFieldName::AddressLevel4,
        CyberCore::AutofillFieldName::AddressLevel3,
        CyberCore::AutofillFieldName::AddressLevel2,
        CyberCore::AutofillFieldName::AddressLevel1,
        CyberCore::AutofillFieldName::Country,
        CyberCore::AutofillFieldName::CountryName,
        CyberCore::AutofillFieldName::PostalCode,
        CyberCore::AutofillFieldName::CcName,
        CyberCore::AutofillFieldName::CcGivenName,
        CyberCore::AutofillFieldName::CcAdditionalName,
        CyberCore::AutofillFieldName::CcFamilyName,
        CyberCore::AutofillFieldName::CcNumber,
        CyberCore::AutofillFieldName::CcExp,
        CyberCore::AutofillFieldName::CcExpMonth,
        CyberCore::AutofillFieldName::CcExpYear,
        CyberCore::AutofillFieldName::CcCsc,
        CyberCore::AutofillFieldName::CcType,
        CyberCore::AutofillFieldName::TransactionCurrency,
        CyberCore::AutofillFieldName::TransactionAmount,
        CyberCore::AutofillFieldName::Language,
        CyberCore::AutofillFieldName::Bday,
        CyberCore::AutofillFieldName::BdayDay,
        CyberCore::AutofillFieldName::BdayMonth,
        CyberCore::AutofillFieldName::BdayYear,
        CyberCore::AutofillFieldName::Sex,
        CyberCore::AutofillFieldName::URL,
        CyberCore::AutofillFieldName::Photo,
        CyberCore::AutofillFieldName::Tel,
        CyberCore::AutofillFieldName::TelCountryCode,
        CyberCore::AutofillFieldName::TelNational,
        CyberCore::AutofillFieldName::TelAreaCode,
        CyberCore::AutofillFieldName::TelLocal,
        CyberCore::AutofillFieldName::TelLocalPrefix,
        CyberCore::AutofillFieldName::TelLocalSuffix,
        CyberCore::AutofillFieldName::TelExtension,
        CyberCore::AutofillFieldName::Email,
        CyberCore::AutofillFieldName::Impp
    >;
};

} // namespace WTF
