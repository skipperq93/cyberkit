/*
* Copyright (C) 2024 Apple Inc. All rights reserved.
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

#if swift(>=5.9)

import Foundation
import CryptoKit

import PALSwift

enum UnsafeErrors: Error {
    case invalidLength
    case emptySpan
}

extension CryptoKit.HashFunction {
    mutating func update(data: SpanConstUInt8) {
        if data.empty() {
            self.update(data: Data.empty())
        } else {
            self.update(
                bufferPointer: UnsafeRawBufferPointer(
                    start: data.__dataUnsafe(), count: data.size()))
        }
    }
}

extension ContiguousBytes {
    public func copyToVectorUInt8() -> VectorUInt8 {
        return self.withUnsafeBytes { buf in
            let result = VectorUInt8(buf.count)
            buf.copyBytes(
                to: UnsafeMutableRawBufferPointer(
                    start: UnsafeMutableRawPointer(mutating: result.__dataUnsafe()),
                    count: result.size()), count: result.size())
            return result
        }
    }
}

extension Data {
    static let emptyData = Data()
    fileprivate static func temporaryDataFromSpan(spanNoCopy: SpanConstUInt8) -> Data {
        if spanNoCopy.empty() {
            return Data.empty()
        } else {
            return Data(
                bytesNoCopy: UnsafeMutablePointer(mutating: spanNoCopy.__dataUnsafe()),
                count: spanNoCopy.size(), deallocator: .none)
        }
    }

    // CryptoKit does not support a null pointer with zero length. We instead need to pass an empty Data. This class provides that.
    public static func empty() -> Data {
        return emptyData
    }
}

private class _WorkAroundRadar116406681 {
    // rdar://116406681
    private func forceLinkageForVectorDestructor() {
        let _ = VectorUInt8()
    }
}

extension AES.GCM {
    public static func seal(
        _ message: SpanConstUInt8, key: SpanConstUInt8, iv: SpanConstUInt8, ad: SpanConstUInt8
    ) throws -> AES.GCM.SealedBox {
        if ad.size() > 0 {
            return try AES.GCM.seal(
                Data.temporaryDataFromSpan(spanNoCopy: message),
                using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: key)),
                nonce: AES.GCM.Nonce(data: Data.temporaryDataFromSpan(spanNoCopy: iv)),
                authenticating: Data.temporaryDataFromSpan(spanNoCopy: ad))
        } else {
            return try AES.GCM.seal(
                Data.temporaryDataFromSpan(spanNoCopy: message),
                using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: key)),
                nonce: AES.GCM.Nonce(data: Data.temporaryDataFromSpan(spanNoCopy: iv))
            )
        }
    }
}

extension AES.KeyWrap {
    public static func unwrap(_ wrapped: SpanConstUInt8, using: SpanConstUInt8) throws
        -> SymmetricKey
    {
        return try AES.KeyWrap.unwrap(
            Data.temporaryDataFromSpan(spanNoCopy: wrapped),
            using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: using)))

    }
    public static func wrap(_ keyToWrap: SpanConstUInt8, using: SpanConstUInt8) throws
        -> VectorUInt8
    {
        return try AES.KeyWrap.wrap(
            SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: keyToWrap)),
            using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: using))
        ).copyToVectorUInt8()
    }
}

extension P256.Signing.ECDSASignature {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}
extension P384.Signing.ECDSASignature {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}
extension P521.Signing.ECDSASignature {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}

// The following fileprivate functions are modified from the implementation at https://github.com/apple/swift-crypto/blob/8fa345c2081cfbd4851dffff5dd5bed48efe6081/Sources/Crypto/Keys/EC/BoringSSL/NISTCurvesKeys_boring.swift#L384
#if os(iOS) && WK_IOS_BEFORE_16
fileprivate func readx963CompressedPublicNumbers(bytesPtr: UnsafeRawBufferPointer) throws -> (x: ArbitraryInteger, yBit: Bool) {
    // The x9.63 compressed public key format is a discriminator byte (0x2 or 0x3) that signals which
    // of the possible two Y values is being used, concatenated with the X point of the key.
    let yBit: Bool
    
    switch bytesPtr.first {
    case 0x03:
        yBit = true
    case 0x02:
        yBit = false
    default:
        throw CryptoKitError.incorrectKeySize // This is the same error CryptoKit throws on Apple platforms.
    }
    
    let xBytes: UnsafeRawBufferPointer = UnsafeRawBufferPointer(rebasing: bytesPtr.dropFirst())
    
    let xPointer: ArbitraryInteger = xBytes.withUnsafeBytes { (bytesPointer: UnsafeRawBufferPointer) -> ArbitraryInteger in
        ArbitraryInteger_create(bytesPointer.baseAddress, bytesPointer.count).pointee
    }
    
    return (x: xPointer, yBit: yBit)
}

fileprivate func setPublicKey(coordinateByteCount: Int, x: inout ArbitraryInteger, yBit: Bool) throws -> SpanConstUInt8 {
    var groupPtr: OpaquePointer! = nil
    switch coordinateByteCount {
    case 32:
        groupPtr = EC_group_p256()
    case 48:
        groupPtr = EC_group_p384()
    case 72:
        groupPtr = EC_group_p521()
    default:
        throw CryptoKitError.incorrectKeySize
    }
    
    guard let key = EC_KEY_new(), EC_KEY_set_group(key, groupPtr) == 1 else {
        throw CryptoKitError.unwrapFailure
    }
    
    guard let point = EC_POINT_new(groupPtr) else {
        throw CryptoKitError.unwrapFailure
    }
    defer {
        // We either error, or EC_KEY_set_public_key dups the key,
        // so we must always free.
        EC_POINT_free(point)
    }
    let rc = EC_POINT_set_compressed_coordinates_GFp(groupPtr, point, x.getInteger(), yBit ? 1 : 0, nil)

    guard rc == 1 else {
        throw CryptoKitError.unwrapFailure
    }

    guard EC_KEY_set_public_key(key, point) == 1 else {
        throw CryptoKitError.unwrapFailure
    }
    
    return getx963Representation(key, groupPtr)
}

fileprivate func NISTCurve_init_compressed(coordinateByteCount: Int, withCompressed span: SpanConstUInt8) throws -> SpanConstUInt8 {
    let bytes = Data.temporaryDataFromSpan(spanNoCopy: span)
    let length = bytes.withUnsafeBytes { $0.count }
    
    guard length == coordinateByteCount + 1 else {
        throw CryptoKitError.incorrectParameterSize
    }
    
    return try bytes.withUnsafeBytes { buffer in
        var tuple = try readx963CompressedPublicNumbers(bytesPtr: buffer)
        return try setPublicKey(coordinateByteCount: coordinateByteCount, x: &tuple.x, yBit: tuple.yBit)
    }
}
#endif

extension P256.Signing.PublicKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    init(spanCompressed: SpanConstUInt8) throws {
        if spanCompressed.empty() {
            throw UnsafeErrors.emptySpan
        }
#if os(iOS) && WK_IOS_BEFORE_16
        let spanDecompressed = try NISTCurve_init_compressed(coordinateByteCount: 32, withCompressed: spanCompressed)
        try self.init(span: spanDecompressed)
#else
        try self.init(
            compressedRepresentation: Data.temporaryDataFromSpan(spanNoCopy: spanCompressed))
#endif
    }
}

extension P384.Signing.PublicKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    init(spanCompressed: SpanConstUInt8) throws {
        if spanCompressed.empty() {
            throw UnsafeErrors.emptySpan
        }
#if os(iOS) && WK_IOS_BEFORE_16
        let spanDecompressed = try NISTCurve_init_compressed(coordinateByteCount: 48, withCompressed: spanCompressed)
        try self.init(span: spanDecompressed)
#else
        try self.init(
            compressedRepresentation: Data.temporaryDataFromSpan(spanNoCopy: spanCompressed))
#endif
    }
}

extension P521.Signing.PublicKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    init(spanCompressed: SpanConstUInt8) throws {
        if spanCompressed.empty() {
            throw UnsafeErrors.emptySpan
        }
#if os(iOS) && WK_IOS_BEFORE_16
        let spanDecompressed = try NISTCurve_init_compressed(coordinateByteCount: 72, withCompressed: spanCompressed)
        try self.init(span: spanDecompressed)
#else
        try self.init(
            compressedRepresentation: Data.temporaryDataFromSpan(spanNoCopy: spanCompressed))
#endif
    }
}

extension P256.Signing.PrivateKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}

extension P384.Signing.PrivateKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}

extension P521.Signing.PrivateKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(x963Representation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
}

extension Curve25519.Signing.PrivateKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    public func signature(span: SpanConstUInt8) throws -> VectorUInt8 {
        if span.empty() {
            return try self.signature(for: Data.empty()).copyToVectorUInt8()
        }
        return try self.signature(for: Data.temporaryDataFromSpan(spanNoCopy: span))
            .copyToVectorUInt8()
    }
}

extension Curve25519.Signing.PublicKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    public func isValidSignature(signature: SpanConstUInt8, data: SpanConstUInt8) -> Bool {
        if signature.empty() || data.empty() {
            return false
        }
        return self.isValidSignature(
            Data.temporaryDataFromSpan(spanNoCopy: signature),
            for: Data.temporaryDataFromSpan(spanNoCopy: data))
    }

}

extension Curve25519.KeyAgreement.PrivateKey {
    init(span: SpanConstUInt8) throws {
        if span.empty() {
            throw UnsafeErrors.emptySpan
        }
        try self.init(rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: span))
    }
    public func sharedSecretFromKeyAgreement(pubSpan: SpanConstUInt8) throws -> VectorUInt8 {
        if pubSpan.empty() {
            throw UnsafeErrors.emptySpan
        }
        let pub = try Curve25519.KeyAgreement.PublicKey(
            rawRepresentation: Data.temporaryDataFromSpan(spanNoCopy: pubSpan))
        return try self.sharedSecretFromKeyAgreement(with: pub).copyToVectorUInt8()
    }
}

extension CryptoKit.HMAC {
    static func authenticationCode(
        data: SpanConstUInt8,
        key: SpanConstUInt8
    ) -> VectorUInt8 {
        return self.authenticationCode(
            for: Data.temporaryDataFromSpan(spanNoCopy: data),
            using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: key))
        ).copyToVectorUInt8()
    }
    static func isValidAuthenticationCode(
        mac: SpanConstUInt8, data: SpanConstUInt8, key: SpanConstUInt8
    ) -> Bool {
        return Self.isValidAuthenticationCode(
            Data.temporaryDataFromSpan(spanNoCopy: mac),
            authenticating: Data.temporaryDataFromSpan(spanNoCopy: data),
            using: SymmetricKey(data: Data.temporaryDataFromSpan(spanNoCopy: key)))
    }
}

extension CryptoKit.HKDF {
    static func deriveKey(
        inputKeyMaterial: SpanConstUInt8, salt: SpanConstUInt8, info: SpanConstUInt8,
        outputByteCount: Int
    ) -> VectorUInt8 {
        return Self.deriveKey(
            inputKeyMaterial: SymmetricKey(
                data: Data.temporaryDataFromSpan(spanNoCopy: inputKeyMaterial)),
            salt: Data.temporaryDataFromSpan(spanNoCopy: salt),
            info: Data.temporaryDataFromSpan(spanNoCopy: info), outputByteCount: outputByteCount
        ).copyToVectorUInt8()

    }
}

#endif
