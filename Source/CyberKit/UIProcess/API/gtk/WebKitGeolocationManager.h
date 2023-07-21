/*
 * Copyright (C) 2019 Igalia S.L.
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

#if !defined(__WEBKIT2_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <webkit2/webkit2.h> can be included directly."
#endif

#ifndef CyberKitGeolocationManager_h
#define CyberKitGeolocationManager_h

#include <glib-object.h>
#include <webkit2/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_GEOLOCATION_MANAGER            (webkit_geolocation_manager_get_type())
#define WEBKIT_GEOLOCATION_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_GEOLOCATION_MANAGER, CyberKitGeolocationManager))
#define WEBKIT_IS_GEOLOCATION_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_GEOLOCATION_MANAGER))
#define WEBKIT_GEOLOCATION_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_GEOLOCATION_MANAGER, CyberKitGeolocationManagerClass))
#define WEBKIT_IS_GEOLOCATION_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_GEOLOCATION_MANAGER))
#define WEBKIT_GEOLOCATION_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_GEOLOCATION_MANAGER, CyberKitGeolocationManagerClass))

#define WEBKIT_TYPE_GEOLOCATION_POSITION           (webkit_geolocation_position_get_type())

typedef struct _CyberKitGeolocationManager        CyberKitGeolocationManager;
typedef struct _CyberKitGeolocationManagerClass   CyberKitGeolocationManagerClass;
typedef struct _CyberKitGeolocationManagerPrivate CyberKitGeolocationManagerPrivate;
typedef struct _CyberKitGeolocationPosition       CyberKitGeolocationPosition;

struct _CyberKitGeolocationManager {
    GObject parent;

    /*< private >*/
    CyberKitGeolocationManagerPrivate *priv;
};

struct _CyberKitGeolocationManagerClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_geolocation_manager_get_type                 (void);

WEBKIT_API void
webkit_geolocation_manager_update_position           (CyberKitGeolocationManager  *manager,
                                                     CyberKitGeolocationPosition *position);

WEBKIT_API void
webkit_geolocation_manager_failed                    (CyberKitGeolocationManager  *manager,
                                                     const char                *error_message);

WEBKIT_API gboolean
webkit_geolocation_manager_get_enable_high_accuracy (CyberKitGeolocationManager  *manager);


WEBKIT_API GType
webkit_geolocation_position_get_type                (void);

WEBKIT_API CyberKitGeolocationPosition *
webkit_geolocation_position_new                     (double                     latitude,
                                                     double                     longitude,
                                                     double                     accuracy);

WEBKIT_API CyberKitGeolocationPosition *
webkit_geolocation_position_copy                    (CyberKitGeolocationPosition *position);

WEBKIT_API void
webkit_geolocation_position_free                    (CyberKitGeolocationPosition *position);

WEBKIT_API void
webkit_geolocation_position_set_timestamp           (CyberKitGeolocationPosition *position,
                                                     guint64                    timestamp);

WEBKIT_API void
webkit_geolocation_position_set_altitude            (CyberKitGeolocationPosition *position,
                                                     double                     altitude);

WEBKIT_API void
webkit_geolocation_position_set_altitude_accuracy   (CyberKitGeolocationPosition *position,
                                                     double                     altitude_accuracy);

WEBKIT_API void
webkit_geolocation_position_set_heading             (CyberKitGeolocationPosition *position,
                                                     double                     heading);

WEBKIT_API void
webkit_geolocation_position_set_speed               (CyberKitGeolocationPosition *position,
                                                     double                     speed);

G_END_DECLS

#endif /* CyberKitGeolocationManager_h */
