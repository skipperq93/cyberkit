/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#include "IntPointHash.h"
#include "IntRect.h"
#include "IntSizeHash.h"

namespace WTF {

template<> struct IntHash<CyberCore::IntRect> {
    static unsigned hash(const CyberCore::IntRect& key)
    {
        return pairIntHash(DefaultHash<CyberCore::IntPoint>::hash(key.location()), DefaultHash<CyberCore::IntSize>::hash(key.size()));
    }
    static bool equal(const CyberCore::IntRect& a, const CyberCore::IntRect& b)
    {
        return DefaultHash<CyberCore::IntPoint>::equal(a.location(), b.location()) && DefaultHash<CyberCore::IntSize>::equal(a.size(), b.size());
    }
    static const bool safeToCompareToEmptyOrDeleted = true;
};
template<> struct DefaultHash<CyberCore::IntRect> : IntHash<CyberCore::IntRect> { };

template<> struct HashTraits<CyberCore::IntRect> : GenericHashTraits<CyberCore::IntRect> {
    static const bool emptyValueIsZero = true;
    static void constructDeletedValue(CyberCore::IntRect& slot) { new (NotNull, &slot) CyberCore::IntRect(-1, -1, -1, -1); }
    static bool isDeletedValue(const CyberCore::IntRect& value) { return value.x() == -1 && value.y() == -1 && value.width() == -1 && value.height() == -1; }
};

}
