/*
 * Copyright (C) 2012 Igalia S.L.
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

#include "WebContextMenuItemGlib.h"
#include "CyberKitContextMenu.h"

#if PLATFORM(GTK)
#include <CyberCore/GRefPtrGtk.h>
#include <CyberCore/GUniquePtrGtk.h>
#endif

CyberKitContextMenu* webkitContextMenuCreate(const Vector<CyberKit::WebContextMenuItemData>&);
void webkitContextMenuPopulate(CyberKitContextMenu*, Vector<CyberKit::WebContextMenuItemGlib>&);
void webkitContextMenuPopulate(CyberKitContextMenu*, Vector<CyberKit::WebContextMenuItemData>&);
void webkitContextMenuSetParentItem(CyberKitContextMenu*, CyberKitContextMenuItem*);
CyberKitContextMenuItem* webkitContextMenuGetParentItem(CyberKitContextMenu*);
#if PLATFORM(GTK)
#if USE(GTK4)
void webkitContextMenuSetEvent(CyberKitContextMenu*, GRefPtr<GdkEvent>&&);
#else
void webkitContextMenuSetEvent(CyberKitContextMenu*, GUniquePtr<GdkEvent>&&);
#endif
#endif
