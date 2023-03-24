/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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

#include "Test.h"
#include "CyberCoreTestUtilities.h"
#include <CyberCore/DestinationColorSpace.h>
#include <CyberCore/DisplayListDrawingContext.h>
#include <CyberCore/GraphicsContext.h>
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/InMemoryDisplayList.h>
#include <wtf/StdLibExtras.h>

#if PLATFORM(COCOA)
#include <CyberCore/GraphicsContextCG.h>
#endif

#if ENABLE(APPLE_PAY)
#include <CyberCore/ApplePayLogoSystemImage.h>
#endif

#if ENABLE(VIDEO)
#include <CyberCore/VideoFrame.h>
#endif

namespace TestCyberKitAPI {

constexpr unsigned testContextWidth = 77;
constexpr unsigned testContextHeight = 88;

static RefPtr<CyberCore::ImageBuffer> createReferenceTarget()
{
    auto colorSpace = CyberCore::DestinationColorSpace::SRGB();
    auto pixelFormat = CyberCore::PixelFormat::BGRA8;
    CyberCore::FloatSize logicalSize { testContextWidth, testContextHeight };
    float scale = 1;
    return CyberCore::ImageBuffer::create(logicalSize, CyberCore::RenderingPurpose::Unspecified, scale, colorSpace, pixelFormat);
}

static CyberCore::Path createTestPath()
{
    CyberCore::Path p;
    p.addLineTo({ 8, 9 });
    p.addLineTo({ 9, 0 });
    p.closeSubpath();
    return p;
}

static Ref<CyberCore::ImageBuffer> createTestImageBuffer()
{
    auto colorSpace = CyberCore::DestinationColorSpace::SRGB();
    auto pixelFormat = CyberCore::PixelFormat::BGRA8;
    CyberCore::FloatSize logicalSize { 3, 7 };
    float scale = 1;
    auto result = CyberCore::ImageBuffer::create(logicalSize, CyberCore::RenderingPurpose::Unspecified, scale, colorSpace, pixelFormat);
    RELEASE_ASSERT(result);
    return result.releaseNonNull();
}

namespace {

class TestDrawingContext : public CyberCore::DisplayList::DrawingContext {
public:
    TestDrawingContext(CyberCore::FloatSize logicalSize)
        : CyberCore::DisplayList::DrawingContext { logicalSize }
        , m_writingClient { makeUnique<CyberCore::DisplayList::InMemoryDisplayList::WritingClient>() }
        , m_readingClient { makeUnique<CyberCore::DisplayList::InMemoryDisplayList::ReadingClient>() }
    {
        displayList().setItemBufferWritingClient(m_writingClient.get());
        displayList().setItemBufferReadingClient(m_readingClient.get());
    }

private:
    std::unique_ptr<CyberCore::DisplayList::InMemoryDisplayList::WritingClient> m_writingClient;
    std::unique_ptr<CyberCore::DisplayList::InMemoryDisplayList::ReadingClient> m_readingClient;
};

}

static std::unique_ptr<TestDrawingContext> createDisplayListTarget()
{
    return makeUnique<TestDrawingContext>(CyberCore::FloatSize { testContextWidth, testContextHeight });
}

// Function that applies same functor to two contexts. BifurbicatedGraphicsContext seems to have missing features.
template<typename F>
void forBoth(CyberCore::GraphicsContext& a, CyberCore::GraphicsContext& b, F&& func)
{
    func(a);
    func(b);
}

#define FOR_EVERY_GRAPHICS_CONTEXT_STATE(MACRO) \
    MACRO(fillBrush) \
    MACRO(fillRule) \
    MACRO(strokeBrush) \
    MACRO(strokeThickness) \
    MACRO(strokeStyle) \
    MACRO(dropShadow) \
    MACRO(style) \
    MACRO(compositeMode) \
    MACRO(alpha) \
    MACRO(textDrawingMode) \
    MACRO(imageInterpolationQuality) \
    MACRO(shouldAntialias) \
    MACRO(shouldSmoothFonts) \
    MACRO(shouldSubpixelQuantizeFonts) \
    MACRO(shadowsIgnoreTransforms) \
    MACRO(drawLuminanceMask) \
    MACRO(isInTransparencyLayer) \
    MACRO(scaleFactor)

// FIXME: Currently using ImageBuffer, which applies custom CTM over the default.
//        MACRO(getCTM)
// FIXME: Move away from FOR_ macro because it doesn't survive ifdefs
//        MACRO(useDarkAppearance)

static testing::AssertionResult checkEqualState(CyberCore::GraphicsContext& a, CyberCore::GraphicsContext& b)
{
#define CHECK_STATE(STATE) \
    if (a.STATE() != b.STATE()) \
        return testing::AssertionFailure() << "State " #STATE " does not match.";
    FOR_EVERY_GRAPHICS_CONTEXT_STATE(CHECK_STATE);
#undef CHECK_STATE
    if (a.clipBounds() != b.clipBounds()) {
        return testing::AssertionFailure() << "GraphicsContext::clipBounds() does not match. Expected:" << a.clipBounds() << " got: " << b.clipBounds();
    }
    return testing::AssertionSuccess();
}

namespace {

template<typename Operation>
class DisplayListRecorderResultStateTest : public testing::Test {
protected:
    Operation operation() { return Operation { }; }
    String operationDescription() { return Operation::description(); }
};

struct NoCommands {
    void operator()(CyberCore::GraphicsContext& c)
    {
    }

