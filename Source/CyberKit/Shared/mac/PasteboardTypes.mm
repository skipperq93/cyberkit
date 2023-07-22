/*
 * Copyright (C) 2011 Apple Inc. All rights reserved.
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

#import "config.h"
#import "PasteboardTypes.h"

#import <CyberCore/LegacyNSPasteboardTypes.h>

#if PLATFORM(MAC)

namespace CyberKit {

NSString * const PasteboardTypes::WebArchivePboardType = @"Apple Web Archive pasteboard type";
NSString * const PasteboardTypes::WebURLsWithTitlesPboardType = @"WebURLsWithTitlesPboardType";
NSString * const PasteboardTypes::WebURLPboardType = @"public.url";
NSString * const PasteboardTypes::WebURLNamePboardType = @"public.url-name";
NSString * const PasteboardTypes::WebDummyPboardType = @"Apple CyberKit dummy pasteboard type";

static inline NSArray *retain(NSArray *array)
{
    CFRetain(array);
    return array;
}
    
NSArray* PasteboardTypes::forEditing()
{
    static NSArray *types = retain([NSArray arrayWithObjects:WebArchivePboardType, (__bridge NSString *)kUTTypeWebArchive, CyberCore::legacyHTMLPasteboardType(), CyberCore::legacyFilenamesPasteboardType(), CyberCore::legacyTIFFPasteboardType(), CyberCore::legacyPDFPasteboardType(),
        CyberCore::legacyURLPasteboardType(), CyberCore::legacyRTFDPasteboardType(), CyberCore::legacyRTFPasteboardType(), CyberCore::legacyStringPasteboardType(), CyberCore::legacyColorPasteboardType(), (__bridge NSString *)kUTTypePNG, nil]);
    return types;
}

NSArray* PasteboardTypes::forURL()
{
    static NSArray *types = retain([NSArray arrayWithObjects:WebURLsWithTitlesPboardType, CyberCore::legacyURLPasteboardType(), WebURLPboardType,  WebURLNamePboardType, CyberCore::legacyStringPasteboardType(), CyberCore::legacyFilenamesPasteboardType(), CyberCore::legacyFilesPromisePasteboardType(), nil]);
    return types;
}

NSArray* PasteboardTypes::forImages()
{
    static NSArray *types = retain([NSArray arrayWithObjects:CyberCore::legacyTIFFPasteboardType(), WebURLsWithTitlesPboardType, CyberCore::legacyURLPasteboardType(), WebURLPboardType, WebURLNamePboardType, CyberCore::legacyStringPasteboardType(), nil]);
    return types;
}

NSArray* PasteboardTypes::forImagesWithArchive()
{
    static NSArray *types = retain([NSArray arrayWithObjects:CyberCore::legacyTIFFPasteboardType(), WebURLsWithTitlesPboardType, CyberCore::legacyURLPasteboardType(), WebURLPboardType, WebURLNamePboardType, CyberCore::legacyStringPasteboardType(), CyberCore::legacyRTFDPasteboardType(), WebArchivePboardType, nil]);
    return types;
}

NSArray* PasteboardTypes::forSelection()
{
    static NSArray *types = retain([NSArray arrayWithObjects:WebArchivePboardType, (__bridge NSString *)kUTTypeWebArchive, CyberCore::legacyRTFDPasteboardType(), CyberCore::legacyRTFPasteboardType(), CyberCore::legacyStringPasteboardType(), nil]);
    return types;
}
    
} // namespace CyberKit

#endif // PLATFORM(MAC)
