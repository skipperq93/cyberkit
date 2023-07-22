/*
 * Copyright (C) 2018 Metrological Group B.V.
 * Copyright (C) 2018 Igalia S.L.
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

#include "GraphicsContextImpl.h"
#include "NicosiaPaintingOperation.h"

namespace Nicosia {

class CairoOperationRecorder final : public CyberCore::GraphicsContextImpl {
public:
    CairoOperationRecorder(CyberCore::GraphicsContext&, PaintingOperations&);

private:
    bool hasPlatformContext() const override { return false; }
    PlatformGraphicsContext* platformContext() const override { return nullptr; }

    void updateState(const CyberCore::GraphicsContextState&, CyberCore::GraphicsContextState::StateChangeFlags) override;
    void clearShadow() override;

    void setLineCap(CyberCore::LineCap) override;
    void setLineDash(const DashArray&, float) override;
    void setLineJoin(CyberCore::LineJoin) override;
    void setMiterLimit(float) override;

    void fillRect(const CyberCore::FloatRect&) override;
    void fillRect(const CyberCore::FloatRect&, const CyberCore::Color&) override;
    void fillRect(const CyberCore::FloatRect&, CyberCore::Gradient&) override;
    void fillRect(const CyberCore::FloatRect&, const CyberCore::Color&, CyberCore::CompositeOperator, CyberCore::BlendMode) override;
    void fillRoundedRect(const CyberCore::FloatRoundedRect&, const CyberCore::Color&, CyberCore::BlendMode) override;
    void fillRectWithRoundedHole(const CyberCore::FloatRect&, const CyberCore::FloatRoundedRect&, const CyberCore::Color&) override;
    void fillPath(const CyberCore::Path&) override;
    void fillEllipse(const CyberCore::FloatRect&) override;
    void strokeRect(const CyberCore::FloatRect&, float) override;
    void strokePath(const CyberCore::Path&) override;
    void strokeEllipse(const CyberCore::FloatRect&) override;
    void clearRect(const CyberCore::FloatRect&) override;

    void drawGlyphs(const CyberCore::Font&, const CyberCore::GlyphBuffer&, unsigned, unsigned, const CyberCore::FloatPoint&, CyberCore::FontSmoothingMode) override;

    CyberCore::ImageDrawResult drawImage(CyberCore::Image&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::ImagePaintingOptions&) override;
    CyberCore::ImageDrawResult drawTiledImage(CyberCore::Image&, const CyberCore::FloatRect&, const CyberCore::FloatPoint&, const CyberCore::FloatSize&, const CyberCore::FloatSize&, const CyberCore::ImagePaintingOptions&) override;
    CyberCore::ImageDrawResult drawTiledImage(CyberCore::Image&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::FloatSize&, CyberCore::Image::TileRule, CyberCore::Image::TileRule, const CyberCore::ImagePaintingOptions&) override;
    void drawNativeImage(const CyberCore::NativeImagePtr&, const CyberCore::FloatSize&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::ImagePaintingOptions&) override;
    void drawPattern(CyberCore::Image&, const CyberCore::FloatRect&, const CyberCore::FloatRect&, const CyberCore::AffineTransform&, const CyberCore::FloatPoint&, const CyberCore::FloatSize&, const CyberCore::ImagePaintingOptions&) override;

    void drawRect(const CyberCore::FloatRect&, float) override;
    void drawLine(const CyberCore::FloatPoint&, const CyberCore::FloatPoint&) override;
    void drawLinesForText(const CyberCore::FloatPoint&, float thickness, const DashArray&, bool, bool) override;
    void drawDotsForDocumentMarker(const CyberCore::FloatRect&, CyberCore::DocumentMarkerLineStyle) override;
    void drawEllipse(const CyberCore::FloatRect&) override;
    void drawPath(const CyberCore::Path&) override;

    void drawFocusRing(const CyberCore::Path&, float, float, const CyberCore::Color&) override;
    void drawFocusRing(const Vector<CyberCore::FloatRect>&, float, float, const CyberCore::Color&) override;

    void save() override;
    void restore() override;

    void translate(float, float) override;
    void rotate(float angleInRadians) override;
    void scale(const CyberCore::FloatSize&) override;
    void concatCTM(const CyberCore::AffineTransform&) override;
    void setCTM(const CyberCore::AffineTransform&) override;
    CyberCore::AffineTransform getCTM(CyberCore::GraphicsContext::IncludeDeviceScale) override;

    void beginTransparencyLayer(float) override;
    void endTransparencyLayer() override;

    void clip(const CyberCore::FloatRect&) override;
    void clipOut(const CyberCore::FloatRect&) override;
    void clipOut(const CyberCore::Path&) override;
    void clipPath(const CyberCore::Path&, CyberCore::WindRule) override;
    CyberCore::IntRect clipBounds() override;
    void clipToImageBuffer(CyberCore::ImageBuffer&, const CyberCore::FloatRect&) override;

    void applyDeviceScaleFactor(float) override;

    CyberCore::FloatRect roundToDevicePixels(const CyberCore::FloatRect&, CyberCore::GraphicsContext::RoundingMode) override;

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
