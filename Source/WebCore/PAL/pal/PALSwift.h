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

#pragma once

#include <cstdint>
#include <wtf/Vector.h>

namespace Cpp {

using VectorUInt8 = WTF::Vector<uint8_t>;
using SpanConstUInt8 = std::span<const uint8_t>;
using OptionalVectorUInt8 = std::optional<WTF::Vector<uint8_t>>;

enum class ErrorCodes: int {
    Success = 0,
    WrongTagSize,
    EncryptionFailed,
    EncryptionResultNil,
    InvalidArgument,
    TooBigArguments,
    DecryptionFailed,
    HashingFailed,
    PublicKeyProvidedToSign,
    FailedToSign,
    FailedToVerify,
    PrivateKeyProvidedForVerification,
    FailedToImport,
    FailedToDerive,
    FailedToExport,
    DefaultValue,
    UnsupportedAlgorithm,
};
struct CryptoOperationReturnValue {
    ErrorCodes errorCode = ErrorCodes::DefaultValue;
    VectorUInt8 result;
};

} // Cpp

#ifndef __swift__
#include "PALSwift-Generated.h"
#endif

#if PLATFORM(IOS) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160000
extern "C" {
typedef enum {
  // POINT_CONVERSION_COMPRESSED indicates that the point is encoded as z||x,
  // where the octet z specifies which solution of the quadratic equation y
  // is.
  POINT_CONVERSION_COMPRESSED = 2,

  // POINT_CONVERSION_UNCOMPRESSED indicates that the point is encoded as
  // z||x||y, where z is the octet 0x04.
  POINT_CONVERSION_UNCOMPRESSED = 4,

  // POINT_CONVERSION_HYBRID indicates that the point is encoded as z||x||y,
  // where z specifies which solution of the quadratic equation y is. This is
  // not supported by the code and has never been observed in use.
  //
  // TODO(agl): remove once node.js no longer references this.
  POINT_CONVERSION_HYBRID = 6,
} point_conversion_form_t;
typedef struct ec_group_st EC_GROUP;
typedef struct ec_key_st EC_KEY;
typedef struct ec_point_st EC_POINT;
typedef struct bignum_st BIGNUM;
typedef struct bignum_ctx BN_CTX;

extern int __attribute__((weak)) EC_KEY_set_group(EC_KEY *key, const EC_GROUP *group);
extern EC_POINT * __attribute__((weak)) EC_POINT_new(const EC_GROUP *group);
extern void __attribute__((weak)) EC_POINT_free(EC_POINT *point);
extern EC_KEY * __attribute__((weak)) EC_KEY_new(void);
extern int __attribute__((weak)) EC_POINT_set_compressed_coordinates_GFp(const EC_GROUP *group,
                                                                  EC_POINT *point,
                                                                  const BIGNUM *x,
                                                                  int y_bit,
                                                                  BN_CTX *ctx);
extern int __attribute__((weak)) EC_KEY_set_public_key(EC_KEY *key, const EC_POINT *pub_key);
extern const EC_POINT * __attribute__((weak)) EC_KEY_get0_public_key(const EC_KEY *key);
extern size_t __attribute__((weak)) EC_POINT_point2oct(const EC_GROUP *group,
                                                const EC_POINT *point,
                                                point_conversion_form_t form,
                                                uint8_t *buf,
                                                size_t max_out,
                                                BN_CTX *ctx);

extern BIGNUM * __attribute__((weak)) BN_bin2bn(const uint8_t *in, size_t len, BIGNUM *ret);
extern BN_CTX * __attribute__((weak)) BN_CTX_new(void);

extern EC_GROUP * __attribute__((weak)) EC_group_p256(void);
extern EC_GROUP * __attribute__((weak)) EC_group_p384(void);
extern EC_GROUP * __attribute__((weak)) EC_group_p521(void);

class ArbitraryInteger {
    BIGNUM *space;
    
public:
    
    ArbitraryInteger() = delete;
    
    ArbitraryInteger(const uint8_t *in, size_t len) {
        BN_bin2bn(in, len, space);
    }
    
    BIGNUM *getInteger() {
        return space;
    }
};

ArbitraryInteger* ArbitraryInteger_create(const uint8_t *in, size_t len) {
    return new ArbitraryInteger(in, len);
}
}

// Returns x963Representation
Cpp::SpanConstUInt8 getx963Representation(EC_KEY *key, EC_GROUP *group) {
    const EC_POINT *p = EC_KEY_get0_public_key(key);
    uint8_t *buf;
    size_t len = EC_POINT_point2oct(EC_group_p256(), p, POINT_CONVERSION_UNCOMPRESSED, buf, 64, BN_CTX_new());
    std::span<const uint8_t> span(buf, len);
    return span;
}
#endif
