/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "GeneratedSerializers.h"

template<size_t...> struct MembersInCorrectOrder;
template<size_t onlyOffset> struct MembersInCorrectOrder<onlyOffset> { static constexpr bool value = true; };
template<size_t firstOffset, size_t secondOffset, size_t... remainingOffsets> struct MembersInCorrectOrder<firstOffset, secondOffset, remainingOffsets...> {
    static constexpr bool value = firstOffset > secondOffset ? false : MembersInCorrectOrder<secondOffset, remainingOffsets...>::value;
};

#if COMPILER(GCC)
IGNORE_WARNINGS_BEGIN("invalid-offsetof")
#endif
#if ENABLE(TEST_FEATURE)
#include "FirstMemberType.h"
#endif
#include "HeaderWithoutCondition"
#if ENABLE(TEST_FEATURE)
#include "SecondMemberType.h"
#endif
#if ENABLE(TEST_FEATURE)
#include "StructHeader.h"
#endif
#include <Namespace/EmptyConstructorNullable.h>
#include <Namespace/EmptyConstructorStruct.h>
#include <Namespace/ReturnRefClass.h>
#include <CyberCore/FloatBoxExtent.h>
#include <CyberCore/InheritanceGrandchild.h>
#include <CyberCore/InheritsFrom.h>
#include <CyberCore/TimingFunction.h>
#include <wtf/CreateUsingClass.h>
#include <wtf/Seconds.h>

namespace IPC {


template<> struct ArgumentCoder<Namespace::OtherClass> {
    static void encode(Encoder&, const Namespace::OtherClass&);
    static std::optional<Namespace::OtherClass> decode(Decoder&);
};


#if ENABLE(TEST_FEATURE)

void ArgumentCoder<Namespace::Subnamespace::StructName>::encode(Encoder& encoder, const Namespace::Subnamespace::StructName& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.firstMemberName)>, FirstMemberType>);
#if ENABLE(SECOND_MEMBER)
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.secondMemberName)>, SecondMemberType>);
#endif
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.nullableTestMember)>, RetainPtr<CFTypeRef>>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(Namespace::Subnamespace::StructName, firstMemberName)
#if ENABLE(SECOND_MEMBER)
        , offsetof(Namespace::Subnamespace::StructName, secondMemberName)
#endif
        , offsetof(Namespace::Subnamespace::StructName, nullableTestMember)
    >::value);
    encoder << instance.firstMemberName;
#if ENABLE(SECOND_MEMBER)
    encoder << instance.secondMemberName;
#endif
    encoder << !!instance.nullableTestMember;
    if (!!instance.nullableTestMember)
        encoder << instance.nullableTestMember;
}

void ArgumentCoder<Namespace::Subnamespace::StructName>::encode(OtherEncoder& encoder, const Namespace::Subnamespace::StructName& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.firstMemberName)>, FirstMemberType>);
#if ENABLE(SECOND_MEMBER)
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.secondMemberName)>, SecondMemberType>);
#endif
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.nullableTestMember)>, RetainPtr<CFTypeRef>>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(Namespace::Subnamespace::StructName, firstMemberName)
#if ENABLE(SECOND_MEMBER)
        , offsetof(Namespace::Subnamespace::StructName, secondMemberName)
#endif
        , offsetof(Namespace::Subnamespace::StructName, nullableTestMember)
    >::value);
    encoder << instance.firstMemberName;
#if ENABLE(SECOND_MEMBER)
    encoder << instance.secondMemberName;
#endif
    encoder << !!instance.nullableTestMember;
    if (!!instance.nullableTestMember)
        encoder << instance.nullableTestMember;
}

std::optional<Namespace::Subnamespace::StructName> ArgumentCoder<Namespace::Subnamespace::StructName>::decode(Decoder& decoder)
{
    auto firstMemberName = decoder.decode<FirstMemberType>();
    if (!firstMemberName)
        return std::nullopt;

#if ENABLE(SECOND_MEMBER)
    auto secondMemberName = decoder.decode<SecondMemberType>();
    if (!secondMemberName)
        return std::nullopt;
#endif

    auto hasnullableTestMember = decoder.decode<bool>();
    if (!hasnullableTestMember)
        return std::nullopt;
    std::optional<RetainPtr<CFTypeRef>> nullableTestMember;
    if (*hasnullableTestMember) {
        nullableTestMember = decoder.decode<RetainPtr<CFTypeRef>>();
        if (!nullableTestMember)
            return std::nullopt;
    } else
        nullableTestMember = std::optional<RetainPtr<CFTypeRef>> { RetainPtr<CFTypeRef> { } };

    return {
        Namespace::Subnamespace::StructName {
            WTFMove(*firstMemberName),
#if ENABLE(SECOND_MEMBER)
            WTFMove(*secondMemberName),
#endif
            WTFMove(*nullableTestMember)
        }
    };
}

