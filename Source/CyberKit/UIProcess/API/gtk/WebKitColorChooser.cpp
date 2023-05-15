/*
 * Copyright (C) 2015 Igalia S.L.
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

#include "config.h"
#include "CyberKitColorChooser.h"

#include "CyberKitColorChooserRequestPrivate.h"
#include "CyberKitWebViewPrivate.h"
#include <CyberCore/Color.h>
#include <CyberCore/IntRect.h>

namespace CyberKit {
using namespace CyberCore;

Ref<CyberKitColorChooser> CyberKitColorChooser::create(WebPageProxy& page, const CyberCore::Color& initialColor, const CyberCore::IntRect& rect)
{
    return adoptRef(*new CyberKitColorChooser(page, initialColor, rect));
}

CyberKitColorChooser::CyberKitColorChooser(WebPageProxy& page, const Color& initialColor, const IntRect& rect)
    : WebColorPickerGtk(page, initialColor, rect)
    , m_elementRect(rect)
{
}

CyberKitColorChooser::~CyberKitColorChooser()
{
    endPicker();
}

void CyberKitColorChooser::endPicker()
{
    if (!m_request) {
        WebColorPickerGtk::endPicker();
        return;
    }

    webkit_color_chooser_request_finish(m_request.get());
}

void CyberKitColorChooser::colorChooserRequestFinished(CyberKitColorChooserRequest*, CyberKitColorChooser* colorChooser)
{
    colorChooser->m_request = nullptr;
}

void CyberKitColorChooser::colorChooserRequestRGBAChanged(CyberKitColorChooserRequest* request, GParamSpec*, CyberKitColorChooser* colorChooser)
{
    GdkRGBA rgba;
    webkit_color_chooser_request_get_rgba(request, &rgba);
    colorChooser->didChooseColor(rgba);
}

void CyberKitColorChooser::showColorPicker(const Color& color)
{
    m_initialColor = color;
    GRefPtr<CyberKitColorChooserRequest> request = adoptGRef(webkitColorChooserRequestCreate(this));
    g_signal_connect(request.get(), "notify::rgba", G_CALLBACK(CyberKitColorChooser::colorChooserRequestRGBAChanged), this);
    g_signal_connect(request.get(), "finished", G_CALLBACK(CyberKitColorChooser::colorChooserRequestFinished), this);

    if (webkitWebViewEmitRunColorChooser(WEBKIT_WEB_VIEW(m_webView), request.get()))
        m_request = request;
    else
        WebColorPickerGtk::showColorPicker(color);
}

} // namespace CyberKit
