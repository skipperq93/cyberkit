/*
 * Copyright (C) 2014 Igalia S.L.
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

#if USE(COORDINATED_GRAPHICS_THREADED)

#include "CompositingRunLoop.h"
#include "CoordinatedGraphicsScene.h"
#include <CyberCore/CoordinatedGraphicsState.h>
#include <CyberCore/GLContext.h>
#include <CyberCore/IntSize.h>
#include <CyberCore/TextureMapper.h>
#include <wtf/Atomics.h>
#include <wtf/FastMalloc.h>
#include <wtf/Noncopyable.h>
#include <wtf/ThreadSafeRefCounted.h>

#if USE(REQUEST_ANIMATION_FRAME_DISPLAY_MONITOR)
#include "ThreadedDisplayRefreshMonitor.h"
#endif

namespace CyberKit {

class CoordinatedGraphicsScene;
class CoordinatedGraphicsSceneClient;

class ThreadedCompositor : public CoordinatedGraphicsSceneClient, public ThreadSafeRefCounted<ThreadedCompositor> {
    WTF_MAKE_NONCOPYABLE(ThreadedCompositor);
    WTF_MAKE_FAST_ALLOCATED;
public:
    class Client {
    public:
        virtual uint64_t nativeSurfaceHandleForCompositing() = 0;
        virtual void didDestroyGLContext() = 0;

        virtual void resize(const CyberCore::IntSize&) = 0;
        virtual void willRenderFrame() = 0;
        virtual void didRenderFrame() = 0;
    };

    enum class ShouldDoFrameSync { No, Yes };

    static Ref<ThreadedCompositor> create(Client&, ThreadedDisplayRefreshMonitor::Client&, CyberCore::PlatformDisplayID, const CyberCore::IntSize&, float scaleFactor, ShouldDoFrameSync = ShouldDoFrameSync::Yes, CyberCore::TextureMapper::PaintFlags = 0);
    virtual ~ThreadedCompositor();

    void setNativeSurfaceHandleForCompositing(uint64_t);
    void setScaleFactor(float);
    void setScrollPosition(const CyberCore::IntPoint&, float scale);
    void setViewportSize(const CyberCore::IntSize&, float scale);
    void setDrawsBackground(bool);

    void updateSceneState(const CyberCore::CoordinatedGraphicsState&);

    void invalidate();

    void forceRepaint();

#if USE(REQUEST_ANIMATION_FRAME_DISPLAY_MONITOR)
    RefPtr<CyberCore::DisplayRefreshMonitor> displayRefreshMonitor(CyberCore::PlatformDisplayID);
    void handleDisplayRefreshMonitorUpdate();
#endif

    void frameComplete();

private:
    ThreadedCompositor(Client&, ThreadedDisplayRefreshMonitor::Client&, CyberCore::PlatformDisplayID, const CyberCore::IntSize&, float scaleFactor, ShouldDoFrameSync, CyberCore::TextureMapper::PaintFlags);

    // CoordinatedGraphicsSceneClient
    void updateViewport() override;

    void renderLayerTree();
    void sceneUpdateFinished();

    void createGLContext();

    Client& m_client;
    RefPtr<CoordinatedGraphicsScene> m_scene;
    std::unique_ptr<CyberCore::GLContext> m_context;

    uint64_t m_nativeSurfaceHandle;
    ShouldDoFrameSync m_doFrameSync;
    CyberCore::TextureMapper::PaintFlags m_paintFlags { 0 };
    bool m_inForceRepaint { false };

    std::unique_ptr<CompositingRunLoop> m_compositingRunLoop;

    struct {
        Lock lock;
        CyberCore::IntSize viewportSize;
        CyberCore::IntPoint scrollPosition;
        float scaleFactor { 1 };
        bool drawsBackground { true };
        bool needsResize { false };

        Vector<CyberCore::CoordinatedGraphicsState> states;

        bool clientRendersNextFrame { false };
        bool coordinateUpdateCompletionWithClient { false };
    } m_attributes;

#if USE(REQUEST_ANIMATION_FRAME_DISPLAY_MONITOR)
    Ref<ThreadedDisplayRefreshMonitor> m_displayRefreshMonitor;
#endif
};

} // namespace CyberKit

#endif // USE(COORDINATED_GRAPHICS_THREADED)