#endif


void ArgumentCoder<Namespace::OtherClass>::encode(Encoder& encoder, const Namespace::OtherClass& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.isNull)>, bool>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.b)>, bool>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.dataDetectorResults)>, RetainPtr<NSArray>>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(Namespace::OtherClass, isNull)
        , offsetof(Namespace::OtherClass, a)
        , offsetof(Namespace::OtherClass, dataDetectorResults)
    >::value);
    encoder << instance.isNull;
    encoder << instance.a;
    encoder << instance.b;
    encoder << instance.dataDetectorResults;
}

std::optional<Namespace::OtherClass> ArgumentCoder<Namespace::OtherClass>::decode(Decoder& decoder)
{
    auto isNull = decoder.decode<bool>();
    if (!isNull)
        return std::nullopt;

    auto a = decoder.decode<int>();
    if (!a)
        return std::nullopt;

    auto b = decoder.decode<bool>();
    if (!b)
        return std::nullopt;

    auto dataDetectorResults = IPC::decode<NSArray>(decoder, @[ NSArray.class, PAL::getDDScannerResultClass() ]);
    if (!dataDetectorResults)
        return std::nullopt;

    return {
        Namespace::OtherClass {
            WTFMove(*isNull),
            WTFMove(*a),
            WTFMove(*b),
            WTFMove(*dataDetectorResults)
        }
    };
}


void ArgumentCoder<Namespace::ReturnRefClass>::encode(Encoder& encoder, const Namespace::ReturnRefClass& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.functionCall().member1)>, double>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.functionCall().member2)>, double>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.uniqueMember)>, std::unique_ptr<int>>);
    encoder << instance.functionCall().member1;
    encoder << instance.functionCall().member2;
    encoder << instance.uniqueMember;
}

std::optional<Ref<Namespace::ReturnRefClass>> ArgumentCoder<Namespace::ReturnRefClass>::decode(Decoder& decoder)
{
    auto functionCallmember1 = decoder.decode<double>();
    if (!functionCallmember1)
        return std::nullopt;

    auto functionCallmember2 = decoder.decode<double>();
    if (!functionCallmember2)
        return std::nullopt;

    auto uniqueMember = decoder.decode<std::unique_ptr<int>>();
    if (!uniqueMember)
        return std::nullopt;

    return {
        Namespace::ReturnRefClass::create(
            WTFMove(*functionCallmember1),
            WTFMove(*functionCallmember2),
            WTFMove(*uniqueMember)
        )
    };
}


void ArgumentCoder<Namespace::EmptyConstructorStruct>::encode(Encoder& encoder, const Namespace::EmptyConstructorStruct& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.m_int)>, int>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.m_double)>, double>);
    encoder << instance.m_int;
    encoder << instance.m_double;
}

std::optional<Namespace::EmptyConstructorStruct> ArgumentCoder<Namespace::EmptyConstructorStruct>::decode(Decoder& decoder)
{
    auto m_int = decoder.decode<int>();
    if (!m_int)
        return std::nullopt;

    auto m_double = decoder.decode<double>();
    if (!m_double)
        return std::nullopt;

    Namespace::EmptyConstructorStruct result;
    result.m_int = WTFMove(*m_int);
    result.m_double = WTFMove(*m_double);
    return { WTFMove(result) };
}


void ArgumentCoder<Namespace::EmptyConstructorNullable>::encode(Encoder& encoder, const Namespace::EmptyConstructorNullable& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.m_isNull)>, bool>);
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.m_type)>, MemberType>);
#endif
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.m_value)>, OtherMemberType>);
#endif
    static_assert(MembersInCorrectOrder<0
        , offsetof(Namespace::EmptyConstructorNullable, m_isNull)
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
        , offsetof(Namespace::EmptyConstructorNullable, m_type)
#endif
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
        , offsetof(Namespace::EmptyConstructorNullable, m_value)
#endif
    >::value);
    encoder << instance.m_isNull;
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    encoder << instance.m_type;
#endif
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    encoder << instance.m_value;
#endif
}

