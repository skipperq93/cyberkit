/*
 * Copyright (C) 2007-2017 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE, INC. ``AS IS'' AND ANY
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
 *
 */

#if PLATFORM(IOS_FAMILY)

#import "DOMUIKitExtensions.h"

#import "DOM.h"
#import "DOMCore.h"
#import "DOMExtensions.h"
#import "DOMHTML.h"
#import "DOMHTMLAreaElementInternal.h"
#import "DOMHTMLElementInternal.h"
#import "DOMHTMLImageElementInternal.h"
#import "DOMHTMLSelectElementInternal.h"
#import "DOMInternal.h"
#import "DOMNodeInternal.h"
#import "DOMRangeInternal.h"
#import <CyberCore/CachedImage.h>
#import <CyberCore/Editing.h>
#import <CyberCore/FloatPoint.h>
#import <CyberCore/FontCascade.h>
#import <CyberCore/FrameSelection.h>
#import <CyberCore/HTMLAreaElement.h>
#import <CyberCore/HTMLImageElement.h>
#import <CyberCore/HTMLInputElement.h>
#import <CyberCore/HTMLSelectElement.h>
#import <CyberCore/HTMLTextAreaElement.h>
#import <CyberCore/Image.h>
#import <CyberCore/InlineBox.h>
#import <CyberCore/Node.h>
#import <CyberCore/Range.h>
#import <CyberCore/RenderBlock.h>
#import <CyberCore/RenderBlockFlow.h>
#import <CyberCore/RenderBox.h>
#import <CyberCore/RenderObject.h>
#import <CyberCore/RenderStyleConstants.h>
#import <CyberCore/RenderText.h>
#import <CyberCore/RoundedRect.h>
#import <CyberCore/SharedBuffer.h>
#import <CyberCore/SimpleRange.h>
#import <CyberCore/VisiblePosition.h>
#import <CyberCore/VisibleUnits.h>
#import <CyberCore/WAKAppKitStubs.h>

using CyberCore::Node;
using CyberCore::Position;
using CyberCore::Range;
using CyberCore::RenderBlock;
using CyberCore::RenderBox;
using CyberCore::RenderObject;
using CyberCore::RenderText;
using CyberCore::VisiblePosition;

@implementation DOMRange (UIKitExtensions)

- (void)move:(UInt32)amount inDirection:(WebTextAdjustmentDirection)direction
{
    auto& range = *core(self);

    CyberCore::FrameSelection frameSelection;
    frameSelection.setSelection(makeSimpleRange(range));
    
    CyberCore::TextGranularity granularity = CyberCore::TextGranularity::CharacterGranularity;
    // Until CyberKit supports vertical layout, "down" is equivalent to "forward by a line" and
    // "up" is equivalent to "backward by a line".
    if (direction == WebTextAdjustmentDown) {
        direction = WebTextAdjustmentForward;
        granularity = CyberCore::TextGranularity::LineGranularity;
    } else if (direction == WebTextAdjustmentUp) {
        direction = WebTextAdjustmentBackward;
        granularity = CyberCore::TextGranularity::LineGranularity;
    }

    for (UInt32 i = 0; i < amount; i++)
        frameSelection.modify(CyberCore::FrameSelection::AlterationMove, (CyberCore::SelectionDirection)direction, granularity);

    Position start = frameSelection.selection().start().parentAnchoredEquivalent();
    Position end = frameSelection.selection().end().parentAnchoredEquivalent();
    if (start.containerNode())
        range.setStart(*start.containerNode(), start.offsetInContainerNode());
    if (end.containerNode())
        range.setEnd(*end.containerNode(), end.offsetInContainerNode());
}

- (void)extend:(UInt32)amount inDirection:(WebTextAdjustmentDirection)direction
{
    auto& range = *core(self);

    CyberCore::FrameSelection frameSelection;
    frameSelection.setSelection(makeSimpleRange(range));

    for (UInt32 i = 0; i < amount; i++)
        frameSelection.modify(CyberCore::FrameSelection::AlterationExtend, (CyberCore::SelectionDirection)direction, CyberCore::TextGranularity::CharacterGranularity);

    Position start = frameSelection.selection().start().parentAnchoredEquivalent();
    Position end = frameSelection.selection().end().parentAnchoredEquivalent();
    if (start.containerNode())
        range.setStart(*start.containerNode(), start.offsetInContainerNode());
    if (end.containerNode())
        range.setEnd(*end.containerNode(), end.offsetInContainerNode());
}

