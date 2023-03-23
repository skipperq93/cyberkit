/*
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
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

#import "DOMDocumentFragmentInternal.h"
#import "DOMExtensions.h"
#import "DOMHTMLCollectionInternal.h"
#import "DOMHTMLDocumentInternal.h"
#import "DOMHTMLInputElementInternal.h"
#import "DOMHTMLSelectElementInternal.h"
#import "DOMHTMLTextAreaElementInternal.h"
#import "DOMNodeInternal.h"
#import "DOMPrivate.h"
#import <CyberCore/DocumentFragment.h>
#import <CyberCore/FrameView.h>
#import <CyberCore/HTMLCollection.h>
#import <CyberCore/HTMLDocument.h>
#import <CyberCore/HTMLInputElement.h>
#import <CyberCore/HTMLParserIdioms.h>
#import <CyberCore/HTMLSelectElement.h>
#import <CyberCore/HTMLTextAreaElement.h>
#import <CyberCore/Range.h>
#import <CyberCore/RenderTextControl.h>
#import <CyberCore/Settings.h>
#import <CyberCore/SimpleRange.h>
#import <CyberCore/markup.h>

#if PLATFORM(IOS_FAMILY)
#import "DOMHTMLElementInternal.h"
#import <CyberCore/Autocapitalize.h>
#import <CyberCore/HTMLTextFormControlElement.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/RenderLayer.h>
#import <CyberCore/RenderLayerScrollableArea.h>
#import <CyberCore/WAKWindow.h>
#import <CyberCore/CyberCoreThreadMessage.h>
#endif

// FIXME: We should move all these into the various specific element source files.
// These were originally here because they were hand written and the rest generated,
// but that is no longer true.

#if PLATFORM(IOS_FAMILY)

@implementation DOMHTMLElement (DOMHTMLElementExtensions)

- (int)scrollXOffset
{
    auto* renderer = core(self)->renderer();
    if (!renderer)
        return 0;

    if (!is<CyberCore::RenderBlockFlow>(*renderer))
        renderer = renderer->containingBlock();

    if (!is<CyberCore::RenderBox>(*renderer) || !renderer->hasNonVisibleOverflow())
        return 0;

    auto* layer = downcast<CyberCore::RenderBox>(*renderer).layer();
    if (!layer)
        return 0;
    auto* scrollableArea = layer->scrollableArea();
    if (!scrollableArea)
        return 0;

    return scrollableArea->scrollOffset().x();
}

- (int)scrollYOffset
{
    auto* renderer = core(self)->renderer();
    if (!renderer)
        return 0;

    if (!is<CyberCore::RenderBlockFlow>(*renderer))
        renderer = renderer->containingBlock();
    if (!is<CyberCore::RenderBox>(*renderer) || !renderer->hasNonVisibleOverflow())
        return 0;

    auto* layer = downcast<CyberCore::RenderBox>(*renderer).layer();
    if (!layer)
        return 0;
    auto* scrollableArea = layer->scrollableArea();
    if (!scrollableArea)
        return 0;

    return scrollableArea->scrollOffset().y();
}

- (void)setScrollXOffset:(int)x scrollYOffset:(int)y
{
    [self setScrollXOffset:x scrollYOffset:y adjustForIOSCaret:NO];
}

- (void)setScrollXOffset:(int)x scrollYOffset:(int)y adjustForIOSCaret:(BOOL)adjustForIOSCaret
{
    auto* renderer = core(self)->renderer();
    if (!renderer)
        return;

    if (!is<CyberCore::RenderBlockFlow>(*renderer))
        renderer = renderer->containingBlock();
    if (!renderer->hasNonVisibleOverflow() || !is<CyberCore::RenderBox>(*renderer))
        return;

    auto* layer = downcast<CyberCore::RenderBox>(*renderer).layer();
    if (!layer)
        return;
    auto* scrollableArea = layer->ensureLayerScrollableArea();

    auto scrollPositionChangeOptions = CyberCore::ScrollPositionChangeOptions::createProgrammatic();
    scrollPositionChangeOptions.clamping = CyberCore::ScrollClamping::Unclamped;
    scrollableArea->scrollToOffset(CyberCore::ScrollOffset(x, y), scrollPositionChangeOptions);
}

- (void)absolutePosition:(int *)x :(int *)y :(int *)w :(int *)h
{
    auto* renderer = core(self)->renderBox();
    if (renderer) {
        if (w)
            *w = renderer->width();
        if (h)
            *h = renderer->width();
        if (x && y) {
            CyberCore::FloatPoint floatPoint(*x, *y);
            renderer->localToAbsolute(floatPoint);
            CyberCore::IntPoint point = roundedIntPoint(floatPoint);
            *x = point.x();
            *y = point.y();
        }
    }
}

@end

#endif // PLATFORM(IOS_FAMILY)

//------------------------------------------------------------------------------------------
// DOMHTMLDocument

@implementation DOMHTMLDocument (DOMHTMLDocumentExtensions)

- (DOMDocumentFragment *)createDocumentFragmentWithMarkupString:(NSString *)markupString baseURL:(NSURL *)baseURL
{
    return kit(createFragmentFromMarkup(*core(self), markupString, [baseURL absoluteString]).ptr());
}

- (DOMDocumentFragment *)createDocumentFragmentWithText:(NSString *)text
{
    // FIXME: Since this is not a contextual fragment, it won't handle whitespace properly.
    return kit(createFragmentFromText(makeRangeSelectingNodeContents(*core(self)), text).ptr());
}

@end

@implementation DOMHTMLDocument (WebPrivate)

- (DOMDocumentFragment *)_createDocumentFragmentWithMarkupString:(NSString *)markupString baseURLString:(NSString *)baseURLString
{
    NSURL *baseURL = core(self)->completeURL(CyberCore::stripLeadingAndTrailingHTMLSpaces(baseURLString));
    return [self createDocumentFragmentWithMarkupString:markupString baseURL:baseURL];
}

- (DOMDocumentFragment *)_createDocumentFragmentWithText:(NSString *)text
{
    return [self createDocumentFragmentWithText:text];
}

@end

@implementation DOMHTMLInputElement (FormAutoFillTransition)

- (BOOL)_isTextField
{
    return core(self)->isTextField();
}

@end

@implementation DOMHTMLSelectElement (FormAutoFillTransition)

- (void)_activateItemAtIndex:(int)index
{
    // Use the setSelectedIndexByUser function so a change event will be fired. <rdar://problem/6760590>
    if (CyberCore::HTMLSelectElement* select = core(self))
        select->optionSelectedByUser(index, true);
}

- (void)_activateItemAtIndex:(int)index allowMultipleSelection:(BOOL)allowMultipleSelection
{
    // Use the setSelectedIndexByUser function so a change event will be fired. <rdar://problem/6760590>
    // If this is a <select multiple> the allowMultipleSelection flag will allow setting multiple
    // selections without clearing the other selections.
    if (CyberCore::HTMLSelectElement* select = core(self))
        select->optionSelectedByUser(index, true, allowMultipleSelection);
}

@end

#if PLATFORM(IOS_FAMILY)

@implementation DOMHTMLInputElement (FormPromptAdditions)

- (BOOL)_isEdited
{
    return core(self)->lastChangeWasUserEdit();
}

@end

@implementation DOMHTMLTextAreaElement (FormPromptAdditions)

- (BOOL)_isEdited
{
    return core(self)->lastChangeWasUserEdit();
}

@end

static WebAutocapitalizeType webAutocapitalizeType(CyberCore::AutocapitalizeType type)
{
    switch (type) {
    case CyberCore::AutocapitalizeType::Default:
        return WebAutocapitalizeTypeDefault;
    case CyberCore::AutocapitalizeType::None:
        return WebAutocapitalizeTypeNone;
    case CyberCore::AutocapitalizeType::Words:
        return WebAutocapitalizeTypeWords;
    case CyberCore::AutocapitalizeType::Sentences:
        return WebAutocapitalizeTypeSentences;
    case CyberCore::AutocapitalizeType::AllCharacters:
        return WebAutocapitalizeTypeAllCharacters;
    }
}

@implementation DOMHTMLInputElement (AutocapitalizeAdditions)

- (WebAutocapitalizeType)_autocapitalizeType
{
    CyberCore::HTMLInputElement* inputElement = core(self);
    return webAutocapitalizeType(inputElement->autocapitalizeType());
}

@end

@implementation DOMHTMLTextAreaElement (AutocapitalizeAdditions)

- (WebAutocapitalizeType)_autocapitalizeType
{
    CyberCore::HTMLTextAreaElement* textareaElement = core(self);
    return webAutocapitalizeType(textareaElement->autocapitalizeType());
}

@end

@implementation DOMHTMLInputElement (WebInputChangeEventAdditions)

- (void)setValueWithChangeEvent:(NSString *)newValue
{
    CyberCore::JSMainThreadNullState state;
    core(self)->setValue(newValue, CyberCore::DispatchInputAndChangeEvent);
}

- (void)setValueAsNumberWithChangeEvent:(double)newValueAsNumber
{
    CyberCore::JSMainThreadNullState state;
    core(self)->setValueAsNumber(newValueAsNumber, CyberCore::DispatchInputAndChangeEvent);
}

@end

#endif

Class kitClass(CyberCore::HTMLCollection* collection)
{
    if (collection->type() == CyberCore::SelectOptions)
        return [DOMHTMLOptionsCollection class];
    return [DOMHTMLCollection class];
}
