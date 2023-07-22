/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#include "config.h"
#include "WKBundleNodeHandle.h"
#include "WKBundleNodeHandlePrivate.h"

#include "InjectedBundleNodeHandle.h"
#include "InjectedBundleRangeHandle.h"
#include "WKAPICast.h"
#include "WKBundleAPICast.h"
#include "WebFrame.h"
#include "WebImage.h"
#include <CyberCore/HTMLTextFormControlElement.h>

static CyberCore::AutoFillButtonType toAutoFillButtonType(WKAutoFillButtonType wkAutoFillButtonType)
{
    switch (wkAutoFillButtonType) {
    case kWKAutoFillButtonTypeNone:
        return CyberCore::AutoFillButtonType::None;
    case kWKAutoFillButtonTypeContacts:
        return CyberCore::AutoFillButtonType::Contacts;
    case kWKAutoFillButtonTypeCredentials:
        return CyberCore::AutoFillButtonType::Credentials;
    case kWKAutoFillButtonTypeStrongPassword:
        return CyberCore::AutoFillButtonType::StrongPassword;
    case kWKAutoFillButtonTypeCreditCard:
        return CyberCore::AutoFillButtonType::CreditCard;
    }
    ASSERT_NOT_REACHED();
    return CyberCore::AutoFillButtonType::None;
}

static WKAutoFillButtonType toWKAutoFillButtonType(CyberCore::AutoFillButtonType autoFillButtonType)
{
    switch (autoFillButtonType) {
    case CyberCore::AutoFillButtonType::None:
        return kWKAutoFillButtonTypeNone;
    case CyberCore::AutoFillButtonType::Contacts:
        return kWKAutoFillButtonTypeContacts;
    case CyberCore::AutoFillButtonType::Credentials:
        return kWKAutoFillButtonTypeCredentials;
    case CyberCore::AutoFillButtonType::StrongPassword:
        return kWKAutoFillButtonTypeStrongPassword;
    case CyberCore::AutoFillButtonType::CreditCard:
        return kWKAutoFillButtonTypeCreditCard;
    }
    ASSERT_NOT_REACHED();
    return kWKAutoFillButtonTypeNone;
}

WKTypeID WKBundleNodeHandleGetTypeID()
{
    return CyberKit::toAPI(CyberKit::InjectedBundleNodeHandle::APIType);
}

WKBundleNodeHandleRef WKBundleNodeHandleCreate(JSContextRef contextRef, JSObjectRef objectRef)
{
    RefPtr<CyberKit::InjectedBundleNodeHandle> nodeHandle = CyberKit::InjectedBundleNodeHandle::getOrCreate(contextRef, objectRef);
    return toAPI(nodeHandle.leakRef());
}

WKBundleNodeHandleRef WKBundleNodeHandleCopyDocument(WKBundleNodeHandleRef nodeHandleRef)
{
    RefPtr<CyberKit::InjectedBundleNodeHandle> nodeHandle = CyberKit::toImpl(nodeHandleRef)->document();
    return toAPI(nodeHandle.leakRef());
}

WKRect WKBundleNodeHandleGetRenderRect(WKBundleNodeHandleRef nodeHandleRef, bool* isReplaced)
{
    return CyberKit::toAPI(CyberKit::toImpl(nodeHandleRef)->renderRect(isReplaced));
}

WKImageRef WKBundleNodeHandleCopySnapshotWithOptions(WKBundleNodeHandleRef nodeHandleRef, WKSnapshotOptions options)
{
    RefPtr<CyberKit::WebImage> image = CyberKit::toImpl(nodeHandleRef)->renderedImage(CyberKit::toSnapshotOptions(options), options & kWKSnapshotOptionsExcludeOverflow);
    return toAPI(image.leakRef());
}

WKBundleRangeHandleRef WKBundleNodeHandleCopyVisibleRange(WKBundleNodeHandleRef nodeHandleRef)
{
    RefPtr<CyberKit::InjectedBundleRangeHandle> rangeHandle = CyberKit::toImpl(nodeHandleRef)->visibleRange();
    return toAPI(rangeHandle.leakRef());
}

WKRect WKBundleNodeHandleGetElementBounds(WKBundleNodeHandleRef elementHandleRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(elementHandleRef)->elementBounds());
}

void WKBundleNodeHandleSetHTMLInputElementValueForUser(WKBundleNodeHandleRef htmlInputElementHandleRef, WKStringRef valueRef)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementValueForUser(CyberKit::toWTFString(valueRef));
}

void WKBundleNodeHandleSetHTMLInputElementSpellcheckEnabled(WKBundleNodeHandleRef htmlInputElementHandleRef, bool enabled)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementSpellcheckEnabled(enabled);
}

bool WKBundleNodeHandleGetHTMLInputElementAutoFilled(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toImpl(htmlInputElementHandleRef)->isHTMLInputElementAutoFilled();
}

void WKBundleNodeHandleSetHTMLInputElementAutoFilled(WKBundleNodeHandleRef htmlInputElementHandleRef, bool filled)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementAutoFilled(filled);
}

void WKBundleNodeHandleSetHTMLInputElementAutoFilledAndViewable(WKBundleNodeHandleRef htmlInputElementHandleRef, bool autoFilledAndViewable)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementAutoFilledAndViewable(autoFilledAndViewable);
}

