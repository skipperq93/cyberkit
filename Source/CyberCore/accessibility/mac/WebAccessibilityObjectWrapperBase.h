/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "AccessibilityObjectInterface.h"
#import "FontPlatformData.h"
#import <CoreGraphics/CoreGraphics.h>
#import <variant>
#import <wtf/RefPtr.h>
#import <wtf/WeakPtr.h>

namespace CyberCore {
struct AccessibilitySearchCriteria;
class AccessibilityObject;
class AXIsolatedObject;
class Document;
class IntRect;
class FloatPoint;
class HTMLTextFormControlElement;
class Path;
class VisiblePosition;

// NSAttributedString support.
// FIXME: move to a new AXAttributedStringBuilder class. For now, these
// functions are implemented in AccessibilityObjectCocoa.mm. Additional helper
// functions are implemented in AccessibilityObjectMac or IOS .mm respectively.
bool attributedStringContainsRange(NSAttributedString *, const NSRange&);
void attributedStringSetNumber(NSMutableAttributedString *, NSString *, NSNumber *, const NSRange&);
void attributedStringSetFont(NSMutableAttributedString *, CTFontRef, const NSRange&);
void attributedStringSetSpelling(NSMutableAttributedString *, Node*, StringView, const NSRange&);
RetainPtr<NSAttributedString> attributedStringCreate(Node*, StringView, AXCoreObject::SpellCheck);
}

@interface WebAccessibilityObjectWrapperBase : NSObject {
    CyberCore::AccessibilityObject* m_axObject;

#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
    CyberCore::AXIsolatedObject* m_isolatedObject;
    // To be accessed only on the main thread.
    bool m_isolatedObjectInitialized;
#endif

    CyberCore::AXID _identifier;
}

- (id)initWithAccessibilityObject:(CyberCore::AccessibilityObject*)axObject;
#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
- (void)attachIsolatedObject:(CyberCore::AXIsolatedObject*)isolatedObject;
- (BOOL)hasIsolatedObject;
#endif

- (void)detach;
#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
- (void)detachIsolatedObject:(CyberCore::AccessibilityDetachmentType)detachmentType;
#endif

@property (nonatomic, assign) CyberCore::AXID identifier;

// FIXME: unified these two methods into one.
#if PLATFORM(MAC)
// Updates the underlying object and accessibility hierarchy , and returns the
// corresponding AXCoreObject.
- (CyberCore::AXCoreObject*)updateObjectBackingStore;
#else
- (BOOL)_prepareAccessibilityCall;
#endif

// This can be either an AccessibilityObject or an AXIsolatedObject
- (CyberCore::AXCoreObject*)axBackingObject;

- (NSArray<NSDictionary *> *)lineRectsAndText;

// These are pre-fixed with base so that AppKit does not end up calling into these directly (bypassing safety checks).
- (NSString *)baseAccessibilityHelpText;
- (NSArray<NSString *> *)baseAccessibilitySpeechHint;

- (NSString *)ariaLandmarkRoleDescription;

- (id)attachmentView;
// Used to inform an element when a notification is posted for it. Used by tests.
- (void)accessibilityPostedNotification:(NSString *)notificationName;
- (void)accessibilityPostedNotification:(NSString *)notificationName userInfo:(NSDictionary *)userInfo;

- (CGPathRef)convertPathToScreenSpace:(const CyberCore::Path&)path;

- (CGRect)convertRectToSpace:(const CyberCore::FloatRect&)rect space:(CyberCore::AccessibilityConversionSpace)space;

// Math related functions
- (NSArray *)accessibilityMathPostscriptPairs;
- (NSArray *)accessibilityMathPrescriptPairs;

- (NSRange)accessibilityVisibleCharacterRange;

- (NSDictionary<NSString *, id> *)baseAccessibilityResolvedEditingStyles;

extern CyberCore::AccessibilitySearchCriteria accessibilitySearchCriteriaForSearchPredicateParameterizedAttribute(const NSDictionary *);

extern NSArray *makeNSArray(const CyberCore::AXCoreObject::AccessibilityChildrenVector&);
extern NSRange makeNSRange(std::optional<CyberCore::SimpleRange>);
extern std::optional<CyberCore::SimpleRange> makeDOMRange(CyberCore::Document*, NSRange);

#if PLATFORM(IOS_FAMILY)
- (id)_accessibilityWebDocumentView;
#endif

@end
