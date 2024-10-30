/*
 * Copyright (C) 2020 Apple Inc. All rights reserved.
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

#import "Color.h"
#import "TextAttachmentForSerialization.h"
#import <wtf/ObjectIdentifier.h>
#import <wtf/RetainPtr.h>
#import <wtf/URL.h>
#import <wtf/Vector.h>

#if PLATFORM(MAC)
#define PlatformColor                   NSColor
#define PlatformColorClass              NSColor.class
#define PlatformFont                    NSFont
#define PlatformFontClass               NSFont.class
#define PlatformImageClass              NSImage
#define PlatformNSColorClass            NSColor
#define PlatformNSParagraphStyle        NSParagraphStyle.class
#define PlatformNSPresentationIntent    NSPresentationIntent.class
#define PlatformNSShadow                NSShadow.class
#define PlatformNSTextAttachment        NSTextAttachment.class
#define PlatformNSTextList              NSTextList
#define PlatformNSTextTab               NSTextTab
#define PlatformNSTextTable             NSTextTable
#define PlatformNSTextTableBlock        NSTextTableBlock.class
#else
#define PlatformColor                   UIColor
#define PlatformColorClass              PAL::getUIColorClass()
#define PlatformFont                    UIFont
#define PlatformFontClass               PAL::getUIFontClass()
#define PlatformImageClass              PAL::getUIImageClass()
#define PlatformNSColorClass            getNSColorClass()
#define PlatformNSParagraphStyle        PAL::getNSParagraphStyleClass()
#if !HAVE(NS_PRESENTATION_INTENT)
#define NSPresentationIntent            WebCorePresentationIntent
#define NSPresentationIntentKind        WebCorePresentationIntentKind
#define NSPresentationIntentKindParagraph WebCorePresentationIntentKindParagraph
#define NSPresentationIntentKindHeader  WebCorePresentationIntentKindHeader
#define NSPresentationIntentKindOrderedList WebCorePresentationIntentKindOrderedList
#define NSPresentationIntentKindUnorderedList WebCorePresentationIntentKindUnorderedList
#define NSPresentationIntentKindListItem WebCorePresentationIntentKindListItem
#define NSPresentationIntentKindCodeBlock WebCorePresentationIntentKindCodeBlock
#define NSPresentationIntentKindBlockQuote WebCorePresentationIntentKindBlockQuote
#define NSPresentationIntentKindThematicBreak WebCorePresentationIntentKindThematicBreak
#define NSPresentationIntentKindTable WebCorePresentationIntentKindTable
#define NSPresentationIntentKindTableHeaderRow WebCorePresentationIntentKindTableHeaderRow
#define NSPresentationIntentKindTableRow WebCorePresentationIntentKindTableRow
#define NSPresentationIntentKindTableCell WebCorePresentationIntentKindTableCell
#endif
#define PlatformNSPresentationIntent    NSPresentationIntent.class
#define PlatformNSShadow                PAL::getNSShadowClass()
#define PlatformNSTextAttachment        getNSTextAttachmentClass()
#define PlatformNSTextList              getNSTextListClass()
#define PlatformNSTextTab               getNSTextTabClass()
#define PlatformNSTextTable             getNSTextTableClass()
#define PlatformNSTextTableBlock        getNSTextTableBlockClass()
#endif

OBJC_CLASS NSAttributedString;
OBJC_CLASS NSDate;
OBJC_CLASS NSDictionary;
OBJC_CLASS NSParagraphStyle;
OBJC_CLASS NSShadow;
OBJC_CLASS NSTextAttachment;
OBJC_CLASS PlatformColor;

#if HAVE(NS_PRESENTATION_INTENT) || !defined(__OBJC__)
OBJC_CLASS NSPresentationIntent;
#else
typedef NS_ENUM(NSInteger, NSPresentationIntentKind) {
    NSPresentationIntentKindParagraph,
    NSPresentationIntentKindHeader,
    NSPresentationIntentKindOrderedList,
    NSPresentationIntentKindUnorderedList,
    NSPresentationIntentKindListItem,
    NSPresentationIntentKindCodeBlock,
    NSPresentationIntentKindBlockQuote,
    NSPresentationIntentKindThematicBreak,
    NSPresentationIntentKindTable,
    NSPresentationIntentKindTableHeaderRow,
    NSPresentationIntentKindTableRow,
    NSPresentationIntentKindTableCell,
};

NS_ASSUME_NONNULL_BEGIN
WEBCORE_EXPORT @interface NSPresentationIntent : NSObject <NSCopying, NSCoding, NSSecureCoding>
@property(class, readonly) BOOL supportsSecureCoding;
@property (readonly) NSPresentationIntentKind intentKind;
@property(readonly, nullable, strong) NSPresentationIntent *parentIntent;

/// An integer value which uniquely identifies this intent in the document. Identity disambiguates attributes which apply to contiguous text -- for example, two headers in a row with the same level. It can also be used to track the location in an attributed string of a particular part of a document, even after mutation.
@property (readonly) NSInteger identity;

/// If the intent is not a list, this value is 0.
@property (readonly) NSInteger ordinal;

/// If the intent is not a table, this value is `nil`.
@property (nullable, readonly) NSArray<NSNumber *> *columnAlignments;

/// If the intent is not a table, this value is 0.
@property (readonly) NSInteger columnCount;

/// If the intent is not a header, this value is 0.
@property (readonly) NSInteger headerLevel;

/// If the intent is not a code block, this value is `nil`.
@property (readonly, nullable, copy) NSString *languageHint;

/// The column to which this cell belongs (0-based). If the intent is not a cell, this value is 0.
@property (readonly) NSInteger column;

/// The row to which this cell belongs (0-based). If the intent is not a row, this value is 0. Header rows are always row 0. If the table has more rows, those start at row 1.
@property (readonly) NSInteger row;

/// The indentation level of this intent. Each nested list increases the indentation level by one; all elements within the same list (and not then nested into a child list intent) have the same indentation level.
/// Text outside list intents has an indentation level of 0.
@property (readonly) NSInteger indentationLevel;

+ (NSPresentationIntent *)paragraphIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)headerIntentWithIdentity:(NSInteger)identity level:(NSInteger)level nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)codeBlockIntentWithIdentity:(NSInteger)identity languageHint:(nullable NSString *)languageHint nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)thematicBreakIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)orderedListIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)unorderedListIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)listItemIntentWithIdentity:(NSInteger)identity ordinal:(NSInteger)ordinal nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)blockQuoteIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableIntentWithIdentity:(NSInteger)identity columnCount:(NSInteger)columnCount alignments:(NSArray<NSNumber *> *)alignments nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableHeaderRowIntentWithIdentity:(NSInteger)identity nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableRowIntentWithIdentity:(NSInteger)identity row:(NSInteger)row nestedInsideIntent:(nullable NSPresentationIntent *)parent;
+ (NSPresentationIntent *)tableCellIntentWithIdentity:(NSInteger)identity column:(NSInteger)column nestedInsideIntent:(nullable NSPresentationIntent *)parent;

- (NSPresentationIntent *) initWithIntentKind:(NSPresentationIntentKind) kind
                                 parentIntent:(nullable NSPresentationIntent *) parent
                                     identity:(NSInteger) identity
                                      ordinal:(NSInteger) ordinal
                             columnAlignments:(nullable NSArray<NSNumber *> *) alignments
                                  columnCount:(NSInteger) columnCount
                                  headerLevel:(NSInteger) headerLevel
                                 languageHint:(nullable NSString *) languageHint
                                       column:(NSInteger) column
                                          row:(NSInteger) row
                             indentationLevel:(NSInteger) indentationLevel;
@end
NS_ASSUME_NONNULL_END
#endif

namespace WebCore {

class Font;

struct AttributedStringTextTableIDType;
using AttributedStringTextTableID = ObjectIdentifier<AttributedStringTextTableIDType>;

struct AttributedStringTextTableBlockIDType;
using AttributedStringTextTableBlockID = ObjectIdentifier<AttributedStringTextTableBlockIDType>;

struct AttributedStringTextListIDType;
using AttributedStringTextListID = ObjectIdentifier<AttributedStringTextListIDType>;

struct WEBCORE_EXPORT AttributedString {
    struct Range {
        size_t location { 0 };
        size_t length { 0 };
    };

    using TextTableID = AttributedStringTextTableID;
    using TextTableBlockID = AttributedStringTextTableBlockID;
    using TextListID = AttributedStringTextListID;
    using TableBlockAndTableIDPair = std::pair<TextTableBlockID, TextTableID>;

    struct ParagraphStyleWithTableAndListIDs {
        RetainPtr<NSParagraphStyle> style;
        Vector<std::optional<TableBlockAndTableIDPair>> tableBlockAndTableIDs; // Same length as `-textBlocks`.
        Vector<TextListID> listIDs; // Same length as `-textLists`.
    };

    struct ColorFromCGColor {
        Color color;
    };

    struct ColorFromPlatformColor {
        Color color;
    };

    struct AttributeValue {
        std::variant<
            double,
            String,
            URL,
            Ref<Font>,
            Vector<String>,
            Vector<double>,
            ParagraphStyleWithTableAndListIDs,
            RetainPtr<NSPresentationIntent>,
            RetainPtr<NSShadow>,
            RetainPtr<NSDate>,
            ColorFromCGColor,
            ColorFromPlatformColor,
#if ENABLE(MULTI_REPRESENTATION_HEIC)
            MultiRepresentationHEICAttachmentData,
#endif
            TextAttachmentFileWrapper,
            TextAttachmentMissingImage
        > value;
    };

    String string;
    Vector<std::pair<Range, HashMap<String, AttributeValue>>> attributes;
    std::optional<HashMap<String, AttributeValue>> documentAttributes;

    AttributedString();
    AttributedString(String&&, Vector<std::pair<Range, HashMap<String, AttributeValue>>>&&, std::optional<HashMap<String, AttributeValue>>&&);
    AttributedString(AttributedString&&);
    AttributedString(const AttributedString&);
    AttributedString& operator=(AttributedString&&);
    AttributedString& operator=(const AttributedString&);
    ~AttributedString();

    static AttributedString fromNSAttributedStringAndDocumentAttributes(RetainPtr<NSAttributedString>&&, RetainPtr<NSDictionary>&& documentAttributes);
    static AttributedString fromNSAttributedString(RetainPtr<NSAttributedString>&&);
    static bool rangesAreSafe(const String&, const Vector<std::pair<Range, HashMap<String, AttributeValue>>>&);
    RetainPtr<NSDictionary> documentAttributesAsNSDictionary() const;
    RetainPtr<NSAttributedString> nsAttributedString() const;
    bool isNull() const;
};

} // namespace WebCore
