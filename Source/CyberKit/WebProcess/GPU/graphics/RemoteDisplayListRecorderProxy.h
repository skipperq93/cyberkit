/*
 * Copyright (C) 2021-2023 Apple Inc. All rights reserved.
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

#include "DisplayListRecorderFlushIdentifier.h"
#include "SharedVideoFrame.h"
#include <CyberCore/DisplayListRecorder.h>
#include <CyberCore/DrawGlyphsRecorder.h>
#include <CyberCore/GraphicsContext.h>
#include <wtf/WeakPtr.h>

namespace IPC {
class StreamClientConnection;
}

namespace CyberKit {

class RemoteRenderingBackendProxy;
class RemoteImageBufferProxy;

class RemoteDisplayListRecorderProxy : public CyberCore::DisplayList::Recorder {
public:
    RemoteDisplayListRecorderProxy(RemoteImageBufferProxy&, RemoteRenderingBackendProxy&, const CyberCore::FloatRect& initialClip, const CyberCore::AffineTransform&);
    ~RemoteDisplayListRecorderProxy() = default;

    void convertToLuminanceMask() final;
    void transformToColorSpace(const CyberCore::DestinationColorSpace&) final;
    void flushContext(DisplayListRecorderFlushIdentifier);
    void disconnect();

private:
    template<typename T> void send(T&& message);

    friend class CyberCore::DrawGlyphsRecorder;

    CyberCore::RenderingMode renderingMode() const final;

    void recordSave() final;
    void recordRestore() final;
    void recordTranslate(float x, float y) final;
    void recordRotate(float angle) final;
    void recordScale(const CyberCore::FloatSize&) final;
    void recordSetCTM(const CyberCore::AffineTransform&) final;
    void recordConcatenateCTM(const CyberCore::AffineTransform&) final;
    void recordSetInlineFillColor(CyberCore::SRGBA<uint8_t>) final;
    void recordSetInlineStrokeColor(CyberCore::SRGBA<uint8_t>) final;
    void recordSetStrokeThickness(float) final;
    void recordSetState(const CyberCore::GraphicsContextState&) final;
    void recordSetLineCap(CyberCore::LineCap) final;
    void recordSetLineDash(const CyberCore::DashArray&, float dashOffset) final;
    void recordSetLineJoin(CyberCore::LineJoin) final;
    void recordSetMiterLimit(float) final;
    void recordClearShadow() final;
    void recordClip(const CyberCore::FloatRect&) final;
    void recordClipOut(const CyberCore::FloatRect&) final;
    void recordClipToImageBuffer(CyberCore::ImageBuffer&, const CyberCore::FloatRect& destinationRect) final;
    void recordClipOutToPath(const CyberCore::Path&) final;
    void recordClipPath(const CyberCore::Path&, CyberCore::WindRule) final;
    void recordDrawFilteredImageBuffer(CyberCore::ImageBuffer*, const CyberCore::FloatRect& sourceImageRect, CyberCore::Filter&) final;
    void recordDrawGlyphs(const CyberCore::Font&, const CyberCore::GlyphBufferGlyph*, const CyberCore::GlyphBufferAdvance*, unsigned count, const CyberCore::FloatPoint& localAnchor, CyberCore::FontSmoothingMode) final;
    void recordDrawDecomposedGlyphs(const CyberCore::Font&, const CyberCore::DecomposedGlyphs&) final;
    void recordDrawImageBuffer(CyberCore::ImageBuffer&, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&) final;
    void recordDrawNativeImage(CyberCore::RenderingResourceIdentifier imageIdentifier, const CyberCore::FloatSize& imageSize, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& srcRect, const CyberCore::ImagePaintingOptions&) final;
    void recordDrawSystemImage(CyberCore::SystemImage&, const CyberCore::FloatRect&);
    void recordDrawPattern(CyberCore::RenderingResourceIdentifier, const CyberCore::FloatRect& destRect, const CyberCore::FloatRect& tileRect, const CyberCore::AffineTransform&, const CyberCore::FloatPoint& phase, const CyberCore::FloatSize& spacing, const CyberCore::ImagePaintingOptions& = { }) final;
    void recordBeginTransparencyLayer(float) final;
    void recordEndTransparencyLayer() final;
    void recordDrawRect(const CyberCore::FloatRect&, float) final;
    void recordDrawLine(const CyberCore::FloatPoint& point1, const CyberCore::FloatPoint& point2) final;
    void recordDrawLinesForText(const CyberCore::FloatPoint& blockLocation, const CyberCore::FloatSize& localAnchor, float thickness, const CyberCore::DashArray& widths, bool printing, bool doubleLines, CyberCore::StrokeStyle) final;
    void recordDrawDotsForDocumentMarker(const CyberCore::FloatRect&, const CyberCore::DocumentMarkerLineStyle&) final;
    void recordDrawEllipse(const CyberCore::FloatRect&) final;
    void recordDrawPath(const CyberCore::Path&) final;
    void recordDrawFocusRingPath(const CyberCore::Path&, float outlineWidth, const CyberCore::Color&) final;
    void recordDrawFocusRingRects(const Vector<CyberCore::FloatRect>&, float outlineOffset, float outlineWidth, const CyberCore::Color&) final;
    void recordFillRect(const CyberCore::FloatRect&) final;
    void recordFillRectWithColor(const CyberCore::FloatRect&, const CyberCore::Color&) final;
    void recordFillRectWithGradient(const CyberCore::FloatRect&, CyberCore::Gradient&) final;
    void recordFillCompositedRect(const CyberCore::FloatRect&, const CyberCore::Color&, CyberCore::CompositeOperator, CyberCore::BlendMode) final;
    void recordFillRoundedRect(const CyberCore::FloatRoundedRect&, const CyberCore::Color&, CyberCore::BlendMode) final;
    void recordFillRectWithRoundedHole(const CyberCore::FloatRect&, const CyberCore::FloatRoundedRect&, const CyberCore::Color&) final;
#if ENABLE(INLINE_PATH_DATA)
    void recordFillLine(const CyberCore::LineData&) final;
    void recordFillArc(const CyberCore::ArcData&) final;
    void recordFillQuadCurve(const CyberCore::QuadCurveData&) final;
    void recordFillBezierCurve(const CyberCore::BezierCurveData&) final;
#endif
    void recordFillPath(const CyberCore::Path&) final;
    void recordFillEllipse(const CyberCore::FloatRect&) final;
#if ENABLE(VIDEO)
    void recordPaintFrameForMedia(CyberCore::MediaPlayer&, const CyberCore::FloatRect& destination) final;
    void recordPaintVideoFrame(CyberCore::VideoFrame&, const CyberCore::FloatRect&, bool shouldDiscardAlpha) final;
#endif
    void recordStrokeRect(const CyberCore::FloatRect&, float) final;
#if ENABLE(INLINE_PATH_DATA)
    void recordStrokeLine(const CyberCore::LineData&) final;
    void recordStrokeLineWithColorAndThickness(CyberCore::SRGBA<uint8_t>, float, const CyberCore::LineData&) final;
    void recordStrokeArc(const CyberCore::ArcData&) final;
    void recordStrokeQuadCurve(const CyberCore::QuadCurveData&) final;
    void recordStrokeBezierCurve(const CyberCore::BezierCurveData&) final;
#endif
    void recordStrokePath(const CyberCore::Path&) final;
    void recordStrokeEllipse(const CyberCore::FloatRect&) final;
    void recordClearRect(const CyberCore::FloatRect&) final;
    void recordDrawControlPart(CyberCore::ControlPart&, const CyberCore::FloatRoundedRect& borderRect, float deviceScaleFactor, const CyberCore::ControlStyle&) final;
#if USE(CG)
    void recordApplyStrokePattern() final;
    void recordApplyFillPattern() final;
#endif
    void recordApplyDeviceScaleFactor(float) final;

    bool recordResourceUse(CyberCore::NativeImage&) final;
    bool recordResourceUse(CyberCore::ImageBuffer&) final;
    bool recordResourceUse(const CyberCore::SourceImage&) final;
    bool recordResourceUse(CyberCore::Font&) final;
    bool recordResourceUse(CyberCore::DecomposedGlyphs&) final;
    bool recordResourceUse(CyberCore::Gradient&) final;

    RefPtr<CyberCore::ImageBuffer> createImageBuffer(const CyberCore::FloatSize&, float resolutionScale, const CyberCore::DestinationColorSpace&, std::optional<CyberCore::RenderingMode>, std::optional<CyberCore::RenderingMethod>) const final;
    RefPtr<CyberCore::ImageBuffer> createAlignedImageBuffer(const CyberCore::FloatSize&, const CyberCore::DestinationColorSpace&, std::optional<CyberCore::RenderingMethod>) const final;
    RefPtr<CyberCore::ImageBuffer> createAlignedImageBuffer(const CyberCore::FloatRect&, const CyberCore::DestinationColorSpace&, std::optional<CyberCore::RenderingMethod>) const final;

    static inline constexpr Seconds defaultSendTimeout = 3_s;
    CyberCore::RenderingResourceIdentifier m_destinationBufferIdentifier;
    WeakPtr<RemoteImageBufferProxy> m_imageBuffer;
    WeakPtr<RemoteRenderingBackendProxy> m_renderingBackend;
#if PLATFORM(COCOA) && ENABLE(VIDEO)
    SharedVideoFrameWriter m_sharedVideoFrameWriter;
#endif
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