std::optional<Namespace::EmptyConstructorNullable> ArgumentCoder<Namespace::EmptyConstructorNullable>::decode(Decoder& decoder)
{
    auto m_isNull = decoder.decode<bool>();
    if (!m_isNull)
        return std::nullopt;

#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    auto m_type = decoder.decode<MemberType>();
    if (!m_type)
        return std::nullopt;
#endif

#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    auto m_value = decoder.decode<OtherMemberType>();
    if (!m_value)
        return std::nullopt;
#endif

    Namespace::EmptyConstructorNullable result;
    result.m_isNull = WTFMove(*m_isNull);
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    result.m_type = WTFMove(*m_type);
#endif
#if CONDITION_AROUND_M_TYPE_AND_M_VALUE
    result.m_value = WTFMove(*m_value);
#endif
    return { WTFMove(result) };
}


void ArgumentCoder<WithoutNamespace>::encode(Encoder& encoder, const WithoutNamespace& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WithoutNamespace, a)
    >::value);
    encoder << instance.a;
}

std::optional<WithoutNamespace> ArgumentCoder<WithoutNamespace>::decode(Decoder& decoder)
{
    auto a = decoder.decode<int>();
    if (!a)
        return std::nullopt;

    return {
        WithoutNamespace {
            WTFMove(*a)
        }
    };
}


void ArgumentCoder<WithoutNamespaceWithAttributes>::encode(Encoder& encoder, const WithoutNamespaceWithAttributes& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WithoutNamespaceWithAttributes, a)
    >::value);
    encoder << instance.a;
}

void ArgumentCoder<WithoutNamespaceWithAttributes>::encode(OtherEncoder& encoder, const WithoutNamespaceWithAttributes& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WithoutNamespaceWithAttributes, a)
    >::value);
    encoder << instance.a;
}

std::optional<WithoutNamespaceWithAttributes> ArgumentCoder<WithoutNamespaceWithAttributes>::decode(Decoder& decoder)
{
    auto a = decoder.decode<int>();
    if (!a)
        return std::nullopt;

    return {
        WithoutNamespaceWithAttributes {
            WTFMove(*a)
        }
    };
}


void ArgumentCoder<CyberCore::InheritsFrom>::encode(Encoder& encoder, const CyberCore::InheritsFrom& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WithoutNamespace, a)
    >::value);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.b)>, float>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(CyberCore::InheritsFrom, b)
    >::value);
    encoder << instance.a;
    encoder << instance.b;
}

std::optional<CyberCore::InheritsFrom> ArgumentCoder<CyberCore::InheritsFrom>::decode(Decoder& decoder)
{
    auto a = decoder.decode<int>();
    if (!a)
        return std::nullopt;

    auto b = decoder.decode<float>();
    if (!b)
        return std::nullopt;

    return {
        CyberCore::InheritsFrom {
            WithoutNamespace {
                WTFMove(*a)
            },
            WTFMove(*b)
        }
    };
}


void ArgumentCoder<CyberCore::InheritanceGrandchild>::encode(Encoder& encoder, const CyberCore::InheritanceGrandchild& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.a)>, int>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WithoutNamespace, a)
    >::value);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.b)>, float>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(CyberCore::InheritsFrom, b)
    >::value);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.c)>, double>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(CyberCore::InheritanceGrandchild, c)
    >::value);
    encoder << instance.a;
    encoder << instance.b;
    encoder << instance.c;
}

std::optional<CyberCore::InheritanceGrandchild> ArgumentCoder<CyberCore::InheritanceGrandchild>::decode(Decoder& decoder)
{
    auto a = decoder.decode<int>();
    if (!a)
        return std::nullopt;

    auto b = decoder.decode<float>();
    if (!b)
        return std::nullopt;

    auto c = decoder.decode<double>();
    if (!c)
        return std::nullopt;

    return {
        CyberCore::InheritanceGrandchild {
            CyberCore::InheritsFrom {
                WithoutNamespace {
                    WTFMove(*a)
                },
                WTFMove(*b)
            },
            WTFMove(*c)
        }
    };
}


