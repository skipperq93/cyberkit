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

#include "config.h"

#include <CyberCore/PlatformCAAnimation.h>

namespace TestWebKitAPI {

TEST(PlatformCAAnimation, makeKeyPath)
{
    auto translate = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Translate);
    EXPECT_STREQ(translate.ascii().data(), "transform");

    auto scale = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Scale);
    EXPECT_STREQ(scale.ascii().data(), "transform");

    auto rotate = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Rotate);
    EXPECT_STREQ(rotate.ascii().data(), "transform");

    auto transform = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Transform);
    EXPECT_STREQ(transform.ascii().data(), "transform");

    auto opacity = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Opacity);
    EXPECT_STREQ(opacity.ascii().data(), "opacity");

    auto backgroundColor = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::BackgroundColor);
    EXPECT_STREQ(backgroundColor.ascii().data(), "backgroundColor");

    auto filter = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Filter, CyberCore::FilterOperation::Type::Grayscale, 2);
    EXPECT_STREQ(filter.ascii().data(), "filters.filter_2.inputAmount");

#if ENABLE(FILTERS_LEVEL_2)
    auto backdropFilter = CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::WebkitBackdropFilter);
    EXPECT_STREQ(backdropFilter.ascii().data(), "backdropFilters");
#endif
}

static void validateGeneratedKeyPath(CyberCore::AnimatedProperty animatedProperty, CyberCore::FilterOperation::Type filterOperationType = CyberCore::FilterOperation::Type::None, int index = 0)
{
    auto keyPath = CyberCore::PlatformCAAnimation::makeKeyPath(animatedProperty, filterOperationType, index);
    EXPECT_TRUE(CyberCore::PlatformCAAnimation::isValidKeyPath(keyPath));
}

TEST(PlatformCAAnimation, isValidKeyPath)
{
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::Transform);
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::Opacity);
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::BackgroundColor);
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::Filter, CyberCore::FilterOperation::Type::Grayscale, 2);
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::Filter, CyberCore::FilterOperation::Type::Sepia, 22);
#if ENABLE(FILTERS_LEVEL_2)
    validateGeneratedKeyPath(CyberCore::AnimatedProperty::WebkitBackdropFilter);
#endif

    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_"_s));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_0"_s));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_10"_s));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_0.inputAmount."_s));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_0.inputAmounts"_s));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("filters.filter_-10.inputAmount"_s));

    EXPECT_TRUE(CyberCore::PlatformCAAnimation::isValidKeyPath(emptyString(), CyberCore::PlatformCAAnimation::AnimationType::Group));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath(CyberCore::PlatformCAAnimation::makeKeyPath(CyberCore::AnimatedProperty::Translate), CyberCore::PlatformCAAnimation::AnimationType::Group));
    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath(emptyString()));

    EXPECT_FALSE(CyberCore::PlatformCAAnimation::isValidKeyPath("dealloc"_s));
}

}
