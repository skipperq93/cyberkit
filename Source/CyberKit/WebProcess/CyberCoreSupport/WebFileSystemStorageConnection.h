/*
 * Copyright (C) 2021 Apple Inc. All rights reserved.
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

#include "Connection.h"
#include "FileSystemStorageError.h"
#include <CyberCore/FileSystemStorageConnection.h>

namespace IPC {
class Connection;

template<> struct AsyncReplyError<Expected<CyberCore::FileSystemHandleIdentifier, CyberKit::FileSystemStorageError>> {
    static Expected<CyberCore::FileSystemHandleIdentifier, CyberKit::FileSystemStorageError> create()
    {
        return makeUnexpected(CyberKit::FileSystemStorageError::Unknown);
    }
};

template<> struct AsyncReplyError<Expected<Vector<String>, CyberKit::FileSystemStorageError>> {
    static Expected<Vector<String>, CyberKit::FileSystemStorageError> create()
    {
        return makeUnexpected(CyberKit::FileSystemStorageError::Unknown);
    }
};

template<> struct AsyncReplyError<Expected<std::pair<String, bool>, CyberKit::FileSystemStorageError>> {
    static Expected<std::pair<String, bool>, CyberKit::FileSystemStorageError> create()
    {
        return makeUnexpected(CyberKit::FileSystemStorageError::Unknown);
    }
};

}

namespace CyberCore {
template<typename> class ExceptionOr;
class FileSystemDirectoryHandle;
class FileSystemFileHandle;
}

namespace CyberKit {

class WebFileSystemStorageConnection final : public CyberCore::FileSystemStorageConnection {
public:
    static Ref<WebFileSystemStorageConnection> create(IPC::Connection&);
    void connectionClosed();
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&);

private:
    explicit WebFileSystemStorageConnection(IPC::Connection&);

    // FileSystemStorageConnection
    void closeHandle(CyberCore::FileSystemHandleIdentifier) final;
    void isSameEntry(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemStorageConnection::SameEntryCallback&&) final;
    void move(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, const String& newName, VoidCallback&&) final;
    void getFileHandle(CyberCore::FileSystemHandleIdentifier, const String& name, bool createIfNecessary, CyberCore::FileSystemStorageConnection::GetHandleCallback&&) final;
    void getDirectoryHandle(CyberCore::FileSystemHandleIdentifier, const String& name, bool createIfNecessary, CyberCore::FileSystemStorageConnection::GetHandleCallback&&) final;
    void removeEntry(CyberCore::FileSystemHandleIdentifier, const String& name, bool deleteRecursively, CyberCore::FileSystemStorageConnection::VoidCallback&&) final;
    void resolve(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemStorageConnection::ResolveCallback&&) final;
    void getHandleNames(CyberCore::FileSystemHandleIdentifier, FileSystemStorageConnection::GetHandleNamesCallback&&) final;
    void getHandle(CyberCore::FileSystemHandleIdentifier, const String& name, FileSystemStorageConnection::GetHandleCallback&&) final;
    void getFile(CyberCore::FileSystemHandleIdentifier, StringCallback&&) final;

    void createSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemStorageConnection::GetAccessHandleCallback&&) final;
    void closeSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemSyncAccessHandleIdentifier, EmptyCallback&&) final;
    void requestNewCapacityForSyncAccessHandle(CyberCore::FileSystemHandleIdentifier, CyberCore::FileSystemSyncAccessHandleIdentifier, uint64_t newCapacity, RequestCapacityCallback&& completionHandler) final;
    void registerSyncAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier, CyberCore::ScriptExecutionContextIdentifier) final;
    void unregisterSyncAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier) final;
    void invalidateAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier) final;

    HashMap<CyberCore::FileSystemSyncAccessHandleIdentifier, CyberCore::ScriptExecutionContextIdentifier> m_syncAccessHandles;
    RefPtr<IPC::Connection> m_connection;
};

} // namespace CyberKit
