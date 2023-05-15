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

#include "config.h"
#include "WebFileSystemStorageConnection.h"

#include "NetworkStorageManagerMessages.h"
#include <CyberCore/ExceptionOr.h>
#include <CyberCore/FileSystemDirectoryHandle.h>
#include <CyberCore/FileSystemFileHandle.h>
#include <CyberCore/FileSystemHandleCloseScope.h>
#include <CyberCore/ScriptExecutionContext.h>
#include <CyberCore/WorkerFileSystemStorageConnection.h>
#include <CyberCore/WorkerGlobalScope.h>

namespace CyberKit {

Ref<WebFileSystemStorageConnection> WebFileSystemStorageConnection::create(IPC::Connection& connection)
{
    return adoptRef(*new WebFileSystemStorageConnection(connection));
}

WebFileSystemStorageConnection::WebFileSystemStorageConnection(IPC::Connection& connection)
    : m_connection(&connection)
{
}

void WebFileSystemStorageConnection::connectionClosed()
{
    m_connection = nullptr;

    for (auto identifier : m_syncAccessHandles.keys())
        invalidateAccessHandle(identifier);
}

void WebFileSystemStorageConnection::closeHandle(CyberCore::FileSystemHandleIdentifier identifier)
{
    if (!m_connection)
        return;

    m_connection->send(Messages::NetworkStorageManager::CloseHandle(identifier), 0);
}

void WebFileSystemStorageConnection::isSameEntry(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemHandleIdentifier otherIdentifier, CyberCore::FileSystemStorageConnection::SameEntryCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    if (identifier == otherIdentifier)
        return completionHandler(true);

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::IsSameEntry(identifier, otherIdentifier), WTFMove(completionHandler));
}

void WebFileSystemStorageConnection::getFileHandle(CyberCore::FileSystemHandleIdentifier identifier, const String& name, bool createIfNecessary, CyberCore::FileSystemStorageConnection::GetHandleCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::GetFileHandle(identifier, name, createIfNecessary), [this, protectedThis = Ref { *this }, name, completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        auto identifier = result.value();
        ASSERT(identifier.isValid());
        completionHandler(CyberCore::FileSystemHandleCloseScope::create(identifier, false, *this));
    });
}

void WebFileSystemStorageConnection::getDirectoryHandle(CyberCore::FileSystemHandleIdentifier identifier, const String& name, bool createIfNecessary, CyberCore::FileSystemStorageConnection::GetHandleCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::GetDirectoryHandle(identifier, name, createIfNecessary), [this, protectedThis = Ref { *this }, name, completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        auto identifier = result.value();
        ASSERT(identifier.isValid());
        completionHandler(CyberCore::FileSystemHandleCloseScope::create(identifier, true, *this));
    });
}

void WebFileSystemStorageConnection::removeEntry(CyberCore::FileSystemHandleIdentifier identifier, const String& name, bool deleteRecursively, CyberCore::FileSystemStorageConnection::VoidCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::RemoveEntry(identifier, name, deleteRecursively), [completionHandler = WTFMove(completionHandler)](auto error) mutable {
        return completionHandler(convertToExceptionOr(error));
    });
}

void WebFileSystemStorageConnection::resolve(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemHandleIdentifier otherIdentifier, CyberCore::FileSystemStorageConnection::ResolveCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::Resolve(identifier, otherIdentifier), [completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        completionHandler(WTFMove(result.value()));
    });
}

void WebFileSystemStorageConnection::getFile(CyberCore::FileSystemHandleIdentifier identifier, StringCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::GetFile(identifier), [completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        completionHandler(WTFMove(result.value()));
    });
}

void WebFileSystemStorageConnection::createSyncAccessHandle(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemStorageConnection::GetAccessHandleCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::CreateSyncAccessHandle(identifier), [completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        completionHandler(CyberCore::FileSystemStorageConnection::SyncAccessHandleInfo { result->identifier, result->handle.release(), result->capacity });
    });
}

void WebFileSystemStorageConnection::closeSyncAccessHandle(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemSyncAccessHandleIdentifier accessHandleIdentifier, FileSystemStorageConnection::EmptyCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler();

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::CloseSyncAccessHandle(identifier, accessHandleIdentifier), WTFMove(completionHandler));
}

void WebFileSystemStorageConnection::getHandleNames(CyberCore::FileSystemHandleIdentifier identifier, FileSystemStorageConnection::GetHandleNamesCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::GetHandleNames(identifier), [completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));

        completionHandler(WTFMove(result.value()));
    });
}

void WebFileSystemStorageConnection::getHandle(CyberCore::FileSystemHandleIdentifier identifier, const String& name, FileSystemStorageConnection::GetHandleCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::GetHandle(identifier, name), [this, protectedThis = Ref { *this }, completionHandler = WTFMove(completionHandler)](auto result) mutable {
        if (!result)
            return completionHandler(convertToException(result.error()));
        
        auto [identifier, isDirectory] = result.value();
        ASSERT(identifier.isValid());
        completionHandler(CyberCore::FileSystemHandleCloseScope::create(identifier, isDirectory, *this));
    });
}

void WebFileSystemStorageConnection::move(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemHandleIdentifier destinationIdentifier, const String& newName, VoidCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(CyberCore::Exception { CyberCore::UnknownError, "Connection is lost"_s });

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::Move(identifier, destinationIdentifier, newName), [completionHandler = WTFMove(completionHandler)](auto error) mutable {
        completionHandler(convertToExceptionOr(error));
    });
}

void WebFileSystemStorageConnection::registerSyncAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier identifier, CyberCore::ScriptExecutionContextIdentifier contextIdentifier)
{
    m_syncAccessHandles.add(identifier, contextIdentifier);
}

void WebFileSystemStorageConnection::unregisterSyncAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier identifier)
{
    m_syncAccessHandles.remove(identifier);
}

void WebFileSystemStorageConnection::invalidateAccessHandle(CyberCore::FileSystemSyncAccessHandleIdentifier identifier)
{
    if (auto contextIdentifier = m_syncAccessHandles.get(identifier)) {
        CyberCore::ScriptExecutionContext::postTaskTo(contextIdentifier, [identifier](auto& context) mutable {
            if (FileSystemStorageConnection* connection = downcast<CyberCore::WorkerGlobalScope>(context).fileSystemStorageConnection())
                connection->invalidateAccessHandle(identifier);
        });
    }
}

void WebFileSystemStorageConnection::requestNewCapacityForSyncAccessHandle(CyberCore::FileSystemHandleIdentifier identifier, CyberCore::FileSystemSyncAccessHandleIdentifier accessHandleIdentifier, uint64_t newCapacity, RequestCapacityCallback&& completionHandler)
{
    if (!m_connection)
        return completionHandler(std::nullopt);

    m_connection->sendWithAsyncReply(Messages::NetworkStorageManager::RequestNewCapacityForSyncAccessHandle(identifier, accessHandleIdentifier, newCapacity), WTFMove(completionHandler));
}

} // namespace CyberKit
