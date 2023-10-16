/*
 * Copyright (C) 2016-2020 Apple Inc. All rights reserved.
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

#include <CyberCore/ContextMenuItem.h>

#if ENABLE(CONTEXT_MENUS)

namespace TestCyberKitAPI {

TEST(CyberCore, ContextMenuAction_IsValidEnum)
{
    EXPECT_FALSE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemTagNoAction - 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemTagNoAction));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemTagNoAction + 1));

    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastNonCustomTag - 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastNonCustomTag));
    EXPECT_FALSE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastNonCustomTag + 1));

    EXPECT_FALSE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseCustomTag - 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseCustomTag));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseCustomTag + 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastCustomTag - 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastCustomTag));
    EXPECT_FALSE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemLastCustomTag + 1));

    EXPECT_FALSE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseApplicationTag - 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseApplicationTag));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(CyberCore::ContextMenuAction::ContextMenuItemBaseApplicationTag + 1));
    EXPECT_TRUE(WTF::isValidEnum<CyberCore::ContextMenuAction>(std::numeric_limits<int>::max()));
}

} // namespace TestCyberKitAPI

#endif // ENABLE(CONTEXT_MENUS)
