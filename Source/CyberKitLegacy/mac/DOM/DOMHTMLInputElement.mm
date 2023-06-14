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

#import "DOMHTMLInputElementInternal.h"

#import "DOMFileListInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMHTMLFormElementInternal.h"
#import "DOMNodeInternal.h"
#import "DOMNodeListInternal.h"
#import "DOMInternal.h"
#import "DOMPrivate.h"
#import "ExceptionHandlers.h"

#if TARGET_OS_IPHONE
#if __has_include(<UIKit/UITextAutofillSuggestion.h>)

#import <UIKit/UITextAutofillSuggestion.h>

#else

@interface UITextSuggestion : NSObject
@end

@interface UITextAutofillSuggestion : UITextSuggestion
@property (nonatomic, assign) NSString *username;
@property (nonatomic, assign) NSString *password;
@end

#endif // __has_include(<UIKit/UITextAutofillSuggestion.h>)
#endif // TARGET_OS_IPHONE

#import <CyberCore/AutofillElements.h>
#import <CyberCore/FileList.h>
#import <CyberCore/HTMLElement.h>
#import <CyberCore/HTMLFormElement.h>
#import <CyberCore/HTMLInputElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/HitTestResult.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/NameNodeList.h>
#import <CyberCore/NodeList.h>
#import <CyberCore/RenderElement.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLInputElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLInputElement

- (NSString *)accept
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::acceptAttr);
}

- (void)setAccept:(NSString *)newAccept
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::acceptAttr, newAccept);
}

- (NSString *)alt
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::altAttr);
}

- (void)setAlt:(NSString *)newAlt
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::altAttr, newAlt);
}

- (NSString *)autocomplete
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->autocomplete();
}

- (void)setAutocomplete:(NSString *)newAutocomplete
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAutocomplete(newAutocomplete);
}

- (BOOL)autofocus
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autofocusAttr);
}

- (void)setAutofocus:(BOOL)newAutofocus
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::autofocusAttr, newAutofocus);
}

- (BOOL)defaultChecked
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::checkedAttr);
}

- (void)setDefaultChecked:(BOOL)newDefaultChecked
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::checkedAttr, newDefaultChecked);
}

- (BOOL)checked
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->checked();
}

- (void)setChecked:(BOOL)newChecked
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setChecked(newChecked);
}

- (NSString *)dirName
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::dirnameAttr);
}

- (void)setDirName:(NSString *)newDirName
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::dirnameAttr, newDirName);
}

- (BOOL)disabled
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::disabledAttr);
}

- (void)setDisabled:(BOOL)newDisabled
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::disabledAttr, newDisabled);
}

- (DOMHTMLFormElement *)form
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->form()));
}

- (DOMFileList *)files
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->files()));
}

- (void)setFiles:(DOMFileList *)newFiles
{
    CyberCore::JSMainThreadNullState state;
    ASSERT(newFiles);

    IMPL->setFiles(core(newFiles));
}

- (NSString *)formAction
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->formAction();
}

- (void)setFormAction:(NSString *)newFormAction
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setFormAction(newFormAction);
}

- (NSString *)formEnctype
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->formEnctype();
}

- (void)setFormEnctype:(NSString *)newFormEnctype
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setFormEnctype(newFormEnctype);
}

- (NSString *)formMethod
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->formMethod();
}

- (void)setFormMethod:(NSString *)newFormMethod
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setFormMethod(newFormMethod);
}

- (BOOL)formNoValidate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::formnovalidateAttr);
}

- (void)setFormNoValidate:(BOOL)newFormNoValidate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::formnovalidateAttr, newFormNoValidate);
}

- (NSString *)formTarget
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::formtargetAttr);
}

- (void)setFormTarget:(NSString *)newFormTarget
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::formtargetAttr, newFormTarget);
}

- (unsigned)height
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->height();
}

- (void)setHeight:(unsigned)newHeight
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setHeight(newHeight);
}

- (BOOL)indeterminate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->indeterminate();
}

- (void)setIndeterminate:(BOOL)newIndeterminate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setIndeterminate(newIndeterminate);
}

#if ENABLE(DATALIST_ELEMENT)
- (DOMHTMLElement *)list
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->list()));
}
#endif

- (NSString *)max
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::maxAttr);
}

- (void)setMax:(NSString *)newMax
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::maxAttr, newMax);
}

- (int)maxLength
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->maxLength();
}

- (void)setMaxLength:(int)newMaxLength
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setMaxLength(newMaxLength));
}

- (NSString *)min
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::minAttr);
}

- (void)setMin:(NSString *)newMin
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::minAttr, newMin);
}

- (BOOL)multiple
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::multipleAttr);
}

- (void)setMultiple:(BOOL)newMultiple
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::multipleAttr, newMultiple);
}

- (NSString *)name
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getNameAttribute();
}

- (void)setName:(NSString *)newName
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::nameAttr, newName);
}

- (NSString *)pattern
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::patternAttr);
}

- (void)setPattern:(NSString *)newPattern
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::patternAttr, newPattern);
}

- (NSString *)placeholder
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::placeholderAttr);
}

- (void)setPlaceholder:(NSString *)newPlaceholder
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::placeholderAttr, newPlaceholder);
}

- (BOOL)readOnly
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::readonlyAttr);
}

- (void)setReadOnly:(BOOL)newReadOnly
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::readonlyAttr, newReadOnly);
}

- (BOOL)required
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::requiredAttr);
}

- (void)setRequired:(BOOL)newRequired
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::requiredAttr, newRequired);
}

- (NSString *)size
{
    CyberCore::JSMainThreadNullState state;
    return WTF::String::number(IMPL->size());
}

