/*
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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

#if USE(PLUGIN_HOST_PROCESS)

#ifndef HostedNetscapePluginStream_h
#define HostedNetscapePluginStream_h

#include <CyberCore/NetscapePlugInStreamLoader.h>
#include <CyberKitLegacy/npapi.h>
#include <wtf/Ref.h>
#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>
#include <wtf/RetainPtr.h>

namespace CyberCore {
    class FrameLoader;
    class NetscapePlugInStreamLoader;
}

namespace CyberKit {

class NetscapePluginInstanceProxy;

class HostedNetscapePluginStream : public RefCounted<HostedNetscapePluginStream>
                                 , private CyberCore::NetscapePlugInStreamLoaderClient {
public:
    static Ref<HostedNetscapePluginStream> create(NetscapePluginInstanceProxy* instance, uint32_t streamID, NSURLRequest *request)
    {
        return adoptRef(*new HostedNetscapePluginStream(instance, streamID, request));
    }
    static Ref<HostedNetscapePluginStream> create(NetscapePluginInstanceProxy* instance, CyberCore::FrameLoader* frameLoader)
    {
        return adoptRef(*new HostedNetscapePluginStream(instance, frameLoader));
    }

    ~HostedNetscapePluginStream();

    uint32_t streamID() const { return m_streamID; }

    void startStreamWithResponse(NSURLResponse *response);

    // FIXME: Can these be made private?
    void didReceiveData(CyberCore::NetscapePlugInStreamLoader*, const char* bytes, int length) override;
    void didFinishLoading(CyberCore::NetscapePlugInStreamLoader*) override;
    void didFail(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::ResourceError&) override;

    void start();
    void stop();

    void cancelLoad(NPReason reason);

    static NPReason reasonForError(NSError* error);

private:
    NSError *errorForReason(NPReason) const;
    void cancelLoad(NSError *);

    HostedNetscapePluginStream(NetscapePluginInstanceProxy*, uint32_t streamID, NSURLRequest *);
    HostedNetscapePluginStream(NetscapePluginInstanceProxy*, CyberCore::FrameLoader*);
    
    void startStream(NSURL *, long long expectedContentLength, NSDate *lastModifiedDate, NSString *mimeType, NSData *headers);

    NSError *pluginCancelledConnectionError() const;

    // NetscapePlugInStreamLoaderClient methods.
    void willSendRequest(CyberCore::NetscapePlugInStreamLoader*, CyberCore::ResourceRequest&&, const CyberCore::ResourceResponse& redirectResponse, CompletionHandler<void(CyberCore::ResourceRequest&&)>&&) override;
    void didReceiveResponse(CyberCore::NetscapePlugInStreamLoader*, const CyberCore::ResourceResponse&) override;
    bool wantsAllStreams() const override;
    
    RefPtr<NetscapePluginInstanceProxy> m_instance;
    uint32_t m_streamID;
    RetainPtr<NSMutableURLRequest> m_request;

    RetainPtr<NSURL> m_requestURL;
    RetainPtr<NSURL> m_responseURL;
    RetainPtr<NSString> m_mimeType;

    CyberCore::FrameLoader* m_frameLoader;
    RefPtr<CyberCore::NetscapePlugInStreamLoader> m_loader;
};

}

#endif // HostedNetscapePluginStream_h
#endif // USE(PLUGIN_HOST_PROCESS)
