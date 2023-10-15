/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#import <CyberCore/FloatPoint.h>
#import <CyberCore/PageIdentifier.h>
#import <wtf/Lock.h>
#import <wtf/NakedPtr.h>

namespace CyberKit {
class WebPage;
}

namespace CyberCore {
class AXCoreObject;
}

@interface WKAccessibilityWebPageObjectBase : NSObject {
    NakedPtr<CyberKit::WebPage> m_page;
    CyberCore::PageIdentifier m_pageID;
#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
    Lock m_cacheLock;
    CyberCore::FloatPoint m_position WTF_GUARDED_BY_LOCK(m_cacheLock);
    CyberCore::IntSize m_size WTF_GUARDED_BY_LOCK(m_cacheLock);
    NakedPtr<CyberCore::AXCoreObject> m_isolatedTreeRoot WTF_GUARDED_BY_LOCK(m_cacheLock);
#endif
    RetainPtr<id> m_parent;
    bool m_hasMainFramePlugin;
}

- (void)setWebPage:(NakedPtr<CyberKit::WebPage>)page;
#if ENABLE(ACCESSIBILITY_ISOLATED_TREE)
- (void)setPosition:(const CyberCore::FloatPoint&)point;
- (void)setSize:(const CyberCore::IntSize&)size;
- (void)setIsolatedTreeRoot:(NakedPtr<CyberCore::AXCoreObject>)root;
#endif
- (void)setRemoteParent:(id)parent;
- (void)setHasMainFramePlugin:(bool)hasPlugin;

- (id)accessibilityRootObjectWrapper;
- (id)accessibilityFocusedUIElement;

@end
