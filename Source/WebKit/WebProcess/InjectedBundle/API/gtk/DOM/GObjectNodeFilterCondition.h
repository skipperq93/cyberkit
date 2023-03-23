/*
 *  Copyright (C) 2014 Igalia S.L.
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

#include "CyberKitDOMNodeFilter.h"
#include <CyberCore/NodeFilterCondition.h>
#include <wtf/glib/GRefPtr.h>

namespace CyberCore {
class Node;
}

namespace CyberKit {

class GObjectNodeFilterCondition final : public CyberCore::NodeFilterCondition {
public:
    static Ref<GObjectNodeFilterCondition> create(CyberKitDOMNodeFilter* filter)
    {
        return adoptRef(*new GObjectNodeFilterCondition(filter));
    }

    unsigned short acceptNode(CyberCore::Node&) const override;

private:
    GObjectNodeFilterCondition(CyberKitDOMNodeFilter* filter)
        : m_filter(filter)
    {
    }
    ~GObjectNodeFilterCondition();

    GRefPtr<CyberKitDOMNodeFilter> m_filter;
};

} // namespace CyberKit
