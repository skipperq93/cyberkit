/*
 * Copyright (C) 2014 Igalia S.L.
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

#include "config.h"
#include "ActivateFonts.h"

#include <string>
#include <vector>
#include <windows.h>

namespace WTR {

static const wchar_t* fontsEnvironmentVariable = L"WEBKIT_TESTFONTS";

static const std::wstring& fontsPath()
{
    static std::wstring path;
    static bool initialized;

    if (initialized)
        return path;
    initialized = true;

    DWORD size = GetEnvironmentVariableW(fontsEnvironmentVariable, 0, 0);
    std::vector<wchar_t> buffer(size);
    if (GetEnvironmentVariableW(fontsEnvironmentVariable, buffer.data(), buffer.size())) {
        path = buffer.data();
        if (path[path.length() - 1] != '\\')
            path.append(L"\\");
    }

    return path;
}

void activateFonts()
{
    static const wchar_t* fontFilenames[] = {
        L"AHEM____.TTF",
        L"CyberKitWeightWatcher100.ttf",
        L"CyberKitWeightWatcher200.ttf",
        L"CyberKitWeightWatcher300.ttf",
        L"CyberKitWeightWatcher400.ttf",
        L"CyberKitWeightWatcher500.ttf",
        L"CyberKitWeightWatcher600.ttf",
        L"CyberKitWeightWatcher700.ttf",
        L"CyberKitWeightWatcher800.ttf",
        L"CyberKitWeightWatcher900.ttf",
        0
    };

    std::wstring baseFontPath = fontsPath();
    if (!baseFontPath.empty()) {
        for (size_t i = 0; fontFilenames[i]; ++i) {
            std::wstring fontPath = baseFontPath + fontFilenames[i];
            AddFontResourceExW(fontPath.c_str(), FR_PRIVATE, nullptr);
        }
    }
}

void installFakeHelvetica(WKStringRef)
{
}

void uninstallFakeHelvetica()
{
}

} // namespace WTR
