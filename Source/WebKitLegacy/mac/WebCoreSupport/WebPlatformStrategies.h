/*
 * Copyright (C) 2010-2017 Apple Inc. All rights reserved.
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

#include <CyberCore/LoaderStrategy.h>
#include <CyberCore/PasteboardStrategy.h>
#include <CyberCore/PlatformStrategies.h>

class PasteboardCustomData;
struct PasteboardImage;
struct PasteboardWebContent;

class WebPlatformStrategies : public CyberCore::PlatformStrategies, private CyberCore::PasteboardStrategy {
public:
    static void initializeIfNecessary();
    
private:
    WebPlatformStrategies();
    
    // CyberCore::PlatformStrategies
    CyberCore::LoaderStrategy* createLoaderStrategy() override;
    CyberCore::PasteboardStrategy* createPasteboardStrategy() override;
    CyberCore::MediaStrategy* createMediaStrategy() override;
    CyberCore::BlobRegistry* createBlobRegistry() override;

    // CyberCore::PasteboardStrategy
#if PLATFORM(IOS_FAMILY)
    void writeToPasteboard(const CyberCore::PasteboardURL&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void writeToPasteboard(const CyberCore::PasteboardWebContent&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void writeToPasteboard(const CyberCore::PasteboardImage&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void writeToPasteboard(const String& pasteboardType, const String&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void updateSupportedTypeIdentifiers(const Vector<String>& identifiers, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
#endif
    String readStringFromPasteboard(size_t index, const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    RefPtr<CyberCore::SharedBuffer> readBufferFromPasteboard(std::optional<size_t> index, const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    URL readURLFromPasteboard(size_t index, const String& pasteboardName, String& title, const CyberCore::PasteboardContext*) override;
    int getPasteboardItemsCount(const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    std::optional<CyberCore::PasteboardItemInfo> informationForItemAtIndex(size_t index, const String& pasteboardName, int64_t changeCount, const CyberCore::PasteboardContext*) override;
    std::optional<Vector<CyberCore::PasteboardItemInfo>> allPasteboardItemInfo(const String& pasteboardName, int64_t changeCount, const CyberCore::PasteboardContext*) override;
    int getNumberOfFiles(const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void getTypes(Vector<String>& types, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    RefPtr<CyberCore::SharedBuffer> bufferForType(const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    void getPathnamesForType(Vector<String>& pathnames, const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    String stringForType(const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    Vector<String> allStringsForType(const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t changeCount(const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    CyberCore::Color color(const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    URL url(const String& pasteboardName, const CyberCore::PasteboardContext*) override;

    int64_t writeCustomData(const Vector<CyberCore::PasteboardCustomData>&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    Vector<String> typesSafeForDOMToReadAndWrite(const String& pasteboardName, const String& origin, const CyberCore::PasteboardContext*) override;

    int64_t addTypes(const Vector<String>& pasteboardTypes, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t setTypes(const Vector<String>& pasteboardTypes, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t setBufferForType(CyberCore::SharedBuffer*, const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t setURL(const CyberCore::PasteboardURL&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t setColor(const CyberCore::Color&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    int64_t setStringForType(const String&, const String& pasteboardType, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    bool containsStringSafeForDOMToReadForType(const String&, const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    bool containsURLStringSuitableForLoading(const String& pasteboardName, const CyberCore::PasteboardContext*) override;
    String urlStringSuitableForLoading(const String& pasteboardName, String& title, const CyberCore::PasteboardContext*) override;
};

