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

#import "DOMCSSMediaRule.h"

#import <CyberCore/CSSMediaRule.h>
#import <CyberCore/CSSRuleList.h>
#import "DOMCSSRuleInternal.h"
#import "DOMCSSRuleListInternal.h"
#import "DOMMediaListInternal.h"
#import "DOMNodeInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/JSExecState.h>
#import <CyberCore/MediaList.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::CSSMediaRule*>(reinterpret_cast<CyberCore::CSSRule*>(_internal))

@implementation DOMCSSMediaRule

- (DOMMediaList *)media
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->media()));
}

- (DOMCSSRuleList *)cssRules
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->cssRules()));
}

- (unsigned)insertRule:(NSString *)rule index:(unsigned)index
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->insertRule(rule, index));
}

- (void)deleteRule:(unsigned)index
{
    CyberCore::JSMainThreadNullState state;
    return raiseOnDOMError(IMPL->deleteRule(index));
}

@end

@implementation DOMCSSMediaRule (DOMCSSMediaRuleDeprecated)

- (unsigned)insertRule:(NSString *)rule :(unsigned)index
{
    return [self insertRule:rule index:index];
}

@end

#undef IMPL
