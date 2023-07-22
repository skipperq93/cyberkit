/*
 * Copyright (C) 2010-2018 Apple Inc. All rights reserved.
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

namespace CyberKit {

class WebPlatformStrategies : public CyberCore::PlatformStrategies, private CyberCore::PasteboardStrategy {
    friend NeverDestroyed<WebPlatformStrategies>;
public:
    static void initialize();
    
private:
    WebPlatformStrategies();
    
    // CyberCore::PlatformStrategies
    CyberCore::LoaderStrategy* createLoaderStrategy() override;
    CyberCore::PasteboardStrategy* createPasteboardStrategy() override;
    CyberCore::BlobRegistry* createBlobRegistry() override;

    // CyberCore::PasteboardStrategy
#if PLATFORM(IOS_FAMILY)
    void writeToPasteboard(const CyberCore::PasteboardWebContent&, const String& pasteboardName) override;
    void writeToPasteboard(const CyberCore:: PasteboardURL&, const String& pasteboardName) override;
    void writeToPasteboard(const CyberCore::PasteboardImage&, const String& pasteboardName) override;
    void writeToPasteboard(const String& pasteboardType, const String&, const String& pasteboardName) override;
    void updateSupportedTypeIdentifiers(const Vector<String>& identifiers, const String& pasteboardName) override;
#endif
#if PLATFORM(COCOA)
    int getNumberOfFiles(const String& pasteboardName) override;
    void getTypes(Vector<String>& types, const String& pasteboardName) override;
    RefPtr<CyberCore::SharedBuffer> bufferForType(const String& pasteboardType, const String& pasteboardName) override;
    void getPathnamesForType(Vector<String>& pathnames, const String& pasteboardType, const String& pasteboardName) override;
    String stringForType(const String& pasteboardType, const String& pasteboardName) override;
    Vector<String> allStringsForType(const String& pasteboardType, const String& pasteboardName) override;
    int64_t changeCount(const String& pasteboardName) override;
    String uniqueName() override;
    CyberCore::Color color(const String& pasteboardName) override;
    URL url(const String& pasteboardName) override;

    int64_t addTypes(const Vector<String>& pasteboardTypes, const String& pasteboardName) override;
    int64_t setTypes(const Vector<String>& pasteboardTypes, const String& pasteboardName) override;
    int64_t setBufferForType(CyberCore::SharedBuffer*, const String& pasteboardType, const String& pasteboardName) override;
    int64_t setURL(const CyberCore::PasteboardURL&, const String& pasteboardName) override;
    int64_t setColor(const CyberCore::Color&, const String& pasteboardName) override;
    int64_t setStringForType(const String&, const String& pasteboardType, const String& pasteboardName) override;
#endif
#if PLATFORM(GTK)
    void writeToClipboard(const String& pasteboardName, const CyberCore::SelectionData&) override;
    Ref<CyberCore::SelectionData> readFromClipboard(const String& pasteboardName) override;
#endif
#if USE(LIBWPE)
    void getTypes(Vector<String>& types) override;
    void writeToPasteboard(const CyberCore::PasteboardWebContent&) override;
    void writeToPasteboard(const String& pasteboardType, const String&) override;
#endif

    String readStringFromPasteboard(size_t index, const String& pasteboardType, const String& pasteboardName) override;
    RefPtr<CyberCore::SharedBuffer> readBufferFromPasteboard(size_t index, const String& pasteboardType, const String& pasteboardName) override;
    URL readURLFromPasteboard(size_t index, const String& pasteboardName, String& title) override;
    int getPasteboardItemsCount(const String& pasteboardName) override;
    Optional<CyberCore::PasteboardItemInfo> informationForItemAtIndex(size_t index, const String& pasteboardName, int64_t changeCount) override;
    Optional<Vector<CyberCore::PasteboardItemInfo>> allPasteboardItemInfo(const String& pasteboardName, int64_t changeCount) override;
    Vector<String> typesSafeForDOMToReadAndWrite(const String& pasteboardName, const String& origin) override;
    int64_t writeCustomData(const Vector<CyberCore::PasteboardCustomData>&, const String&) override;
};

} // namespace CyberKit