    static String description()
    {
        return ""_s;
    }
};

struct ChangeAntialias {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0)))DL"_s;
    }
};

struct ChangeAntialiasBeforeSave {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.save(); // This is being tested. The previous antialias == false should be restored on matching restore().
        c.fillRect({ 0, 0, 5.5f, 5.7f });
        c.restore();
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(save)
(fill-rect
  (rect at (0,0) size 5.50x5.70))
(restore))DL"_s;
    }
};

struct ChangeAntialiasBeforeAndAfterSave {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.save(); // This is being tested. The previous antialias == false should be restored on matching restore().
        c.setShouldAntialias(true);
        c.fillRect({ 0, 0, 5.5f, 5.7f });
        c.restore();
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(save)
(set-state
  (change-flags [should-antialias])
  (should-antialias 1))
(fill-rect
  (rect at (0,0) size 5.50x5.70))
(restore))DL"_s;
    }
};

struct ChangeAntialiasInEmptySaveRestore {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.save();
        c.setShouldAntialias(false);
        c.restore(); // This is being tested. The previous antialias == false should be restored on restore().
    }

    static String description()
    {
        return R"DL(
(save)
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(restore))DL"_s;
    }
};

struct DrawSystemImage {
    void operator()(CyberCore::GraphicsContext& c)
    {
#if ENABLE(APPLE_PAY)
        auto image = CyberCore::ApplePayLogoSystemImage::create(CyberCore::ApplePayLogoStyle::White);
        c.setShouldAntialias(false);
        c.drawSystemImage(image.get(), {0, 0, 5, 5}); // This is being tested. The previous antialias == false should be used.
#endif
    }

    static String description()
    {
#if ENABLE(APPLE_PAY)
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(draw-system-image
  (destination at (0,0) size 5x5)))DL"_s;
#else
        return ""_s;
#endif
    }
};

struct ChangeAntialiasBeforeClipRect {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.clip({ 0,0, 9.f, 7.7f });
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(clip
  (rect at (0,0) size 9x7.70)))DL"_s;
    }
};

struct ChangeAntialiasBeforeClipOutRect {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.clipOut({ 0,0, 9.f, 7.7f });
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(clip-out
  (rect at (0,0) size 9x7.70)))DL"_s;
    }
};

struct ChangeAntialiasBeforeClipOutPath {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.clipOut(createTestPath());
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(clip-out-to-path
  (path move to (0,0), add line to (8,9), add line to (9,0), close subpath)))DL"_s;
    }
};

struct ChangeAntialiasBeforeClipPath {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.clipPath(createTestPath(), CyberCore::WindRule::NonZero);
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(clip-path
  (path move to (0,0), add line to (8,9), add line to (9,0), close subpath)
  (wind-rule NON-ZERO)))DL"_s;
    }
};

struct ChangeAntialiasBeforeClipToImageBuffer {
    void operator()(CyberCore::GraphicsContext& c)
    {
        c.setShouldAntialias(false);
        c.clipToImageBuffer(createTestImageBuffer().get(), { 0, 1, 77, 99 });
    }

    static String description()
    {
        return R"DL(
(set-state
  (change-flags [should-antialias])
  (should-antialias 0))
(clip-to-image-buffer
  (dest-rect at (0,1) size 77x99)))DL"_s;
    }
};

using AllOperations = testing::Types<NoCommands, ChangeAntialias, ChangeAntialiasBeforeSave,
    ChangeAntialiasBeforeAndAfterSave, ChangeAntialiasInEmptySaveRestore, DrawSystemImage, ChangeAntialiasBeforeClipRect,
    ChangeAntialiasBeforeClipOutRect, ChangeAntialiasBeforeClipOutPath, ChangeAntialiasBeforeClipPath,
    ChangeAntialiasBeforeClipToImageBuffer>;

}

TYPED_TEST_SUITE_P(DisplayListRecorderResultStateTest);

TYPED_TEST_P(DisplayListRecorderResultStateTest, StateThroughDisplayListIsPreserved)
{
    auto refTarget = createReferenceTarget();
    auto& ref = refTarget->context();
    auto testedTarget = createDisplayListTarget();
    auto& tested = testedTarget->recorder();
    EXPECT_TRUE(checkEqualState(ref, tested));

    forBoth(ref, tested, this->operation());

    EXPECT_TRUE(checkEqualState(ref, tested));

    tested.commitRecording();

    auto resultTarget = createReferenceTarget();
    auto& result = resultTarget->context();

    auto description = testedTarget->displayList().asText({ CyberCore::DisplayList::AsTextFlag::IncludePlatformOperations }).stripWhiteSpace();
    auto expectedDescription = this->operationDescription().stripWhiteSpace();
    EXPECT_EQ(expectedDescription, description);

    testedTarget->replayDisplayList(result);
    EXPECT_TRUE(checkEqualState(ref, result));

    // Unwind the stack in case the test-case tested state middle of the save/restore pair.
    // FIXME: It should be ok to destroy GraphicsContext in the middle of save / restore.
    while (ref.stackSize()) {
        forBoth(ref, tested, [](CyberCore::GraphicsContext& c) {
            c.restore();
        });
    }
    while (result.stackSize())
        result.restore();
}

REGISTER_TYPED_TEST_SUITE_P(DisplayListRecorderResultStateTest, StateThroughDisplayListIsPreserved);

INSTANTIATE_TYPED_TEST_SUITE_P(DisplayListRecorderTest, DisplayListRecorderResultStateTest, AllOperations);

}
