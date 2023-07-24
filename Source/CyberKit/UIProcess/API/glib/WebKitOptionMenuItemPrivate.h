/*
 * Copyright (C) 2017 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#pragma once

#include "CyberKitOptionMenuItem.h"
#include "WebPopupItem.h"
#include <wtf/text/CString.h>

struct _CyberKitOptionMenuItem {
    _CyberKitOptionMenuItem() = default;

    _CyberKitOptionMenuItem(const CyberKit::WebPopupItem& item)
        : label(item.m_text.stripWhiteSpace().utf8())
        , isGroupLabel(item.m_isLabel)
        , isGroupChild(item.m_text.startsWith("    "))
        , isEnabled(item.m_isEnabled)
    {
        if (!item.m_toolTip.isEmpty())
            tooltip = item.m_toolTip.utf8();
    }

    explicit _CyberKitOptionMenuItem(_CyberKitOptionMenuItem* other)
        : label(other->label)
        , tooltip(other->tooltip)
        , isGroupLabel(other->isGroupLabel)
        , isGroupChild(other->isGroupChild)
        , isEnabled(other->isEnabled)
        , isSelected(other->isSelected)
    {
    }

    CString label;
    CString tooltip;
    bool isGroupLabel { false };
    bool isGroupChild { false };
    bool isEnabled { true };
    bool isSelected { false };
};
