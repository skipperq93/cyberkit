/*
 *  Copyright (C) 2009, 2010 Sebastian Dröge <sebastian.droege@collabora.co.uk>
 *  Copyright (C) 2013 Collabora Ltd.
 *  Copyright (C) 2013 Orange
 *  Copyright (C) 2014, 2015 Sebastian Dröge <sebastian@centricular.com>
 *  Copyright (C) 2015, 2016, 2018, 2019, 2020, 2021 Metrological Group B.V.
 *  Copyright (C) 2015, 2016, 2018, 2019, 2020, 2021 Igalia, S.L
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

#if ENABLE(VIDEO) && ENABLE(MEDIA_SOURCE) && USE(GSTREAMER)

#include "GStreamerCommon.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "MediaSourcePrivate.h"
#include "SourceBufferPrivate.h"
#include "SourceBufferPrivateClient.h"
#include "SourceBufferPrivateGStreamer.h"

namespace CyberCore {

class MediaPlayerPrivateGStreamerMSE;

} // namespace CyberCore

G_BEGIN_DECLS

#define WEBKIT_TYPE_MEDIA_SRC            (webkit_media_src_get_type ())
#define WEBKIT_MEDIA_SRC(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), WEBKIT_TYPE_MEDIA_SRC, CyberKitMediaSrc))
#define WEBKIT_MEDIA_SRC_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), WEBKIT_TYPE_MEDIA_SRC, CyberKitMediaSrcClass))
#define WEBKIT_IS_MEDIA_SRC(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WEBKIT_TYPE_MEDIA_SRC))
#define WEBKIT_IS_MEDIA_SRC_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), WEBKIT_TYPE_MEDIA_SRC))

struct CyberKitMediaSrcPrivate;

struct CyberKitMediaSrc {
    GstElement parent;

    CyberKitMediaSrcPrivate *priv;
};

struct CyberKitMediaSrcClass {
    GstElementClass parentClass;
};

GType webkit_media_src_get_type(void);

void webKitMediaSrcEmitStreams(CyberKitMediaSrc* source, const Vector<RefPtr<CyberCore::MediaSourceTrackGStreamer>>& tracks);

void webKitMediaSrcFlush(CyberKitMediaSrc*, const AtomString& streamName);

G_END_DECLS

namespace WTF {
template<> GRefPtr<CyberKitMediaSrc> adoptGRef(CyberKitMediaSrc* ptr);
template<> CyberKitMediaSrc* refGPtr<CyberKitMediaSrc>(CyberKitMediaSrc* ptr);
template<> void derefGPtr<CyberKitMediaSrc>(CyberKitMediaSrc* ptr);
} // namespace WTF

#endif // USE(GSTREAMER)
