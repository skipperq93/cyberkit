/*
 * Copyright (C) 2004-2016 Apple Inc. All rights reserved.
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


#if ENABLE(VIDEO)

#import "DOMHTMLMediaElement.h"

#import "DOMInternal.h"
#import "DOMMediaErrorInternal.h"
#import "DOMNodeInternal.h"
#import "DOMTimeRangesInternal.h"
#import "ExceptionHandlers.h"
#import <CyberCore/ElementInlines.h>
#import <CyberCore/HTMLMediaElement.h>
#import <CyberCore/HTMLNames.h>
#import <CyberCore/JSExecState.h>
#import <CyberCore/MediaError.h>
#import <CyberCore/ThreadCheck.h>
#import <CyberCore/TimeRanges.h>
#import <CyberCore/WebScriptObjectPrivate.h>
#import <wtf/GetPtr.h>
#import <wtf/URL.h>

#define IMPL static_cast<CyberCore::HTMLMediaElement*>(reinterpret_cast<CyberCore::Node*>(_internal))

@implementation DOMHTMLMediaElement

- (DOMMediaError *)error
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->error()));
}

- (NSString *)src
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getURLAttribute(CyberCore::HTMLNames::srcAttr).string();
}

- (void)setSrc:(NSString *)newSrc
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::srcAttr, newSrc);
}

- (NSString *)currentSrc
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->currentSrc().string();
}

- (NSString *)crossOrigin
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->crossOrigin();
}

- (void)setCrossOrigin:(NSString *)newCrossOrigin
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setCrossOrigin(newCrossOrigin);
}

- (unsigned short)networkState
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->networkState();
}

- (NSString *)preload
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->preload();
}

- (void)setPreload:(NSString *)newPreload
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setPreload(newPreload);
}

- (DOMTimeRanges *)buffered
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->buffered()));
}

- (unsigned short)readyState
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->readyState();
}

- (BOOL)seeking
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->seeking();
}

- (double)currentTime
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->currentTime();
}

- (void)setCurrentTime:(double)newCurrentTime
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setCurrentTimeForBindings(newCurrentTime));
}

- (double)duration
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->duration();
}

- (BOOL)paused
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->paused();
}

- (double)defaultPlaybackRate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->defaultPlaybackRate();
}

- (void)setDefaultPlaybackRate:(double)newDefaultPlaybackRate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setDefaultPlaybackRate(newDefaultPlaybackRate);
}

- (double)playbackRate
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->playbackRate();
}

- (void)setPlaybackRate:(double)newPlaybackRate
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setPlaybackRate(newPlaybackRate);
}

- (DOMTimeRanges *)played
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->played()));
}

- (DOMTimeRanges *)seekable
{
    CyberCore::JSMainThreadNullState state;
    return kit(WTF::getPtr(IMPL->seekable()));
}

- (BOOL)ended
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->ended();
}

- (BOOL)autoplay
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::autoplayAttr);
}

- (void)setAutoplay:(BOOL)newAutoplay
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::autoplayAttr, newAutoplay);
}

- (BOOL)loop
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::loopAttr);
}

- (void)setLoop:(BOOL)newLoop
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::loopAttr, newLoop);
}

- (BOOL)controls
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->controls();
}

- (void)setControls:(BOOL)newControls
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setControls(newControls);
}

- (double)volume
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->volume();
}

- (void)setVolume:(double)newVolume
{
    CyberCore::JSMainThreadNullState state;
    raiseOnDOMError(IMPL->setVolume(newVolume));
}

- (BOOL)muted
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->muted();
}

- (void)setMuted:(BOOL)newMuted
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setMuted(newMuted);
}

- (BOOL)defaultMuted
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->hasAttributeWithoutSynchronization(CyberCore::HTMLNames::mutedAttr);
}

- (void)setDefaultMuted:(BOOL)newDefaultMuted
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setBooleanAttribute(CyberCore::HTMLNames::mutedAttr, newDefaultMuted);
}

- (BOOL)webkitPreservesPitch
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitPreservesPitch();
}

- (void)setWebkitPreservesPitch:(BOOL)newWebkitPreservesPitch
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setWebkitPreservesPitch(newWebkitPreservesPitch);
}

- (BOOL)webkitHasClosedCaptions
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitHasClosedCaptions();
}

- (BOOL)webkitClosedCaptionsVisible
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->webkitClosedCaptionsVisible();
}

- (void)setWebkitClosedCaptionsVisible:(BOOL)newWebkitClosedCaptionsVisible
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setWebkitClosedCaptionsVisible(newWebkitClosedCaptionsVisible);
}

- (NSString *)mediaGroup
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->getAttribute(CyberCore::HTMLNames::mediagroupAttr);
}

- (void)setMediaGroup:(NSString *)newMediaGroup
{
    CyberCore::JSMainThreadNullState state;
    IMPL->setAttributeWithoutSynchronization(CyberCore::HTMLNames::mediagroupAttr, newMediaGroup);
}

- (void)load
{
    CyberCore::JSMainThreadNullState state;
    IMPL->load();
}

- (NSString *)canPlayType:(NSString *)type
{
    CyberCore::JSMainThreadNullState state;
    return IMPL->canPlayType(type);
}

- (NSTimeInterval)getStartDate
{
    CyberCore::JSMainThreadNullState state;
    return kit(IMPL->getStartDate());
}

- (void)play
{
    CyberCore::JSMainThreadNullState state;
    IMPL->play();
}

- (void)pause
{
    CyberCore::JSMainThreadNullState state;
    IMPL->pause();
}

- (void)fastSeek:(double)time
{
    CyberCore::JSMainThreadNullState state;
    IMPL->fastSeek(time);
}

@end

#endif // ENABLE(VIDEO)

#undef IMPL
