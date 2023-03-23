/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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

#if ENABLE(CONTEXT_MENUS)

#include <CyberCore/ContextMenuItem.h>
#include <wtf/text/WTFString.h>

namespace API {
class Object;
}

namespace IPC {
class Decoder;
class Encoder;
}

namespace CyberKit {

class WebContextMenuItemData {
public:
    WebContextMenuItemData();
    WebContextMenuItemData(const CyberCore::ContextMenuItem&);
    WebContextMenuItemData(CyberCore::ContextMenuItemType, CyberCore::ContextMenuAction, const String& title, bool enabled, bool checked, unsigned indentationLevel = 0);
    WebContextMenuItemData(CyberCore::ContextMenuAction, const String& title, bool enabled, const Vector<WebContextMenuItemData>& submenu, unsigned indentationLevel = 0);

    CyberCore::ContextMenuItemType type() const { return m_type; }
    CyberCore::ContextMenuAction action() const { return m_action; }
    const String& title() const { return m_title; }
    bool enabled() const { return m_enabled; }
    bool checked() const { return m_checked; }
    unsigned indentationLevel() const { return m_indentationLevel; }
    const Vector<WebContextMenuItemData>& submenu() const { return m_submenu; }
    
    CyberCore::ContextMenuItem core() const;
    
    API::Object* userData() const;
    void setUserData(API::Object*);
    
    void encode(IPC::Encoder&) const;
    static std::optional<WebContextMenuItemData> decode(IPC::Decoder&);

private:
    CyberCore::ContextMenuItemType m_type;
    CyberCore::ContextMenuAction m_action;
    String m_title;
    bool m_enabled;
    bool m_checked;
    unsigned m_indentationLevel;
    Vector<WebContextMenuItemData> m_submenu;
    RefPtr<API::Object> m_userData;
};

Vector<WebContextMenuItemData> kitItems(const Vector<CyberCore::ContextMenuItem>&);
Vector<CyberCore::ContextMenuItem> coreItems(const Vector<WebContextMenuItemData>&);

} // namespace CyberKit

#endif // ENABLE(CONTEXT_MENUS)
