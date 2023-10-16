/*
 *  This file is part of the CyberKit open source project.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef CyberKitDOMDOMWindowPrivate_h
#define CyberKitDOMDOMWindowPrivate_h

#include <CyberCore/LocalDOMWindow.h>
#include <CyberCore/WindowProxy.h>
#include <webkitdom/CyberKitDOMDOMWindow.h>

namespace CyberKit {
CyberKitDOMDOMWindow* wrapDOMWindow(CyberCore::LocalDOMWindow*);
CyberKitDOMDOMWindow* kit(CyberCore::LocalDOMWindow*);
CyberKitDOMDOMWindow* kit(CyberCore::WindowProxy*);
CyberCore::LocalDOMWindow* core(CyberKitDOMDOMWindow*);
CyberCore::WindowProxy* toWindowProxy(CyberKitDOMDOMWindow*);
} // namespace CyberKit

#endif /* CyberKitDOMDOMWindowPrivate_h */
