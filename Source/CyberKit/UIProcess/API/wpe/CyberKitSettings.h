/*
 * Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Motorola Mobility, Inc. nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if !defined(__WEBKIT_H_INSIDE__) && !defined(WEBKIT2_COMPILATION)
#error "Only <wpe/webkit.h> can be included directly."
#endif

#ifndef CyberKitSettings_h
#define CyberKitSettings_h

#include <glib-object.h>
#include <wpe/CyberKitDefines.h>

G_BEGIN_DECLS

#define WEBKIT_TYPE_SETTINGS            (webkit_settings_get_type())
#define WEBKIT_SETTINGS(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), WEBKIT_TYPE_SETTINGS, CyberKitSettings))
#define WEBKIT_SETTINGS_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  WEBKIT_TYPE_SETTINGS, CyberKitSettingsClass))
#define WEBKIT_IS_SETTINGS(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), WEBKIT_TYPE_SETTINGS))
#define WEBKIT_IS_SETTINGS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  WEBKIT_TYPE_SETTINGS))
#define WEBKIT_SETTINGS_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  WEBKIT_TYPE_SETTINGS, CyberKitSettingsClass))

typedef struct _CyberKitSettings CyberKitSettings;
typedef struct _CyberKitSettingsClass CyberKitSettingsClass;
typedef struct _CyberKitSettingsPrivate CyberKitSettingsPrivate;

struct _CyberKitSettings {
    GObject parent_instance;

    CyberKitSettingsPrivate *priv;
};

struct _CyberKitSettingsClass {
    GObjectClass parent_class;

    void (*_webkit_reserved0) (void);
    void (*_webkit_reserved1) (void);
    void (*_webkit_reserved2) (void);
    void (*_webkit_reserved3) (void);
};

WEBKIT_API GType
webkit_settings_get_type(void);

WEBKIT_API CyberKitSettings *
webkit_settings_new                                            (void);

WEBKIT_API CyberKitSettings *
webkit_settings_new_with_settings                              (const gchar    *first_setting_name,
                                                                ...);

WEBKIT_API gboolean
webkit_settings_get_enable_javascript                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_javascript                          (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_auto_load_images                           (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_auto_load_images                           (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_load_icons_ignoring_image_load_setting     (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_load_icons_ignoring_image_load_setting     (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_offline_web_application_cache       (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_offline_web_application_cache       (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_html5_local_storage                 (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_html5_local_storage                 (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_html5_database                      (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_html5_database                      (CyberKitSettings *settings,
                                                                gboolean        enabled);
WEBKIT_API gboolean
webkit_settings_get_enable_xss_auditor                         (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_xss_auditor                         (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_frame_flattening                    (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_frame_flattening                    (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_plugins                             (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_plugins                             (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_java                                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_java                                (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_javascript_can_open_windows_automatically  (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_javascript_can_open_windows_automatically  (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_hyperlink_auditing                  (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_hyperlink_auditing                  (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API const gchar *
webkit_settings_get_default_font_family                        (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_font_family                        (CyberKitSettings *settings,
                                                                const gchar    *default_font_family);

WEBKIT_API const gchar *
webkit_settings_get_monospace_font_family                      (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_monospace_font_family                      (CyberKitSettings *settings,
                                                                const gchar    *monospace_font_family);

WEBKIT_API const gchar *
webkit_settings_get_serif_font_family                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_serif_font_family                          (CyberKitSettings *settings,
                                                                const gchar    *serif_font_family);

WEBKIT_API const gchar *
webkit_settings_get_sans_serif_font_family                     (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_sans_serif_font_family                     (CyberKitSettings *settings,
                                                                const gchar    *sans_serif_font_family);

WEBKIT_API const gchar *
webkit_settings_get_cursive_font_family                        (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_cursive_font_family                        (CyberKitSettings *settings,
                                                                const gchar    *cursive_font_family);

WEBKIT_API const gchar *
webkit_settings_get_fantasy_font_family                        (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_fantasy_font_family                        (CyberKitSettings *settings,
                                                                const gchar    *fantasy_font_family);

WEBKIT_API const gchar *
webkit_settings_get_pictograph_font_family                     (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_pictograph_font_family                     (CyberKitSettings *settings,
                                                                const gchar    *pictograph_font_family);

WEBKIT_API guint32
webkit_settings_get_default_font_size                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_font_size                          (CyberKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API guint32
webkit_settings_get_default_monospace_font_size                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_monospace_font_size                (CyberKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API guint32
webkit_settings_get_minimum_font_size                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_minimum_font_size                          (CyberKitSettings *settings,
                                                                guint32         font_size);

WEBKIT_API const gchar *
webkit_settings_get_default_charset                            (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_default_charset                            (CyberKitSettings *settings,
                                                                const gchar    *default_charset);

WEBKIT_API gboolean
webkit_settings_get_enable_developer_extras                    (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_developer_extras                    (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_resizable_text_areas                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_resizable_text_areas                (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_tabs_to_links                       (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_tabs_to_links                       (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_dns_prefetching                     (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_dns_prefetching                     (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_caret_browsing                      (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_caret_browsing                      (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_fullscreen                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_fullscreen                          (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_print_backgrounds                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_print_backgrounds                          (CyberKitSettings *settings,
                                                                gboolean        print_backgrounds);

WEBKIT_API gboolean
webkit_settings_get_enable_webaudio                            (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_webaudio                            (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_webgl                               (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_webgl                               (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API void
webkit_settings_set_allow_modal_dialogs                        (CyberKitSettings *settings,
                                                                gboolean        allowed);

WEBKIT_API gboolean
webkit_settings_get_allow_modal_dialogs                        (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_zoom_text_only                             (CyberKitSettings *settings,
                                                                gboolean        zoom_text_only);

WEBKIT_API gboolean
webkit_settings_get_zoom_text_only                             (CyberKitSettings *settings);

WEBKIT_API gboolean
webkit_settings_get_javascript_can_access_clipboard            (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_javascript_can_access_clipboard            (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_media_playback_requires_user_gesture       (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_media_playback_requires_user_gesture       (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_media_playback_allows_inline               (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_media_playback_allows_inline               (CyberKitSettings *settings,
                                                                gboolean        enabled);
WEBKIT_API gboolean
webkit_settings_get_draw_compositing_indicators                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_draw_compositing_indicators                (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_site_specific_quirks                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_site_specific_quirks                (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_page_cache                          (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_page_cache                          (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API const gchar *
webkit_settings_get_user_agent                                 (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_user_agent                                 (CyberKitSettings *settings,
                                                                const gchar    *user_agent);
WEBKIT_API void
webkit_settings_set_user_agent_with_application_details        (CyberKitSettings *settings,
                                                                const gchar    *application_name,
                                                                const gchar    *application_version);

WEBKIT_API gboolean
webkit_settings_get_enable_smooth_scrolling                    (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_smooth_scrolling                    (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_accelerated_2d_canvas               (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_accelerated_2d_canvas               (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_write_console_messages_to_stdout    (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_write_console_messages_to_stdout    (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_media_stream                        (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_media_stream                        (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_mock_capture_devices                (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_mock_capture_devices                (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_spatial_navigation                  (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_spatial_navigation                  (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_mediasource                         (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_mediasource                         (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_encrypted_media                     (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_encrypted_media                     (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_media_capabilities                  (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_media_capabilities                  (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_allow_file_access_from_file_urls           (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_allow_file_access_from_file_urls           (CyberKitSettings *settings,
                                                                gboolean        allowed);

WEBKIT_API gboolean
webkit_settings_get_allow_universal_access_from_file_urls      (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_allow_universal_access_from_file_urls      (CyberKitSettings *settings,
                                                                gboolean        allowed);

WEBKIT_API gboolean
webkit_settings_get_enable_javascript_markup                   (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_javascript_markup                   (CyberKitSettings *settings,
                                                                gboolean        enabled);

WEBKIT_API gboolean
webkit_settings_get_enable_media                               (CyberKitSettings *settings);

WEBKIT_API void
webkit_settings_set_enable_media                               (CyberKitSettings *settings,
                                                                gboolean        enabled);

G_END_DECLS

#endif /* CyberKitSettings_h */