bool WKBundleNodeHandleGetHTMLInputElementAutoFillButtonEnabled(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toImpl(htmlInputElementHandleRef)->isHTMLInputElementAutoFillButtonEnabled();
}

void WKBundleNodeHandleSetHTMLInputElementAutoFillButtonEnabledWithButtonType(WKBundleNodeHandleRef htmlInputElementHandleRef, WKAutoFillButtonType autoFillButtonType)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementAutoFillButtonEnabled(toAutoFillButtonType(autoFillButtonType));
}

WKAutoFillButtonType WKBundleNodeHandleGetHTMLInputElementAutoFillButtonType(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return toWKAutoFillButtonType(CyberKit::toImpl(htmlInputElementHandleRef)->htmlInputElementAutoFillButtonType());
}

WKAutoFillButtonType WKBundleNodeHandleGetHTMLInputElementLastAutoFillButtonType(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return toWKAutoFillButtonType(CyberKit::toImpl(htmlInputElementHandleRef)->htmlInputElementLastAutoFillButtonType());
}

bool WKBundleNodeHandleGetHTMLInputElementAutoFillAvailable(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toImpl(htmlInputElementHandleRef)->isAutoFillAvailable();
}

void WKBundleNodeHandleSetHTMLInputElementAutoFillAvailable(WKBundleNodeHandleRef htmlInputElementHandleRef, bool autoFillAvailable)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setAutoFillAvailable(autoFillAvailable);
}

WKRect WKBundleNodeHandleGetHTMLInputElementAutoFillButtonBounds(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toAPI(CyberKit::toImpl(htmlInputElementHandleRef)->htmlInputElementAutoFillButtonBounds());
}

bool WKBundleNodeHandleGetHTMLInputElementLastChangeWasUserEdit(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toImpl(htmlInputElementHandleRef)->htmlInputElementLastChangeWasUserEdit();
}

bool WKBundleNodeHandleGetHTMLTextAreaElementLastChangeWasUserEdit(WKBundleNodeHandleRef htmlTextAreaElementHandleRef)
{
    return CyberKit::toImpl(htmlTextAreaElementHandleRef)->htmlTextAreaElementLastChangeWasUserEdit();
}

WKBundleNodeHandleRef WKBundleNodeHandleCopyHTMLTableCellElementCellAbove(WKBundleNodeHandleRef htmlTableCellElementHandleRef)
{
    RefPtr<CyberKit::InjectedBundleNodeHandle> nodeHandle = CyberKit::toImpl(htmlTableCellElementHandleRef)->htmlTableCellElementCellAbove();
    return toAPI(nodeHandle.leakRef());
}

WKBundleFrameRef WKBundleNodeHandleCopyDocumentFrame(WKBundleNodeHandleRef documentHandleRef)
{
    RefPtr<CyberKit::WebFrame> frame = CyberKit::toImpl(documentHandleRef)->documentFrame();
    return toAPI(frame.leakRef());
}

WKBundleFrameRef WKBundleNodeHandleCopyHTMLFrameElementContentFrame(WKBundleNodeHandleRef htmlFrameElementHandleRef)
{
    RefPtr<CyberKit::WebFrame> frame = CyberKit::toImpl(htmlFrameElementHandleRef)->htmlFrameElementContentFrame();
    return toAPI(frame.leakRef());
}

WKBundleFrameRef WKBundleNodeHandleCopyHTMLIFrameElementContentFrame(WKBundleNodeHandleRef htmlIFrameElementHandleRef)
{
    RefPtr<CyberKit::WebFrame> frame = CyberKit::toImpl(htmlIFrameElementHandleRef)->htmlIFrameElementContentFrame();
    return toAPI(frame.leakRef());
}

// Deprecated - use WKBundleNodeHandleGetHTMLInputElementAutoFilled(WKBundleNodeHandleRef).
bool WKBundleNodeHandleGetHTMLInputElementAutofilled(WKBundleNodeHandleRef htmlInputElementHandleRef)
{
    return CyberKit::toImpl(htmlInputElementHandleRef)->isHTMLInputElementAutoFilled();
}

// Deprecated - use WKBundleNodeHandleSetHTMLInputElementAutoFilled(WKBundleNodeHandleRef, bool).
void WKBundleNodeHandleSetHTMLInputElementAutofilled(WKBundleNodeHandleRef htmlInputElementHandleRef, bool filled)
{
    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementAutoFilled(filled);
}

// Deprecated - use WKBundleNodeHandleSetHTMLInputElementAutoFillButtonEnabledWithButtonType(WKBundleNodeHandleRef, WKAutoFillButtonType).
void WKBundleNodeHandleSetHTMLInputElementAutoFillButtonEnabled(WKBundleNodeHandleRef htmlInputElementHandleRef, bool enabled)
{
    CyberCore::AutoFillButtonType autoFillButtonType = enabled ? CyberCore::AutoFillButtonType::Credentials : CyberCore::AutoFillButtonType::None;

    CyberKit::toImpl(htmlInputElementHandleRef)->setHTMLInputElementAutoFillButtonEnabled(autoFillButtonType);
}
