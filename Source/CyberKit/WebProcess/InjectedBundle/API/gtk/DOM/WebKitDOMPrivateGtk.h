/*
 *  Copyright (C) 1999-2001 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2008 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Samuel Weinig <sam@webkit.org>
 *  Copyright (C) 2008 Luke Kenneth Casson Leighton <lkcl@lkcl.net>
 *  Copyright (C) 2008 Martin Soto <soto@freedesktop.org>
 *  Copyright (C) 2009-2013 Igalia S.L.
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

#include "webkitdomdefines.h"

#define WEBKIT_WARN_FEATURE_NOT_PRESENT(Feature) g_warning("CyberKitGTK+ was not compiled with support for " Feature);

namespace CyberCore {
class Node;
class Event;
class StyleSheet;
class HTMLCollection;
class EventTarget;
class Blob;
} // namespace CyberCore

namespace CyberKit {
CyberKitDOMNode* wrapNodeGtk(CyberCore::Node*);
CyberKitDOMEvent* wrap(CyberCore::Event*);
CyberKitDOMStyleSheet* wrap(CyberCore::StyleSheet*);
CyberKitDOMHTMLCollection* wrap(CyberCore::HTMLCollection*);
CyberKitDOMEventTarget* wrap(CyberCore::EventTarget*);
CyberKitDOMBlob* wrap(CyberCore::Blob*);
} // namespace CyberKit
