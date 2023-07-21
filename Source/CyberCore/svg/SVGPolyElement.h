/*
 * Copyright (C) 2004, 2005, 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>
 * Copyright (C) 2018-2019 Apple Inc. All rights reserved.
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

#include "SVGExternalResourcesRequired.h"
#include "SVGGeometryElement.h"
#include "SVGNames.h"

namespace CyberCore {

class SVGPolyElement : public SVGGeometryElement, public SVGExternalResourcesRequired {
    WTF_MAKE_ISO_ALLOCATED(SVGPolyElement);
public:
    const SVGPointList& points() const { return m_points->currentValue(); }

    SVGPointList& points() { return m_points->baseVal(); }
    SVGPointList& animatedPoints() { return *m_points->animVal(); }

    size_t approximateMemoryCost() const override;

protected:
    SVGPolyElement(const QualifiedName&, Document&);

private:
    using PropertyRegistry = SVGPropertyOwnerRegistry<SVGPolyElement, SVGGeometryElement, SVGExternalResourcesRequired>;
    const SVGPropertyRegistry& propertyRegistry() const final { return m_propertyRegistry; }

    void parseAttribute(const QualifiedName&, const AtomString&) override; 
    void svgAttributeChanged(const QualifiedName&) override;

    bool isValid() const override { return SVGTests::isValid(); }
    bool supportsMarkers() const override { return true; }

    PropertyRegistry m_propertyRegistry { *this };
    Ref<SVGAnimatedPointList> m_points { SVGAnimatedPointList::create(this) };
};

} // namespace CyberCore

SPECIALIZE_TYPE_TRAITS_BEGIN(CyberCore::SVGPolyElement)
    static bool isType(const CyberCore::SVGElement& element) { return element.hasTagName(CyberCore::SVGNames::polygonTag) || element.hasTagName(CyberCore::SVGNames::polylineTag); }
    static bool isType(const CyberCore::Node& node) { return is<CyberCore::SVGElement>(node) && isType(downcast<CyberCore::SVGElement>(node)); }
SPECIALIZE_TYPE_TRAITS_END()