void ArgumentCoder<WTF::Seconds>::encode(Encoder& encoder, const WTF::Seconds& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.value())>, double>);
    encoder << instance.value();
}

std::optional<WTF::Seconds> ArgumentCoder<WTF::Seconds>::decode(Decoder& decoder)
{
    auto value = decoder.decode<double>();
    if (!value)
        return std::nullopt;

    return {
        WTF::Seconds {
            WTFMove(*value)
        }
    };
}


void ArgumentCoder<WTF::CreateUsingClass>::encode(Encoder& encoder, const WTF::CreateUsingClass& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.value)>, double>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(WTF::CreateUsingClass, value)
    >::value);
    encoder << instance.value;
}

std::optional<WTF::CreateUsingClass> ArgumentCoder<WTF::CreateUsingClass>::decode(Decoder& decoder)
{
    auto value = decoder.decode<double>();
    if (!value)
        return std::nullopt;

    return {
        WTF::CreateUsingClass::fromDouble(
            WTFMove(*value)
        )
    };
}


void ArgumentCoder<CyberCore::FloatBoxExtent>::encode(Encoder& encoder, const CyberCore::FloatBoxExtent& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.top())>, float>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.right())>, float>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.bottom())>, float>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.left())>, float>);
    encoder << instance.top();
    encoder << instance.right();
    encoder << instance.bottom();
    encoder << instance.left();
}

std::optional<CyberCore::FloatBoxExtent> ArgumentCoder<CyberCore::FloatBoxExtent>::decode(Decoder& decoder)
{
    auto top = decoder.decode<float>();
    if (!top)
        return std::nullopt;

    auto right = decoder.decode<float>();
    if (!right)
        return std::nullopt;

    auto bottom = decoder.decode<float>();
    if (!bottom)
        return std::nullopt;

    auto left = decoder.decode<float>();
    if (!left)
        return std::nullopt;

    return {
        CyberCore::FloatBoxExtent {
            WTFMove(*top),
            WTFMove(*right),
            WTFMove(*bottom),
            WTFMove(*left)
        }
    };
}


void ArgumentCoder<NullableSoftLinkedMember>::encode(Encoder& encoder, const NullableSoftLinkedMember& instance)
{
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.firstMember)>, RetainPtr<DDActionContext>>);
    static_assert(std::is_same_v<std::remove_cvref_t<decltype(instance.secondMember)>, RetainPtr<DDActionContext>>);
    static_assert(MembersInCorrectOrder<0
        , offsetof(NullableSoftLinkedMember, firstMember)
        , offsetof(NullableSoftLinkedMember, secondMember)
    >::value);
    encoder << !!instance.firstMember;
    if (!!instance.firstMember)
        encoder << instance.firstMember;
    encoder << instance.secondMember;
}

std::optional<NullableSoftLinkedMember> ArgumentCoder<NullableSoftLinkedMember>::decode(Decoder& decoder)
{
    auto hasfirstMember = decoder.decode<bool>();
    if (!hasfirstMember)
        return std::nullopt;
    std::optional<RetainPtr<DDActionContext>> firstMember;
    if (*hasfirstMember) {
        firstMember = IPC::decode<DDActionContext>(decoder, PAL::getDDActionContextClass());
        if (!firstMember)
            return std::nullopt;
    } else
        firstMember = std::optional<RetainPtr<DDActionContext>> { RetainPtr<DDActionContext> { } };

    auto secondMember = IPC::decode<DDActionContext>(decoder, PAL::getDDActionContextClass());
    if (!secondMember)
        return std::nullopt;

    return {
        NullableSoftLinkedMember {
            WTFMove(*firstMember),
            WTFMove(*secondMember)
        }
    };
}

enum class CyberCore_TimingFunction_Subclass : IPC::EncodedVariantIndex {
    LinearTimingFunction,
    CubicBezierTimingFunction,
    StepsTimingFunction,
    SpringTimingFunction
};

