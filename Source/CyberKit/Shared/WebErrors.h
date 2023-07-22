/*
 * Copyright (C) 2010-2016 Apple Inc. All rights reserved.
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

#include <wtf/Forward.h>

namespace CyberCore {
class ResourceError;
class ResourceRequest;
class ResourceResponse;
}

namespace CyberKit {

CyberCore::ResourceError cancelledError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError blockedError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError blockedByContentBlockerError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError cannotShowURLError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError wasBlockedByRestrictionsError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError interruptedForPolicyChangeError(const CyberCore::ResourceRequest&);
CyberCore::ResourceError failedCustomProtocolSyncLoad(const CyberCore::ResourceRequest&);
#if ENABLE(CONTENT_FILTERING)
CyberCore::ResourceError blockedByContentFilterError(const CyberCore::ResourceRequest&);
#endif
CyberCore::ResourceError cannotShowMIMETypeError(const CyberCore::ResourceResponse&);
CyberCore::ResourceError fileDoesNotExistError(const CyberCore::ResourceResponse&);
CyberCore::ResourceError pluginWillHandleLoadError(const CyberCore::ResourceResponse&);
CyberCore::ResourceError internalError(const URL&);

#if USE(SOUP)
CyberCore::ResourceError downloadNetworkError(const URL&, const WTF::String&);
CyberCore::ResourceError downloadCancelledByUserError(const CyberCore::ResourceResponse&);
CyberCore::ResourceError downloadDestinationError(const CyberCore::ResourceResponse&, const WTF::String&);
#endif

#if PLATFORM(GTK)
CyberCore::ResourceError printError(const URL&, const WTF::String&);
CyberCore::ResourceError printerNotFoundError(const URL&);
CyberCore::ResourceError invalidPageRangeToPrint(const URL&);
#endif

} // namespace CyberKit
