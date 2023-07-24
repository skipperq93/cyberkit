/*
 * Copyright (C) 2013-2019 Apple Inc. All rights reserved.
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

#if USE(AUDIO_SESSION)

#include <memory>
#include <wtf/CompletionHandler.h>
#include <wtf/EnumTraits.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/Noncopyable.h>
#include <wtf/UniqueRef.h>
#include <wtf/WeakHashSet.h>
#include <wtf/text/WTFString.h>

namespace CyberCore {

class AudioSessionPrivate;

enum class RouteSharingPolicy : uint8_t {
    Default,
    LongFormAudio,
    Independent,
    LongFormVideo
};

class AudioSessionRoutingArbitrationClient;

class WEBCORE_EXPORT AudioSession {
    WTF_MAKE_FAST_ALLOCATED;
    WTF_MAKE_NONCOPYABLE(AudioSession);
    friend class UniqueRef<AudioSession>;
    friend UniqueRef<AudioSession> WTF::makeUniqueRefWithoutFastMallocCheck<AudioSession>();
public:
    static UniqueRef<AudioSession> create();
    static void setSharedSession(UniqueRef<AudioSession>&&);
    static AudioSession& sharedSession();
    virtual ~AudioSession();

    enum CategoryType : uint8_t {
        None,
        AmbientSound,
        SoloAmbientSound,
        MediaPlayback,
        RecordAudio,
        PlayAndRecord,
        AudioProcessing,
    };
    virtual void setCategory(CategoryType, RouteSharingPolicy);
    virtual CategoryType category() const;

    void setCategoryOverride(CategoryType);
    CategoryType categoryOverride() const;

    virtual RouteSharingPolicy routeSharingPolicy() const;
    virtual String routingContextUID() const;

    virtual float sampleRate() const;
    virtual size_t bufferSize() const;
    virtual size_t numberOfOutputChannels() const;

    bool tryToSetActive(bool);

    virtual size_t preferredBufferSize() const;
    virtual void setPreferredBufferSize(size_t);

    class MutedStateObserver {
    public:
        virtual ~MutedStateObserver() = default;

        virtual void hardwareMutedStateDidChange(AudioSession*) = 0;
    };

    void addMutedStateObserver(MutedStateObserver*);
    void removeMutedStateObserver(MutedStateObserver*);

    void audioOutputDeviceChanged();
    void setIsPlayingToBluetoothOverride(Optional<bool>);

    virtual bool isMuted() const;
    virtual void handleMutedStateChange();

    void beginInterruption();
    enum class MayResume { No, Yes };
    void endInterruption(MayResume);

    class InterruptionObserver : public CanMakeWeakPtr<InterruptionObserver> {
    public:
        virtual ~InterruptionObserver() = default;

        virtual void beginAudioSessionInterruption() = 0;
        virtual void endAudioSessionInterruption(MayResume) = 0;
    };
    void addInterruptionObserver(InterruptionObserver&);
    void removeInterruptionObserver(InterruptionObserver&);

    virtual bool isActive() const { return m_active; }

    void setRoutingArbitrationClient(WeakPtr<AudioSessionRoutingArbitrationClient>&& client) { m_routingArbitrationClient = client; }

protected:
    friend class NeverDestroyed<AudioSession>;
    AudioSession();

    virtual bool tryToSetActiveInternal(bool);

    std::unique_ptr<AudioSessionPrivate> m_private;
    HashSet<MutedStateObserver*> m_observers;
#if PLATFORM(IOS_FAMILY)
    WeakHashSet<InterruptionObserver> m_interruptionObservers;
#endif

    WeakPtr<AudioSessionRoutingArbitrationClient> m_routingArbitrationClient;
    bool m_active { false }; // Used only for testing.
};

class WEBCORE_EXPORT AudioSessionRoutingArbitrationClient {
public:
    virtual ~AudioSessionRoutingArbitrationClient() = default;

    enum class RoutingArbitrationError : uint8_t { None, Failed, Cancelled };
    enum class DefaultRouteChanged : bool { No, Yes };

    virtual void beginRoutingArbitrationWithCategory(AudioSession::CategoryType, CompletionHandler<void(RoutingArbitrationError, DefaultRouteChanged)>&&) = 0;
    virtual void leaveRoutingAbritration() = 0;

    using WeakValueType = AudioSessionRoutingArbitrationClient;
};

WEBCORE_EXPORT String convertEnumerationToString(RouteSharingPolicy);
WEBCORE_EXPORT String convertEnumerationToString(AudioSession::CategoryType);
WEBCORE_EXPORT String convertEnumerationToString(AudioSessionRoutingArbitrationClient::RoutingArbitrationError);
WEBCORE_EXPORT String convertEnumerationToString(AudioSessionRoutingArbitrationClient::DefaultRouteChanged);

} // namespace CyberCore

namespace WTF {
template<> struct EnumTraits<CyberCore::RouteSharingPolicy> {
    using values = EnumValues<
    CyberCore::RouteSharingPolicy,
    CyberCore::RouteSharingPolicy::Default,
    CyberCore::RouteSharingPolicy::LongFormAudio,
    CyberCore::RouteSharingPolicy::Independent,
    CyberCore::RouteSharingPolicy::LongFormVideo
    >;
};

template <> struct EnumTraits<CyberCore::AudioSession::CategoryType> {
    using values = EnumValues <
    CyberCore::AudioSession::CategoryType,
    CyberCore::AudioSession::CategoryType::None,
    CyberCore::AudioSession::CategoryType::AmbientSound,
    CyberCore::AudioSession::CategoryType::SoloAmbientSound,
    CyberCore::AudioSession::CategoryType::MediaPlayback,
    CyberCore::AudioSession::CategoryType::RecordAudio,
    CyberCore::AudioSession::CategoryType::PlayAndRecord,
    CyberCore::AudioSession::CategoryType::AudioProcessing
    >;
};

template <> struct EnumTraits<CyberCore::AudioSession::MayResume> {
    using values = EnumValues <
    CyberCore::AudioSession::MayResume,
    CyberCore::AudioSession::MayResume::No,
    CyberCore::AudioSession::MayResume::Yes
    >;
};

template <> struct EnumTraits<CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError> {
    using values = EnumValues <
    CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError,
    CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError::None,
    CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError::Failed,
    CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError::Cancelled
    >;
};

template <> struct EnumTraits<CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged> {
    using values = EnumValues <
    CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged,
    CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged::No,
    CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged::Yes
    >;
};

template<typename Type>
struct LogArgument;

template <>
struct LogArgument<CyberCore::RouteSharingPolicy> {
    static String toString(const CyberCore::RouteSharingPolicy policy)
    {
        return convertEnumerationToString(policy);
    }
};

template <>
struct LogArgument<CyberCore::AudioSession::CategoryType> {
    static String toString(const CyberCore::AudioSession::CategoryType category)
    {
        return convertEnumerationToString(category);
    }
};

template <>
struct LogArgument<CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError> {
    static String toString(const CyberCore::AudioSessionRoutingArbitrationClient::RoutingArbitrationError error)
    {
        return convertEnumerationToString(error);
    }
};
template <>
struct LogArgument<CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged> {
    static String toString(const CyberCore::AudioSessionRoutingArbitrationClient::DefaultRouteChanged changed)
    {
        return convertEnumerationToString(changed);
    }
};

} // namespace WTF

#endif // USE(AUDIO_SESSION)
