/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#include <CyberCore/Color.h>
#include <CyberCore/IntPoint.h>
#include <CyberCore/SecurityOriginData.h>
#include <wtf/ListHashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/text/WTFString.h>

#if HAVE(IOSURFACE)
#include <CyberCore/IOSurface.h>
#endif

#if PLATFORM(GTK)
#if USE(GTK4)
#include <gtk/gtk.h>
#else
#include <CyberCore/RefPtrCairo.h>
#endif
#endif

namespace CyberKit {

class WebBackForwardListItem;
class WebPageProxy;

class ViewSnapshot : public RefCounted<ViewSnapshot> {
public:
#if HAVE(IOSURFACE)
    static Ref<ViewSnapshot> create(std::unique_ptr<CyberCore::IOSurface>);
#endif
#if PLATFORM(GTK)
#if USE(GTK4)
    static Ref<ViewSnapshot> create(GRefPtr<GdkTexture>&&);
#else
    static Ref<ViewSnapshot> create(RefPtr<cairo_surface_t>&&);
#endif
#endif

    ~ViewSnapshot();

    void clearImage();
    bool hasImage() const;

#if HAVE(IOSURFACE)
    id asLayerContents();
    RetainPtr<CGImageRef> asImageForTesting();
#endif

    void setRenderTreeSize(uint64_t renderTreeSize) { m_renderTreeSize = renderTreeSize; }
    uint64_t renderTreeSize() const { return m_renderTreeSize; }

    void setBackgroundColor(const CyberCore::Color& color) { m_backgroundColor = color; }
    CyberCore::Color backgroundColor() const { return m_backgroundColor; }

    void setViewScrollPosition(CyberCore::IntPoint scrollPosition) { m_viewScrollPosition = scrollPosition; }
    CyberCore::IntPoint viewScrollPosition() const { return m_viewScrollPosition; }

    void setDeviceScaleFactor(float deviceScaleFactor) { m_deviceScaleFactor = deviceScaleFactor; }
    float deviceScaleFactor() const { return m_deviceScaleFactor; }

    void setOrigin(CyberCore::SecurityOriginData&& origin) { m_origin = WTFMove(origin); }
    const CyberCore::SecurityOriginData& origin() const { return m_origin; }

#if HAVE(IOSURFACE)
    CyberCore::IOSurface* surface() const { return m_surface.get(); }

    size_t estimatedImageSizeInBytes() const { return m_surface ? m_surface->totalBytes() : 0; }
    CyberCore::IntSize size() const { return m_surface ? m_surface->size() : CyberCore::IntSize(); }

    void setSurface(std::unique_ptr<CyberCore::IOSurface>);

    CyberCore::SetNonVolatileResult setVolatile(bool);
#endif

#if PLATFORM(GTK)
#if USE(GTK4)
    GdkTexture* texture() const { return m_texture.get(); }
#else
    cairo_surface_t* surface() const { return m_surface.get(); }
#endif

    size_t estimatedImageSizeInBytes() const;
    CyberCore::IntSize size() const;
#endif

private:
#if HAVE(IOSURFACE)
    explicit ViewSnapshot(std::unique_ptr<CyberCore::IOSurface>);

    std::unique_ptr<CyberCore::IOSurface> m_surface;
#endif

#if PLATFORM(GTK)
#if USE(GTK4)
    explicit ViewSnapshot(GRefPtr<GdkTexture>&&);

    GRefPtr<GdkTexture> m_texture;
#else
    explicit ViewSnapshot(RefPtr<cairo_surface_t>&&);

    RefPtr<cairo_surface_t> m_surface;
#endif
#endif

    uint64_t m_renderTreeSize;
    float m_deviceScaleFactor;
    CyberCore::Color m_backgroundColor;
    CyberCore::IntPoint m_viewScrollPosition; // Scroll position at snapshot time. Integral to make comparison reliable.
    CyberCore::SecurityOriginData m_origin;
};

class ViewSnapshotStore {
    WTF_MAKE_NONCOPYABLE(ViewSnapshotStore);
    friend class ViewSnapshot;
public:
    ViewSnapshotStore();
    ~ViewSnapshotStore();

    static ViewSnapshotStore& singleton();

    void recordSnapshot(WebPageProxy&, WebBackForwardListItem&);

    void discardSnapshotImages();
    void discardSnapshotImagesForOrigin(const CyberCore::SecurityOriginData&);

    void setDisableSnapshotVolatilityForTesting(bool disable) { m_disableSnapshotVolatility = disable; }
    bool disableSnapshotVolatilityForTesting() const { return m_disableSnapshotVolatility; }

private:
    void didAddImageToSnapshot(ViewSnapshot&);
    void willRemoveImageFromSnapshot(ViewSnapshot&);
    void pruneSnapshots(WebPageProxy&);

    size_t m_snapshotCacheSize { 0 };

    ListHashSet<ViewSnapshot*> m_snapshotsWithImages;
    bool m_disableSnapshotVolatility { false };
};

} // namespace CyberKit
