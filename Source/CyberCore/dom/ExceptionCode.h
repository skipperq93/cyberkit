/*
 *  Copyright (C) 2006-2020 Apple Inc. All rights reserved.
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

#include <wtf/EnumTraits.h>

namespace CyberCore {

enum ExceptionCode {
    // DOMException error names (https://webidl.spec.whatwg.org/#idl-DOMException-error-names).
    // Those need to be kept in sync with the array in DOMException.cpp.
    IndexSizeError, // Deprecated. Use RangeError instead.
    HierarchyRequestError,
    WrongDocumentError,
    InvalidCharacterError,
    NoModificationAllowedError,
    NotFoundError,
    NotSupportedError,
    InUseAttributeError,
    InvalidStateError,
    SyntaxError,
    InvalidModificationError,
    NamespaceError,
    InvalidAccessError, // Deprecated. use NotAllowedError instead.
    TypeMismatchError, // Deprecated. Use TypeError instead.
    SecurityError,
    NetworkError,
    AbortError,
    URLMismatchError,
    QuotaExceededError,
    TimeoutError,
    InvalidNodeTypeError,
    DataCloneError,
    EncodingError,
    NotReadableError,
    UnknownError,
    ConstraintError,
    DataError,
    TransactionInactiveError,
    ReadonlyError,
    VersionError,
    OperationError,
    NotAllowedError,

    // Simple exceptions (https://webidl.spec.whatwg.org/#idl-exceptions).
    RangeError,
    TypeError,
    JSSyntaxError, // Different from DOM SYNTAX_ERR.

    // Non-standard error.
    StackOverflowError,
    OutOfMemoryError,

    // Used to indicate to the bindings that a JS exception was thrown below and it should be propagated.
    ExistingExceptionError,
};

} // namespace CyberCore

namespace WTF {

template<> struct EnumTraits<CyberCore::ExceptionCode> {
    using values = EnumValues<
    CyberCore::ExceptionCode,
    CyberCore::ExceptionCode::IndexSizeError,
    CyberCore::ExceptionCode::HierarchyRequestError,
    CyberCore::ExceptionCode::WrongDocumentError,
    CyberCore::ExceptionCode::InvalidCharacterError,
    CyberCore::ExceptionCode::NoModificationAllowedError,
    CyberCore::ExceptionCode::NotFoundError,
    CyberCore::ExceptionCode::NotSupportedError,
    CyberCore::ExceptionCode::InUseAttributeError,
    CyberCore::ExceptionCode::InvalidStateError,
    CyberCore::ExceptionCode::SyntaxError,
    CyberCore::ExceptionCode::InvalidModificationError,
    CyberCore::ExceptionCode::NamespaceError,
    CyberCore::ExceptionCode::InvalidAccessError,
    CyberCore::ExceptionCode::TypeMismatchError,
    CyberCore::ExceptionCode::SecurityError,
    CyberCore::ExceptionCode::NetworkError,
    CyberCore::ExceptionCode::AbortError,
    CyberCore::ExceptionCode::URLMismatchError,
    CyberCore::ExceptionCode::QuotaExceededError,
    CyberCore::ExceptionCode::TimeoutError,
    CyberCore::ExceptionCode::InvalidNodeTypeError,
    CyberCore::ExceptionCode::DataCloneError,
    CyberCore::ExceptionCode::EncodingError,
    CyberCore::ExceptionCode::NotReadableError,
    CyberCore::ExceptionCode::UnknownError,
    CyberCore::ExceptionCode::ConstraintError,
    CyberCore::ExceptionCode::DataError,
    CyberCore::ExceptionCode::TransactionInactiveError,
    CyberCore::ExceptionCode::ReadonlyError,
    CyberCore::ExceptionCode::VersionError,
    CyberCore::ExceptionCode::OperationError,
    CyberCore::ExceptionCode::NotAllowedError,
    CyberCore::ExceptionCode::RangeError,
    CyberCore::ExceptionCode::TypeError,
    CyberCore::ExceptionCode::JSSyntaxError,
    CyberCore::ExceptionCode::StackOverflowError,
    CyberCore::ExceptionCode::ExistingExceptionError
    >;
};

} // namespace WTF
