/*
 * Copyright (C) 2004-2016 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "DOMCSSPrimitiveValueInternal.h"

#import <CyberCore/DeprecatedCSSOMCounter.h>
#import <CyberCore/DeprecatedCSSOMPrimitiveValue.h>
#import <CyberCore/DeprecatedCSSOMRGBColor.h>
#import <CyberCore/DeprecatedCSSOMRect.h>
#import "DOMCSSValueInternal.h"
#import "DOMCounterInternal.h"
#import "DOMNodeInternal.h"
#import "DOMRGBColorInternal.h"
#import "DOMRectInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::DeprecatedCSSOMPrimitiveValue*>(reinterpret_cast<CyberCore::DeprecatedCSSOMValue*>(_internal))

@implementation DOMCSSPrimitiveValue

- (unsigned short)primitiveType
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->primitiveType();
}

- (void)setFloatValue:(unsigned short)unitType floatValue:(float)floatValue
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setFloatValue(unitType, floatValue));
}

- (float)getFloatValue:(unsigned short)unitType
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->getFloatValue(unitType));
}

- (void)setStringValue:(unsigned short)stringType stringValue:(NSString *)stringValue
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setStringValue(stringType, stringValue));
}

- (NSString *)getStringValue
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->getStringValue());
}

- (DOMCounter *)getCounterValue
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->getCounterValue()).ptr());
}

- (DOMRect *)getRectValue
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->getRectValue()).ptr());
}

- (DOMRGBColor *)getRGBColorValue
{
    CyberCore::JSMainThreadNullState state;
    return kit(raiseOnDOMError(IMPL->getRGBColorValue()).ptr());
}

@end

@implementation DOMCSSPrimitiveValue (DOMCSSPrimitiveValueDeprecated)

- (void)setFloatValue:(unsigned short)unitType :(float)floatValue
{
    [self setFloatValue:unitType floatValue:floatValue];
}

- (void)setStringValue:(unsigned short)stringType :(NSString *)stringValue
{
    [self setStringValue:stringType stringValue:stringValue];
}

@end

DOMCSSPrimitiveValue *kit(CyberCore::DeprecatedCSSOMPrimitiveValue* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMCSSPrimitiveValue*>(kit(static_cast<CyberCore::DeprecatedCSSOMValue*>(value)));
}

#undef IMPL
