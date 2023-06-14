/*
 * Copyright (C) 2012 Igalia S.L.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2,1 of the License, or (at your option) any later version.
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

#ifdef __WEBKIT_H__
#error "Headers <wpe/webkit.h> and <wpe/webkit-web-extension.h> cannot be included together."
#endif

#ifndef __WEBKIT_WEB_EXTENSION_H__
#define __WEBKIT_WEB_EXTENSION_H__

#define __WEBKIT_WEB_EXTENSION_H_INSIDE__

#include <wpe/CyberKitConsoleMessage.h>
#include <wpe/CyberKitContextMenu.h>
#include <wpe/CyberKitContextMenuActions.h>
#include <wpe/CyberKitContextMenuItem.h>
#include <wpe/CyberKitFrame.h>
#include <wpe/CyberKitScriptWorld.h>
#include <wpe/CyberKitURIRequest.h>
#include <wpe/CyberKitURIResponse.h>
#include <wpe/CyberKitWebEditor.h>
#include <wpe/CyberKitWebExtension.h>
#include <wpe/CyberKitWebHitTestResult.h>
#include <wpe/CyberKitWebPage.h>
#include <wpe/CyberKitWebProcessEnumTypes.h>

#undef __WEBKIT_WEB_EXTENSION_H_INSIDE__

#endif