- (DOMNode *)firstNode
{
    return kit(core(self)->firstNode());
}

@end

//-------------------

@implementation DOMNode (UIKitExtensions)

// NOTE: Code blatantly copied from [WebInspector _hightNode:] in CyberKit/WebInspector/WebInspector.m@19861
- (NSArray *)boundingBoxes
{
    NSArray *rects = nil;
    NSRect bounds = [self boundingBox];
    if (!NSIsEmptyRect(bounds)) {
        if ([self isKindOfClass:[DOMElement class]]) {
            DOMDocument *document = [self ownerDocument];
            DOMCSSStyleDeclaration *style = [document getComputedStyle:(DOMElement *)self pseudoElement:@""];
            if ([[style getPropertyValue:@"display"] isEqualToString:@"inline"])
                rects = [self lineBoxRects];
        } else if ([self isKindOfClass:[DOMText class]])
            rects = [self lineBoxRects];
    }

    if (![rects count])
        rects = @[[NSValue valueWithRect:bounds]];

    return rects;
}

- (NSArray *)absoluteQuads
{
    NSArray *quads = nil;
    NSRect bounds = [self boundingBox];
    if (!NSIsEmptyRect(bounds)) {
        if ([self isKindOfClass:[DOMElement class]]) {
            DOMDocument *document = [self ownerDocument];
            DOMCSSStyleDeclaration *style = [document getComputedStyle:(DOMElement *)self pseudoElement:@""];
            if ([[style getPropertyValue:@"display"] isEqualToString:@"inline"])
                quads = [self lineBoxQuads];
        } else if ([self isKindOfClass:[DOMText class]])
            quads = [self lineBoxQuads];
    }

    if (![quads count]) {
        WKQuadObject* quadObject = [[WKQuadObject alloc] initWithQuad:[self absoluteQuad]];
        quads = @[quadObject];
        [quadObject release];
    }

    return quads;
}

- (NSArray *)borderRadii
{
    RenderObject* renderer = core(self)->renderer();
    
    if (is<RenderBox>(renderer)) {
        CyberCore::RoundedRect::Radii radii = downcast<RenderBox>(*renderer).borderRadii();
        return @[[NSValue valueWithSize:(CyberCore::FloatSize)radii.topLeft()],
            [NSValue valueWithSize:(CyberCore::FloatSize)radii.topRight()],
            [NSValue valueWithSize:(CyberCore::FloatSize)radii.bottomLeft()],
            [NSValue valueWithSize:(CyberCore::FloatSize)radii.bottomRight()]];
    }
    NSValue *emptyValue = [NSValue valueWithSize:CGSizeZero];
    return @[emptyValue, emptyValue, emptyValue, emptyValue];
}

- (BOOL)containsOnlyInlineObjects
{
    RenderObject* renderer = core(self)->renderer();
    return renderer
        && renderer->childrenInline()
        && (is<RenderBlock>(*renderer) && !downcast<RenderBlock>(*renderer).inlineContinuation())
        && !renderer->isTable();
}

- (BOOL)isSelectableBlock
{
    RenderObject* renderer = core(self)->renderer();
    return renderer && (is<CyberCore::RenderBlockFlow>(*renderer) || (is<RenderBlock>(*renderer) && downcast<RenderBlock>(*renderer).inlineContinuation() != nil));
}

- (DOMRange *)rangeOfContainingParagraph
{
    VisiblePosition position(createLegacyEditingPosition(core(self), 0), CyberCore::DOWNSTREAM);
    return kit(makeSimpleRange(startOfParagraph(position), endOfParagraph(position)));
}

- (CGFloat)textHeight
{  
    RenderObject* renderer = core(self)->renderer();
    if (is<RenderText>(renderer))
        return downcast<RenderText>(*renderer).style().computedLineHeight();
    
    return CGFLOAT_MAX;
}

- (DOMNode *)findExplodedTextNodeAtPoint:(CGPoint)point
{
    auto* renderer = core(self)->renderer();
    if (!is<CyberCore::RenderBlockFlow>(renderer))
        return nil;

    auto* renderText = downcast<CyberCore::RenderBlockFlow>(*renderer).findClosestTextAtAbsolutePoint(point);
    if (renderText && renderText->textNode())
        return kit(renderText->textNode());

    return nil;
}

