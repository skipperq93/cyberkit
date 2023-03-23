/*
 *  Copyright (C) 2017-2022 Igalia S.L. All rights reserved.
 *  Copyright (C) 2022 Metrological Group B.V.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#if USE(GSTREAMER_WEBRTC)

#include "RealtimeIncomingSourceGStreamer.h"

namespace CyberCore {

class RealtimeIncomingAudioSourceGStreamer : public RealtimeIncomingSourceGStreamer {
public:
    static Ref<RealtimeIncomingAudioSourceGStreamer> create(AtomString&& audioTrackId) { return adoptRef(*new RealtimeIncomingAudioSourceGStreamer(WTFMove(audioTrackId))); }

protected:
    RealtimeIncomingAudioSourceGStreamer(AtomString&&);
    ~RealtimeIncomingAudioSourceGStreamer();

private:
    // RealtimeMediaSource API
    const RealtimeMediaSourceSettings& settings() final;
    bool isIncomingAudioSource() const final { return true; }

    // RealtimeIncomingSourceGStreamer API
    void dispatchSample(GRefPtr<GstSample>&&) final;

    RealtimeMediaSourceSettings m_currentSettings;
};

} // namespace CyberCore

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberCore::RealtimeIncomingAudioSourceGStreamer)
    static bool isType(const CyberCore::RealtimeMediaSource& source) { return source.isIncomingAudioSource(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif // USE(GSTREAMER_WEBRTC)