- (void)setSize:(NSString *)newSize
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSize(WTF::String(newSize).toInt());
}

- (NSString *)src
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::srcAttr);
}

- (void)setSrc:(NSString *)newSrc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, newSrc);
}

- (NSString *)step
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::stepAttr);
}

- (void)setStep:(NSString *)newStep
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::stepAttr, newStep);
}

- (NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->type();
}

- (void)setType:(NSString *)newType
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setType(newType);
}

- (NSString *)defaultValue
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->defaultValue();
}

- (void)setDefaultValue:(NSString *)newDefaultValue
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDefaultValue(newDefaultValue);
}

- (NSString *)value
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->value();
}

- (void)setValue:(NSString *)newValue
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setValue(newValue);
}

- (NSTimeInterval)valueAsDate
{
    CyberCore::JSMainThreadNullState state;
    return kit(IMPL->valueAsDate());
}

- (void)setValueAsDate:(NSTimeInterval)newValueAsDate
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setValueAsDate(core(newValueAsDate)));
}

- (double)valueAsNumber
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->valueAsNumber();
}

- (void)setValueAsNumber:(double)newValueAsNumber
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setValueAsNumber(newValueAsNumber));
}

- (unsigned)width
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->width();
}

- (void)setWidth:(unsigned)newWidth
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setWidth(newWidth);
}

- (BOOL)willValidate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->willValidate();
}

- (NSString *)validationMessage
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->validationMessage();
}

- (DOMNodeList *)labels
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->labels()));
}

- (int)selectionStart
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->selectionStart();
}

- (void)setSelectionStart:(int)newSelectionStart
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSelectionStart(newSelectionStart);
}

- (int)selectionEnd
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->selectionEnd();
}

- (void)setSelectionEnd:(int)newSelectionEnd
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSelectionEnd(newSelectionEnd);
}

- (NSString *)selectionDirection
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->selectionDirection();
}

- (void)setSelectionDirection:(NSString *)newSelectionDirection
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSelectionDirection(newSelectionDirection);
}

- (NSString *)align
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::alignAttr);
}

- (void)setAlign:(NSString *)newAlign
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::alignAttr, newAlign);
}

- (NSString *)useMap
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::usemapAttr);
}

- (void)setUseMap:(NSString *)newUseMap
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::usemapAttr, newUseMap);
}

- (BOOL)incremental
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::incrementalAttr);
}

- (void)setIncremental:(BOOL)newIncremental
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::incrementalAttr, newIncremental);
}

- (NSString *)accessKey
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::accesskeyAttr);
}

- (void)setAccessKey:(NSString *)newAccessKey
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::accesskeyAttr, newAccessKey);
}

- (NSString *)altDisplayString
{
    CyberCore::JSMainThreadNullState state;
    return CyberCore::displayString(IMPL->alt(), core(self));
}

- (NSURL *)absoluteImageURL
{
    CyberCore::JSMainThreadNullState state;
    if (!IMPL->renderer() || !IMPL->renderer()->isImage())
        return nil;
    return [self _getURLAttribute:@"src"];
}

#if ENABLE(MEDIA_CAPTURE)
- (BOOL)capture
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::captureAttr);
}

- (void)setCapture:(BOOL)newCapture
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::captureAttr, newCapture);
}
#endif

- (void)stepUp:(int)n
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->stepUp(n));
}

- (void)stepDown:(int)n
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->stepDown(n));
}

- (BOOL)checkValidity
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->checkValidity();
}

- (void)setCustomValidity:(NSString *)error
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCustomValidity(error);
}

- (void)select
{
    CyberCore::JSMainThreadNullState state;
    IMPL->select();
}

- (void)setRangeText:(NSString *)replacement
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setRangeText(replacement));
}

- (void)setRangeText:(NSString *)replacement start:(unsigned)start end:(unsigned)end selectionMode:(NSString *)selectionMode
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setRangeText(replacement, start, end, selectionMode));
}

- (void)setSelectionRange:(int)start end:(int)end
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setSelectionRange(start, end);
}

- (void)click
{
    CyberCore::JSMainThreadNullState state;
    IMPL->click();
}

- (void)setValueForUser:(NSString *)inValue
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setValueForUser(inValue);
}

- (NSDictionary *)_autofillContext
{
    CyberCore::JSMainThreadNullState state;
    if (!CyberCore::AutofillElements::computeAutofillElements(*IMPL))
        return nil;

    NSURL *documentURL = [NSURL URLWithString:self.ownerDocument.URL];
    if (!documentURL)
        return nil;

    return @{ @"_WebViewURL" : documentURL };
}

#if TARGET_OS_IPHONE
- (void)insertTextSuggestion:(UITextAutofillSuggestion *)credentialSuggestion
{
    CyberCore::JSMainThreadNullState state;
    if (is<CyberCore::HTMLInputElement>(IMPL)) {
        if (auto autofillElements = CyberCore::AutofillElements::computeAutofillElements(*IMPL))
            autofillElements->autofill(credentialSuggestion.username, credentialSuggestion.password);
    }
}
#endif // TARGET_OS_IPHONE

@end

CyberCore::HTMLInputElement* core(DOMHTMLInputElement *wrapper)
{
    return wrapper ? reinterpret_cast<CyberCore::HTMLInputElement*>(wrapper->_internal) : 0;
}

DOMHTMLInputElement *kit(CyberCore::HTMLInputElement* value)
{
    CyberCoreThreadViolationCheckRoundOne();
    return static_cast<DOMHTMLInputElement*>(kit(static_cast<CyberCore::Node*>(value)));
}
