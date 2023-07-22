/*
 * Copyright (C) 2007, 2014-2015 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer. 
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution. 
 * 3.  Neither the name of Apple Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "CyberKitDLL.h"
#include "CyberKitStatistics.h"

#include "CyberKitStatisticsPrivate.h"
#include <CyberCore/BString.h>
#include <wtf/text/StringBuilder.h>

using namespace CyberCore;

int WebViewCount;
int WebDataSourceCount;
int WebFrameCount;
int WebHTMLRepresentationCount;
int WebFrameViewCount;

// CyberKitStatistics ---------------------------------------------------------------------------

CyberKitStatistics::CyberKitStatistics()
{
    gClassCount++;
    gClassNameCount().add("CyberKitStatistics");
}

CyberKitStatistics::~CyberKitStatistics()
{
    gClassCount--;
    gClassNameCount().remove("CyberKitStatistics");
}

CyberKitStatistics* CyberKitStatistics::createInstance()
{
    CyberKitStatistics* instance = new CyberKitStatistics();
    instance->AddRef();
    return instance;
}

// IUnknown -------------------------------------------------------------------

HRESULT CyberKitStatistics::QueryInterface(_In_ REFIID riid, _COM_Outptr_ void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;
    *ppvObject = nullptr;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<CyberKitStatistics*>(this);
    else if (IsEqualGUID(riid, IID_ICyberKitStatistics))
        *ppvObject = static_cast<CyberKitStatistics*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG CyberKitStatistics::AddRef()
{
    return ++m_refCount;
}

ULONG CyberKitStatistics::Release()
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

// ICyberKitStatistics ------------------------------------------------------------------------------

HRESULT CyberKitStatistics::webViewCount(_Out_ int* count)
{
    if (!count)
        return E_POINTER;
    *count = WebViewCount;
    return S_OK;
}

HRESULT CyberKitStatistics::frameCount(_Out_ int* count)
{
    if (!count)
        return E_POINTER;
    *count = WebFrameCount;
    return S_OK;
}

HRESULT CyberKitStatistics::dataSourceCount(_Out_ int* count)
{
    if (!count)
        return E_POINTER;
    *count = WebDataSourceCount;
    return S_OK;
}

HRESULT CyberKitStatistics::viewCount(_Out_ int* count)
{
    if (!count)
        return E_POINTER;
    *count = WebFrameViewCount;
    return S_OK;
}

HRESULT CyberKitStatistics::HTMLRepresentationCount(_Out_ int* count)
{
    if (!count)
        return E_POINTER;
    *count = WebHTMLRepresentationCount;
    return S_OK;
}

HRESULT CyberKitStatistics::comClassCount(_Out_ int *classCount)
{
    if (!classCount)
        return E_POINTER;
    *classCount = gClassCount;
    return S_OK;
}

HRESULT CyberKitStatistics::comClassNameCounts(__deref_out_opt BSTR* output)
{
    if (!output)
        return E_POINTER;

    StringBuilder builder;
    for (auto& slot : gClassNameCount()) {
        builder.appendNumber(slot.value);
        builder.append('\t');
        builder.append(slot.key);
        builder.append('\n');
    }
    *output = BString(builder.toString()).release();
    return S_OK;
}