void ArgumentCoder<CyberCore::TimingFunction>::encode(Encoder& encoder, const CyberCore::TimingFunction& instance)
{
    if (auto* subclass = dynamicDowncast<CyberCore::LinearTimingFunction>(instance)) {
        encoder << CyberCore_TimingFunction_Subclass::LinearTimingFunction;
        encoder << *subclass;
    }
    if (auto* subclass = dynamicDowncast<CyberCore::CubicBezierTimingFunction>(instance)) {
        encoder << CyberCore_TimingFunction_Subclass::CubicBezierTimingFunction;
        encoder << *subclass;
    }
    if (auto* subclass = dynamicDowncast<CyberCore::StepsTimingFunction>(instance)) {
        encoder << CyberCore_TimingFunction_Subclass::StepsTimingFunction;
        encoder << *subclass;
    }
    if (auto* subclass = dynamicDowncast<CyberCore::SpringTimingFunction>(instance)) {
        encoder << CyberCore_TimingFunction_Subclass::SpringTimingFunction;
        encoder << *subclass;
    }
}

std::optional<Ref<CyberCore::TimingFunction>> ArgumentCoder<CyberCore::TimingFunction>::decode(Decoder& decoder)
{
    std::optional<CyberCore_TimingFunction_Subclass> type;
    decoder >> type;
    if (!type)
        return std::nullopt;

    if (type == CyberCore_TimingFunction_Subclass::LinearTimingFunction) {
        std::optional<Ref<CyberCore::LinearTimingFunction>> result;
        decoder >> result;
        if (!result)
            return std::nullopt;
        return WTFMove(*result);
    }

    if (type == CyberCore_TimingFunction_Subclass::CubicBezierTimingFunction) {
        std::optional<Ref<CyberCore::CubicBezierTimingFunction>> result;
        decoder >> result;
        if (!result)
            return std::nullopt;
        return WTFMove(*result);
    }

    if (type == CyberCore_TimingFunction_Subclass::StepsTimingFunction) {
        std::optional<Ref<CyberCore::StepsTimingFunction>> result;
        decoder >> result;
        if (!result)
            return std::nullopt;
        return WTFMove(*result);
    }

    if (type == CyberCore_TimingFunction_Subclass::SpringTimingFunction) {
        std::optional<Ref<CyberCore::SpringTimingFunction>> result;
        decoder >> result;
        if (!result)
            return std::nullopt;
        return WTFMove(*result);
    }

    ASSERT_NOT_REACHED();
    return std::nullopt;
}

} // namespace IPC

namespace WTF {

template<> bool isValidEnum<IPC::CyberCore_TimingFunction_Subclass, void>(IPC::EncodedVariantIndex value)
{
    switch (static_cast<IPC::CyberCore_TimingFunction_Subclass>(value)) {
    case IPC::CyberCore_TimingFunction_Subclass::LinearTimingFunction:
    case IPC::CyberCore_TimingFunction_Subclass::CubicBezierTimingFunction:
    case IPC::CyberCore_TimingFunction_Subclass::StepsTimingFunction:
    case IPC::CyberCore_TimingFunction_Subclass::SpringTimingFunction:
        return true;
    default:
        return false;
    }
}

template<> bool isValidEnum<EnumWithoutNamespace, void>(uint8_t value)
{
    switch (static_cast<EnumWithoutNamespace>(value)) {
    case EnumWithoutNamespace::Value1:
    case EnumWithoutNamespace::Value2:
    case EnumWithoutNamespace::Value3:
        return true;
    default:
        return false;
    }
}

#if ENABLE(UINT16_ENUM)
template<> bool isValidEnum<EnumNamespace::EnumType, void>(uint16_t value)
{
    switch (static_cast<EnumNamespace::EnumType>(value)) {
    case EnumNamespace::EnumType::FirstValue:
#if ENABLE(ENUM_VALUE_CONDITION)
    case EnumNamespace::EnumType::SecondValue:
#endif
        return true;
    default:
        return false;
    }
}
#endif

template<> bool isValidOptionSet<EnumNamespace2::OptionSetEnumType>(OptionSet<EnumNamespace2::OptionSetEnumType> value)
{
    constexpr uint8_t allValidBitsValue =
        static_cast<uint8_t>(EnumNamespace2::OptionSetEnumType::OptionSetFirstValue)
#if ENABLE(OPTION_SET_SECOND_VALUE)
        | static_cast<uint8_t>(EnumNamespace2::OptionSetEnumType::OptionSetSecondValue)
#endif
        | static_cast<uint8_t>(EnumNamespace2::OptionSetEnumType::OptionSetThirdValue);
    return (value.toRaw() | allValidBitsValue) == allValidBitsValue;
}

} // namespace WTF

#if COMPILER(GCC)
IGNORE_WARNINGS_END
#endif
