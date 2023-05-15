/*
 * Copyright (C) 2021-2023 Apple Inc.  All rights reserved.
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

#pragma once

#if ENABLE(GPU_PROCESS)

#include "QualifiedRenderingResourceIdentifier.h"
#include <CyberCore/DecomposedGlyphs.h>
#include <CyberCore/Font.h>
#include <CyberCore/FontCustomPlatformData.h>
#include <CyberCore/Gradient.h>
#include <CyberCore/ImageBuffer.h>
#include <CyberCore/NativeImage.h>
#include <CyberCore/ProcessIdentifier.h>
#include <CyberCore/SourceImage.h>
#include <wtf/HashMap.h>

namespace CyberKit {

class QualifiedResourceHeap {
public:
    QualifiedResourceHeap(CyberCore::ProcessIdentifier webProcessIdentifier)
        : m_webProcessIdentifier(webProcessIdentifier)
    {
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::ImageBuffer>&& imageBuffer)
    {
        add(renderingResourceIdentifier, WTFMove(imageBuffer), m_imageBufferCount);
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::NativeImage>&& image)
    {
        add(renderingResourceIdentifier, WTFMove(image), m_nativeImageCount);
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::Font>&& font)
    {
        add(renderingResourceIdentifier, WTFMove(font), m_fontCount);
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::DecomposedGlyphs>&& decomposedGlyphs)
    {
        add(renderingResourceIdentifier, WTFMove(decomposedGlyphs), m_decomposedGlyphsCount);
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::Gradient>&& gradient)
    {
        add(renderingResourceIdentifier, WTFMove(gradient), m_gradientCount);
    }

    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<CyberCore::FontCustomPlatformData>&& customPlatformData)
    {
        add(renderingResourceIdentifier, WTFMove(customPlatformData), m_customPlatformDataCount);
    }

    CyberCore::ImageBuffer* getImageBuffer(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::ImageBuffer>(renderingResourceIdentifier);
    }

    CyberCore::NativeImage* getNativeImage(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::NativeImage>(renderingResourceIdentifier);
    }
    
    std::optional<CyberCore::SourceImage> getSourceImage(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        if (!renderingResourceIdentifier)
            return std::nullopt;

        if (auto nativeImage = getNativeImage(renderingResourceIdentifier))
            return { { *nativeImage } };

        if (auto imageBuffer = getImageBuffer(renderingResourceIdentifier))
            return { { *imageBuffer } };

        return std::nullopt;
    }

    CyberCore::Font* getFont(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::Font>(renderingResourceIdentifier);
    }

    CyberCore::DecomposedGlyphs* getDecomposedGlyphs(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::DecomposedGlyphs>(renderingResourceIdentifier);
    }

    CyberCore::Gradient* getGradient(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::Gradient>(renderingResourceIdentifier);
    }

    CyberCore::FontCustomPlatformData* getFontCustomPlatformData(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        return get<CyberCore::FontCustomPlatformData>(renderingResourceIdentifier);
    }

    bool removeImageBuffer(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::ImageBuffer>(renderingResourceIdentifier, m_imageBufferCount);
    }

    bool removeNativeImage(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::NativeImage>(renderingResourceIdentifier, m_nativeImageCount);
    }

    bool removeFont(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::Font>(renderingResourceIdentifier, m_fontCount);
    }

    bool removeDecomposedGlyphs(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::DecomposedGlyphs>(renderingResourceIdentifier, m_decomposedGlyphsCount);
    }

    bool removeGradient(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::Gradient>(renderingResourceIdentifier, m_gradientCount);
    }

    bool removeFontCustomPlatformData(QualifiedRenderingResourceIdentifier renderingResourceIdentifier)
    {
        return remove<CyberCore::FontCustomPlatformData>(renderingResourceIdentifier, m_customPlatformDataCount);
    }

    void releaseAllResources()
    {
        checkInvariants();

        if (!m_nativeImageCount && !m_fontCount && !m_decomposedGlyphsCount && !m_gradientCount)
            return;

        m_resources.removeIf([] (const auto& resource) {
            return std::holds_alternative<Ref<CyberCore::NativeImage>>(resource.value)
                || std::holds_alternative<Ref<CyberCore::Font>>(resource.value)
                || std::holds_alternative<Ref<CyberCore::DecomposedGlyphs>>(resource.value)
                || std::holds_alternative<Ref<CyberCore::Gradient>>(resource.value)
                || std::holds_alternative<Ref<CyberCore::FontCustomPlatformData>>(resource.value);
        });

        m_nativeImageCount = 0;
        m_fontCount = 0;
        m_decomposedGlyphsCount = 0;
        m_gradientCount = 0;
        m_customPlatformDataCount = 0;

        checkInvariants();
    }

    void releaseAllImageResources()
    {
        checkInvariants();

        if (!m_nativeImageCount)
            return;

        m_resources.removeIf([] (const auto& resource) {
            return std::holds_alternative<Ref<CyberCore::NativeImage>>(resource.value);
        });

        m_nativeImageCount = 0;

        checkInvariants();
    }

private:
    template <typename T>
    void add(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, Ref<T>&& object, unsigned& counter)
    {
        checkInvariants();

        ASSERT(renderingResourceIdentifier.processIdentifier() == m_webProcessIdentifier);
        if (m_resources.add(renderingResourceIdentifier, WTFMove(object)).isNewEntry)
            ++counter;

        checkInvariants();
    }

    template <typename T>
    T* get(QualifiedRenderingResourceIdentifier renderingResourceIdentifier) const
    {
        checkInvariants();

        auto iterator = m_resources.find(renderingResourceIdentifier);
        if (iterator == m_resources.end())
            return nullptr;
        auto value = std::get_if<Ref<T>>(&iterator->value);
        return value ? value->ptr() : nullptr;
    }

    template <typename T>
    bool remove(QualifiedRenderingResourceIdentifier renderingResourceIdentifier, unsigned& counter)
    {
        checkInvariants();

        if (!counter)
            return false;

        auto iterator = m_resources.find(renderingResourceIdentifier);
        if (iterator == m_resources.end())
            return false;
        if (!std::holds_alternative<Ref<T>>(iterator->value))
            return false;

        auto result = m_resources.remove(iterator);
        ASSERT(result);
        --counter;

        checkInvariants();

        return result;
    }

    void checkInvariants() const
    {
#if ASSERT_ENABLED
        unsigned imageBufferCount = 0;
        unsigned nativeImageCount = 0;
        unsigned fontCount = 0;
        unsigned customPlatformDataCount = 0;
        unsigned decomposedGlyphsCount = 0;
        unsigned gradientCount = 0;
        for (const auto& pair : m_resources) {
            WTF::switchOn(pair.value, [&] (std::monostate) {
                ASSERT_NOT_REACHED();
            }, [&] (const Ref<CyberCore::ImageBuffer>&) {
                ++imageBufferCount;
            }, [&] (const Ref<CyberCore::NativeImage>&) {
                ++nativeImageCount;
            }, [&] (const Ref<CyberCore::Font>&) {
                ++fontCount;
            }, [&] (const Ref<CyberCore::FontCustomPlatformData>&) {
                ++customPlatformDataCount;
            }, [&] (const Ref<CyberCore::DecomposedGlyphs>&) {
                ++decomposedGlyphsCount;
            }, [&] (const Ref<CyberCore::Gradient>&) {
                ++gradientCount;
            });
        }
        ASSERT(imageBufferCount == m_imageBufferCount);
        ASSERT(nativeImageCount == m_nativeImageCount);
        ASSERT(fontCount == m_fontCount);
        ASSERT(customPlatformDataCount == m_customPlatformDataCount);
        ASSERT(decomposedGlyphsCount == m_decomposedGlyphsCount);
        ASSERT(gradientCount == m_gradientCount);
        ASSERT(m_resources.size() == m_imageBufferCount + m_nativeImageCount + m_fontCount + m_decomposedGlyphsCount + m_customPlatformDataCount + m_gradientCount);
#endif
    }

    using Resource = std::variant<
        std::monostate,
        Ref<CyberCore::ImageBuffer>,
        Ref<CyberCore::NativeImage>,
        Ref<CyberCore::Font>,
        Ref<CyberCore::DecomposedGlyphs>,
        Ref<CyberCore::Gradient>,
        Ref<CyberCore::FontCustomPlatformData>
    >;
    HashMap<QualifiedRenderingResourceIdentifier, Resource> m_resources;
    CyberCore::ProcessIdentifier m_webProcessIdentifier;
    unsigned m_imageBufferCount { 0 };
    unsigned m_nativeImageCount { 0 };
    unsigned m_fontCount { 0 };
    unsigned m_decomposedGlyphsCount { 0 };
    unsigned m_gradientCount { 0 };
    unsigned m_customPlatformDataCount { 0 };
};

} // namespace CyberKit

#endif // ENABLE(GPU_PROCESS)
