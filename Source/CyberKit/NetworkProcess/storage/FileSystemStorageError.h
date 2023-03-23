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

#include <CyberCore/ExceptionOr.h>
#include <wtf/EnumTraits.h>

namespace CyberKit {

enum class FileSystemStorageError : uint8_t {
    AccessHandleActive,
    BackendNotSupported,
    FileNotFound,
    InvalidModification,
    InvalidName,
    InvalidState,
    TypeMismatch,
    Unknown
};

inline CyberCore::Exception convertToException(FileSystemStorageError error)
{
    switch (error) {
    case FileSystemStorageError::AccessHandleActive:
        return CyberCore::Exception { CyberCore::InvalidStateError, "Some AccessHandle is active"_s };
    case FileSystemStorageError::BackendNotSupported:
        return CyberCore::Exception { CyberCore::NotSupportedError, "Backend does not support this operation"_s };
    case FileSystemStorageError::FileNotFound:
        return CyberCore::Exception { CyberCore::NotFoundError };
    case FileSystemStorageError::InvalidModification:
        return CyberCore::Exception { CyberCore::InvalidModificationError };
    case FileSystemStorageError::InvalidName:
        return CyberCore::Exception { CyberCore::TypeError, "Name is invalid"_s };
    case FileSystemStorageError::InvalidState:
        return CyberCore::Exception { CyberCore::InvalidStateError };
    case FileSystemStorageError::TypeMismatch:
        return CyberCore::Exception { CyberCore::TypeError };
    case FileSystemStorageError::Unknown:
        break;
    }

    return CyberCore::Exception { CyberCore::UnknownError };
}

inline CyberCore::ExceptionOr<void> convertToExceptionOr(std::optional<FileSystemStorageError> error)
{
    if (!error)
        return { };

    return convertToException(*error);
}

} // namespace CyberKit

namespace WTF {

template<> struct EnumTraits<CyberKit::FileSystemStorageError> {
    using values = EnumValues<
        CyberKit::FileSystemStorageError,
        CyberKit::FileSystemStorageError::AccessHandleActive,
        CyberKit::FileSystemStorageError::BackendNotSupported,
        CyberKit::FileSystemStorageError::FileNotFound,
        CyberKit::FileSystemStorageError::InvalidModification,
        CyberKit::FileSystemStorageError::InvalidName,
        CyberKit::FileSystemStorageError::InvalidState,
        CyberKit::FileSystemStorageError::TypeMismatch,
        CyberKit::FileSystemStorageError::Unknown
    >;
};

} // namespace WTF