@end

//-----------------

@implementation DOMElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return 0; }
@end

@implementation DOMHTMLElement (DOMUIKitComplexityExtensions) 

- (int)structuralComplexityContribution
{
    int result = 0;
    RenderObject * renderer = core(self)->renderer();
    if (renderer) {
        if (renderer->isFloatingOrOutOfFlowPositioned() ||
            renderer->isWidget()) {
            result = INT_MAX;
        } else if (!renderer->firstChildSlow()) {
            result = 0;
        } else if (is<CyberCore::RenderBlockFlow>(*renderer) || (is<RenderBlock>(*renderer) && downcast<RenderBlock>(*renderer).inlineContinuation())) {
            BOOL noCost = NO;
            if (is<RenderBox>(*renderer)) {
                RenderBox& asBox = renderer->enclosingBox();
                RenderObject* parent = asBox.parent();
                RenderBox* parentRenderBox = is<RenderBox>(parent) ? downcast<RenderBox>(parent) : nullptr;
                if (parentRenderBox && asBox.width() == parentRenderBox->width()) {
                    noCost = YES;
                }
            }
            result = (noCost ? 0 : 1);
        } else if (renderer->hasTransform()) {
            result = INT_MAX;
        }
    }
    return result;
}

@end

@implementation DOMHTMLBodyElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return 0; }
@end

// Maximally complex elements

@implementation DOMHTMLFormElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLTableElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLFrameElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLIFrameElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLButtonElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLTextAreaElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLInputElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

@implementation DOMHTMLSelectElement (DOMUIKitComplexityExtensions)
- (int)structuralComplexityContribution { return INT_MAX; }
@end

//-----------------
        
@implementation DOMHTMLAreaElement (DOMUIKitExtensions)

- (CGRect)boundingBoxWithOwner:(DOMNode *)owner
{
    // ignores transforms
    return owner ? snappedIntRect(core(self)->computeRect(core(owner)->renderer())) : CGRectZero;
}

- (WKQuad)absoluteQuadWithOwner:(DOMNode *)owner
{
    if (owner) {
        CyberCore::IntRect rect = snappedIntRect(core(self)->computeRect(core(owner)->renderer()));
        WKQuad quad;
        quad.p1 = CGPointMake(rect.x(), rect.y());
        quad.p2 = CGPointMake(rect.maxX(), rect.y());
        quad.p3 = CGPointMake(rect.maxX(), rect.maxY());
        quad.p4 = CGPointMake(rect.x(), rect.maxY());
        return quad;
    }

    WKQuad zeroQuad = { CGPointZero, CGPointZero, CGPointZero, CGPointZero };
    return zeroQuad;
}

- (NSArray *)boundingBoxesWithOwner:(DOMNode *)owner
{
    return @[[NSValue valueWithRect:[self boundingBoxWithOwner:owner]]];
}

- (NSArray *)absoluteQuadsWithOwner:(DOMNode *)owner
{
    WKQuadObject *quadObject = [[WKQuadObject alloc] initWithQuad:[self absoluteQuadWithOwner:owner]];
    NSArray *quadArray = @[quadObject];
    [quadObject release];
    return quadArray;
}

@end

@implementation DOMHTMLSelectElement (DOMUIKitExtensions)

- (unsigned)completeLength
{
    return core(self)->listItems().size();
}

- (DOMNode *)listItemAtIndex:(int)anIndex
{
    return kit(core(self)->listItems()[anIndex]);
}

@end

@implementation DOMHTMLImageElement (WebDOMHTMLImageElementOperationsPrivate)

- (NSData *)dataRepresentation:(BOOL)rawImageData
{
    auto* cachedImage = core(self)->cachedImage();
    if (!cachedImage)
        return nil;
    auto* image = cachedImage->image();
    if (!image)
        return nil;
    auto* data = rawImageData ? cachedImage->resourceBuffer() : image->data();
    if (!data)
        return nil;
    return data->createNSData().autorelease();
}

- (NSString *)mimeType
{
    auto* cachedImage = core(self)->cachedImage();
    if (!cachedImage || !cachedImage->image())
        return nil;
    return cachedImage->response().mimeType();
}

@end

#endif
