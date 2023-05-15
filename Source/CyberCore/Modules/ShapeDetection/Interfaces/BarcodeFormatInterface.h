/*
 * Copyright (C) 2023 Apple Inc. All rights reserved.
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
#include <wtf/EnumTraits.h>

namespace CyberCore::ShapeDetection {

enum class BarcodeFormat : uint8_t {
    Aztec,
    Code_128,
    Code_39,
    Code_93,
    Codabar,
    Data_matrix,
    Ean_13,
    Ean_8,
    Itf,
    Pdf417,
    Qr_code,
    Unknown,
    Upc_a,
    Upc_e,
};

} // namespace CyberCore::ShapeDetection

namespace WTF {

template<> struct EnumTraits<CyberCore::ShapeDetection::BarcodeFormat> {
    using values = EnumValues<
        CyberCore::ShapeDetection::BarcodeFormat,
        CyberCore::ShapeDetection::BarcodeFormat::Aztec,
        CyberCore::ShapeDetection::BarcodeFormat::Code_128,
        CyberCore::ShapeDetection::BarcodeFormat::Code_39,
        CyberCore::ShapeDetection::BarcodeFormat::Code_93,
        CyberCore::ShapeDetection::BarcodeFormat::Codabar,
        CyberCore::ShapeDetection::BarcodeFormat::Data_matrix,
        CyberCore::ShapeDetection::BarcodeFormat::Ean_13,
        CyberCore::ShapeDetection::BarcodeFormat::Ean_8,
        CyberCore::ShapeDetection::BarcodeFormat::Itf,
        CyberCore::ShapeDetection::BarcodeFormat::Pdf417,
        CyberCore::ShapeDetection::BarcodeFormat::Qr_code,
        CyberCore::ShapeDetection::BarcodeFormat::Unknown,
        CyberCore::ShapeDetection::BarcodeFormat::Upc_a,
        CyberCore::ShapeDetection::BarcodeFormat::Upc_e
    >;
};

} // namespace WTF
