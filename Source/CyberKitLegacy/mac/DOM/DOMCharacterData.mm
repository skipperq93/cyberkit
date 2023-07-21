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

#import "DOMCharacterData.h"

#import "DOMElementInternal.h"
#import "DOMNodeInternal.h"
#import <CyberCore/Element.h>
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <CyberCore/CharacterData.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::CharacterData*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMCharacterData

- (NSString *)data
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->data();
}

- (void)setData:(NSString *)newData
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setData(newData);
}

- (unsigned)length
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->length();
}

- (DOMElement *)previousElementSibling
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->previousElementSibling()));
}

- (DOMElement *)nextElementSibling
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->nextElementSibling()));
}

- (NSString *)substringData:(unsigned)offset length:(unsigned)inLength
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->substringData(offset, inLength));
}

- (void)appendData:(NSString *)inData
{
    CyberCore::JSMainThreadNullState state;
    IMPL->appendData(inData);
}

- (void)insertData:(unsigned)offset data:(NSString *)inData
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->insertData(offset, inData));
}

- (void)deleteData:(unsigned)offset length:(unsigned)inLength
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->deleteData(offset, inLength));
}

- (void)replaceData:(unsigned)offset length:(unsigned)inLength data:(NSString *)inData
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->replaceData(offset, inLength, inData));
}

- (void)remove
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->remove());
}

@end

@implementation DOMCharacterData (DOMCharacterDataDeprecated)

- (NSString *)substringData:(unsigned)offset :(unsigned)inLength
{
    return [self substringData:offset length:inLength];
}

- (void)insertData:(unsigned)offset :(NSString *)inData
{
    [self insertData:offset data:inData];
}

- (void)deleteData:(unsigned)offset :(unsigned)inLength
{
    [self deleteData:offset length:inLength];
}

- (void)replaceData:(unsigned)offset :(unsigned)inLength :(NSString *)inData
{
    [self replaceData:offset length:inLength data:inData];
}

@end
