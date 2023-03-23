/*
 * Copyright (C) 2021-2022 Apple Inc. All rights reserved.
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

#if ENABLE(GPU_PROCESS)

#include "Decoder.h"
#include "DisplayListRecorderFlushIdentifier.h"
#include "QualifiedRenderingResourceIdentifier.h"
#include "RemoteRenderingBackend.h"
#include "SharedVideoFrame.h"
#include "StreamMessageReceiver.h"
#include "StreamServerConnection.h"
#include <CyberCore/ControlFactory.h>
#include <CyberCore/DisplayListItems.h>
#include <CyberCore/ProcessIdentifier.h>
#include <wtf/RefCounted.h>
#include <wtf/WeakPtr.h>

namespace WebKit {

class RemoteRenderingBackend;
class RemoteResourceCache;

class RemoteDisplayListRecorder : public IPC::StreamMessageReceiver, public CanMakeWeakPtr<RemoteDisplayListRecorder> {
public:
    static Ref<RemoteDisplayListRecorder> create(CyberCore::ImageBuffer& imageBuffer, QualifiedRenderingResourceIdentifier imageBufferIdentifier, CyberCore::ProcessIdentifier webProcessIdentifier, RemoteRenderingBackend& renderingBackend)
    {
        auto instance = adoptRef(*new RemoteDisplayListRecorder(imageBuffer, imageBufferIdentifier, webProcessIdentifier, renderingBackend));
        instance->startListeningForIPC();
        return instance;
    }

    void stopListeningForIPC();
    void clearImageBufferReference();

    void save();
    void restore();
    void translate(float x, float y);
    void rotate(float angle);
    void scale(const CyberCore::FloatSize& scale);
    void setCTM(const CyberCore::AffineTransform&);
    void concatenateCTM(const CyberCore::AffineTransform&);
    void setInlineFillColor(CyberCore::DisplayList::SetInlineFillColor&&);
    void setInlineStrokeColor(CyberCore::DisplayList::SetInlineStrokeColor&&);
    void setStrokeThickness(float);
    void setState(CyberCore::DisplayList::SetState&&);
    void setLineCap(CyberCore::LineCap);
    void setLineDash(CyberCore::DisplayList::SetLineDash&&);
    void setLineJoin(CyberCore::LineJoin);
    void setMiterLimit(float);
    void clearShadow();
    void clip(const CyberCore::FloatRect&);
    void clipOut(const CyberCore::FloatRect&);
    void clipToImageBuffer(CyberCore::RenderingResourceIdentifier, const CyberCore::FloatRect& destinationRect);
    void clipOutToPath(const CyberCore::Path&);
    void clipPath(const CyberCore::Path&, CyberCore::WindRule);
    void drawGlyphs(CyberCore::DisplayList::DrawGlyphs&&);
    void drawDecomposedGlyphs(CyberCore::RenderingResourceIdentifier fontIdentifier, CyberCore::RenderingResourceIdentifier decomposedGlyphsIdentifier);
    void drawFilteredImageBuffer(std::optional<CyberCore::RenderingResourceIdentifier> sourceImageIdentifier, const CyberCore::FloatRect& sourceImageRect, Ref<CyberCore::Filter>);
    void drawImageBuffer(CyberCore::RenderingResourceIdentifier imageBufferIdentifier, const CyberCore::FloatRect& destinationRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&);
    void drawNativeImage(CyberCore::RenderingResourceIdentifier imageIdentifier, const CyberCore::FloatSize& imageSize, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&);
    void drawSystemImage(Ref<CyberCore::SystemImage>, const CyberCore::FloatRect&);
    void drawPattern(CyberCore::RenderingResourceIdentifier imageIdentifier, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& tileRect, const CyberCore::AffineTransform&, const CyberCore::FloatPoint&, const CyberCore::FloatSize& spacing, const CyberCore::ImagePaintingOptions&);
    void beginTransparencyLayer(float opacity);
    void endTransparencyLayer();
    void drawRect(const CyberCore::FloatRect&, float borderThickness);
    void drawLine(const CyberCore::FloatPoint& point1, const CyberCore::FloatPoint& point2);
    void drawLinesForText(CyberCore::DisplayList::DrawLinesForText&&);
    void drawDotsForDocumentMarker(const CyberCore::FloatRect&, const CyberCore::DocumentMarkerLineStyle&);
    void drawEllipse(const CyberCore::FloatRect&);
    void drawPath(const CyberCore::Path&);
    void drawFocusRingPath(const CyberCore::Path&, float outlineWidth, const CyberCore::Color&);
    void drawFocusRingRects(const Vector<CyberCore::FloatRect>&, float outlineOffset, float outlineWidth, const CyberCore::Color&);
    void fillRect(const CyberCore::FloatRect&);
    void fillRectWithColor(const CyberCore::FloatRect&, const CyberCore::Color&);
    void fillRectWithGradient(CyberCore::DisplayList::FillRectWithGradient&&);
    void fillCompositedRect(const CyberCore::FloatRect&, const CyberCore::Color&, CyberCore::CompositeOperator, CyberCore::BlendMode);
    void fillRoundedRect(const CyberCore::FloatRoundedRect&, const CyberCore::Color&, CyberCore::BlendMode);
    void fillRectWithRoundedHole(const CyberCore::FloatRect&, const CyberCore::FloatRoundedRect&, const CyberCore::Color&);
#if ENABLE(INLINE_PATH_DATA)
    void fillLine(const CyberCore::LineData&);
    void fillArc(const CyberCore::ArcData&);
    void fillQuadCurve(const CyberCore::QuadCurveData&);
    void fillBezierCurve(const CyberCore::BezierCurveData&);
#endif
    void fillPath(const CyberCore::Path&);
    void fillEllipse(const CyberCore::FloatRect&);
    void convertToLuminanceMask();
    void transformToColorSpace(const CyberCore::DestinationColorSpace&);
#if ENABLE(VIDEO)
    void paintFrameForMedia(CyberCore::MediaPlayerIdentifier, const CyberCore::FloatRect& destination);
#endif
    void strokeRect(const CyberCore::FloatRect&, float lineWidth);
#if ENABLE(INLINE_PATH_DATA)
    void strokeLine(const CyberCore::LineData&);
    void strokeLineWithColorAndThickness(CyberCore::DisplayList::SetInlineStrokeColor&&, float, const CyberCore::LineData&);
    void strokeArc(const CyberCore::ArcData&);
    void strokeQuadCurve(const CyberCore::QuadCurveData&);
    void strokeBezierCurve(const CyberCore::BezierCurveData&);
#endif
    void strokePath(const CyberCore::Path&);
    void strokeEllipse(const CyberCore::FloatRect&);
    void clearRect(const CyberCore::FloatRect&);
    void drawControlPart(Ref<CyberCore::ControlPart>, const CyberCore::FloatRoundedRect& borderRect, float deviceScaleFactor, const CyberCore::ControlStyle&);
#if USE(CG)
    void applyStrokePattern();
    void applyFillPattern();
#endif
    void applyDeviceScaleFactor(float);
    void flushContext(DisplayListRecorderFlushIdentifier);

private:
    RemoteDisplayListRecorder(CyberCore::ImageBuffer&, QualifiedRenderingResourceIdentifier, CyberCore::ProcessIdentifier webProcessIdentifier, RemoteRenderingBackend&);

    void setStateWithQualifiedIdentifiers(CyberCore::DisplayList::SetState&&, QualifiedRenderingResourceIdentifier strokePatternImageIdentifier, QualifiedRenderingResourceIdentifier fillPatternImageIdentifier);
    void clipToImageBufferWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier, const CyberCore::FloatRect& destinationRect);
    void drawGlyphsWithQualifiedIdentifier(CyberCore::DisplayList::DrawGlyphs&&, QualifiedRenderingResourceIdentifier fontIdentifier);
    void drawDecomposedGlyphsWithQualifiedIdentifiers(QualifiedRenderingResourceIdentifier fontIdentifier, QualifiedRenderingResourceIdentifier decomposedGlyphsIdentifier);
    void drawImageBufferWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier imageBufferIdentifier, const CyberCore::FloatRect& destinationRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&);
    void drawNativeImageWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier imageIdentifier, const CyberCore::FloatSize& imageSize, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&);
    void drawPatternWithQualifiedIdentifier(QualifiedRenderingResourceIdentifier imageIdentifier, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& tileRect, const CyberCore::AffineTransform&, const CyberCore::FloatPoint&, const CyberCore::FloatSize& spacing, const CyberCore::ImagePaintingOptions&);

    RemoteResourceCache& resourceCache();
    CyberCore::GraphicsContext& drawingContext();

    template<typename T, typename ... AdditionalArgs>
    void handleItem(T&& item, AdditionalArgs&&... args)
    {
        // FIXME: In the future, we should consider buffering up batches of display list items before
        // applying them instead of applying them immediately, so that we can apply clipping and occlusion
        // optimizations to skip over parts of a display list, if possible.
        item.apply(drawingContext(), std::forward<AdditionalArgs>(args)...);
    }

    void startListeningForIPC();
    void didReceiveStreamMessage(IPC::StreamServerConnection&, IPC::Decoder&) final;

#if PLATFORM(COCOA) && ENABLE(VIDEO)
    void paintVideoFrame(SharedVideoFrame&&, const CyberCore::FloatRect&, bool shouldDiscardAlpha);
    void setSharedVideoFrameSemaphore(IPC::Semaphore&&);
    void setSharedVideoFrameMemory(const SharedMemory::Handle&);
#endif

    WeakPtr<CyberCore::ImageBuffer> m_imageBuffer;
    QualifiedRenderingResourceIdentifier m_imageBufferIdentifier;
    CyberCore::ProcessIdentifier m_webProcessIdentifier;
    RefPtr<RemoteRenderingBackend> m_renderingBackend;
    std::unique_ptr<CyberCore::ControlFactory> m_controlFactory;
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    SharedVideoFrameReader m_sharedVideoFrameReader;
#endif
};

} // namespace WebKit

#endif // ENABLE(GPU_PROCESS)
