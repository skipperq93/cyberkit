/*
 * Copyright (C) 2018 Metrological Group B.V.
 * Copyright (C) 2018 Igalia S.L.
 * Copyright (C) 2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "GraphicsContext.h"
#include "NicosiaPaintingOperation.h"

namespace Nicosia {

class CairoOperationRecorder final : public CyberCore::GraphicsContext {
public:
    CairoOperationRecorder(PaintingOperations&);

private:
    bool hasPlatformContext() const override { return false; }
    PlatformGraphicsContext* platformContext() const override { return nullptr; }

    void didUpdateState(CyberCore::GraphicsContextState&) override;

    void setLineCap(CyberCore::LineCap) override;
    void setLineDash(const CyberCore::DashArray&, float) override;
    void setLineJoin(CyberCore::LineJoin) override;
    void setMiterLimit(float) override;

    void fillRect(const CyberCore::FloatRect&) override;
    void fillRect(const CyberCore::FloatRect&, const CyberCore::Color&) override;
    void fillRect(const CyberCore::FloatRect&, CyberCore::Gradient&) override;
    void fillRect(const CyberCore::FloatRect&, const CyberCore::Color&, CyberCore::CompositeOperator, CyberCore::BlendMode) override;
    void fillRoundedRectImpl(const CyberCore::FloatRoundedRect&, const CyberCore::Color&) override { ASSERT_NOT_REACHED(); }
    void fillRoundedRect(const CyberCore::FloatRoundedRect&, const CyberCore::Color&, CyberCore::BlendMode) override;
    void fillRectWithRoundedHole(const CyberCore::FloatRect&, const CyberCore::FloatRoundedRect&, const CyberCore::Color&) override;
    void fillPath(const CyberCore::Path&) override;
    void fillEllipse(const CyberCore::FloatRect&) override;
    void strokeRect(const CyberCore::FloatRect&, float) override;
    void strokePath(const CyberCore::Path&) override;
    void strokeEllipse(const CyberCore::FloatRect&) override;
    void clearRect(const CyberCore::FloatRect&) override;

    void drawGlyphs(const CyberCore::Font&, const CyberCore::GlyphBufferGlyph*, const CyberCore::GlyphBufferAdvance*, unsigned numGlyphs, const CyberCore::FloatPoint&, CyberCore::FontSmoothingMode) override;
    void drawDecomposedGlyphs(const CyberCore::Font&, const CyberCore::DecomposedGlyphs&) override;

    void drawImageBuffer(CyberCore::ImageBuffer&, const CyberCore::FloatRect& destination, const CyberCore::FloatRect& source, const CyberCore::ImagePaintingOptions&) override;
    void drawFilteredImageBuffer(CyberCore::ImageBuffer*, const CyberCore::FloatRect&, CyberCore::Filter&, CyberCore::FilterResults&) override;
    void drawNativeImageInternal(CyberCore::NativeImage&, const CyberCore::FloatSize&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::ImagePaintingOptions&) override;
    void drawPattern(CyberCore::NativeImage&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::AffineTransform&, const CyberCore::FloatPoint&, const CyberCore::FloatSize&, const CyberCore::ImagePaintingOptions&) override;

    void drawRect(const CyberCore::FloatRect&, float) override;
    void drawLine(const CyberCore::FloatPoint&, const CyberCore::FloatPoint&) override;
    void drawLinesForText(const CyberCore::FloatPoint&, float thickness, const CyberCore::DashArray&, bool, bool, CyberCore::StrokeStyle) override;
    void drawDotsForDocumentMarker(const CyberCore::FloatRect&, CyberCore::DocumentMarkerLineStyle) override;
    void drawEllipse(const CyberCore::FloatRect&) override;

    void drawFocusRing(const CyberCore::Path&, float outlineWidth, const CyberCore::Color&) override;
    void drawFocusRing(const Vector<CyberCore::FloatRect>&, float outlineOffset, float outlineWidth, const CyberCore::Color&) override;

    void save() override;
    void restore() override;

    void translate(float, float) override;
    void rotate(float angleInRadians) override;
    void scale(const CyberCore::FloatSize&) override;
    void concatCTM(const CyberCore::AffineTransform&) override;
    void setCTM(const CyberCore::AffineTransform&) override;
    CyberCore::AffineTransform getCTM(CyberCore::GraphicsContext::IncludeDeviceScale) const override;

    void beginTransparencyLayer(float) override;
    void endTransparencyLayer() override;

    void clip(const CyberCore::FloatRect&) override;
    void clipOut(const CyberCore::FloatRect&) override;
    void clipOut(const CyberCore::Path&) override;
    void clipPath(const CyberCore::Path&, CyberCore::WindRule) override;
    CyberCore::IntRect clipBounds() const override;
    void clipToImageBuffer(CyberCore::ImageBuffer&, const CyberCore::FloatRect&) override;
#if ENABLE(VIDEO)
    void paintFrameForMedia(CyberCore::MediaPlayer&, const CyberCore::FloatRect& destination) override;
#endif

    void applyDeviceScaleFactor(float) override;

    void append(std::unique_ptr<PaintingOperation>&&);
    PaintingOperations& m_commandList;

    struct State {
        CyberCore::AffineTransform ctm;
        CyberCore::AffineTransform ctmInverse;
        CyberCore::FloatRect clipBounds;
    };
    Vector<State, 32> m_stateStack;
};

} // namespace Nicosia
