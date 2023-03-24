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

#include "APIDownloadClient.h"
#include "DownloadProxy.h"
#include "CyberKitDownload.h"
#include <CyberCore/ResourceError.h>
#include <CyberCore/ResourceRequest.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/text/WTFString.h>

GRefPtr<CyberKitDownload> webkitDownloadCreate(CyberKit::DownloadProxy&, CyberKitWebView* = nullptr);
void webkitDownloadStarted(CyberKitDownload*);
bool webkitDownloadIsCancelled(CyberKitDownload*);
void webkitDownloadSetResponse(CyberKitDownload*, CyberKitURIResponse*);
void webkitDownloadNotifyProgress(CyberKitDownload*, guint64 bytesReceived);
void webkitDownloadFailed(CyberKitDownload*, const CyberCore::ResourceError&);
void webkitDownloadCancelled(CyberKitDownload*);
void webkitDownloadFinished(CyberKitDownload*);
void webkitDownloadDecideDestinationWithSuggestedFilename(CyberKitDownload*, CString&& suggestedFilename, CompletionHandler<void(CyberKit::AllowOverwrite, WTF::String)>&&);
void webkitDownloadDestinationCreated(CyberKitDownload*, const String& destinationPath);
