include(platform/Cairo.cmake)
include(platform/FreeType.cmake)
include(platform/GCrypt.cmake)
include(platform/GStreamer.cmake)
include(platform/ImageDecoders.cmake)
include(platform/Soup.cmake)
include(platform/TextureMapper.cmake)

set(CyberCore_OUTPUT_NAME CyberCoreGTK)

list(APPEND CyberCore_UNIFIED_SOURCE_LIST_FILES
    "SourcesGTK.txt"

    "platform/SourcesGLib.txt"
)

list(APPEND CyberCore_PRIVATE_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/accessibility/atk"
    "${WEBCORE_DIR}/editing/atk"
    "${WEBCORE_DIR}/page/gtk"
    "${WEBCORE_DIR}/platform/adwaita"
    "${WEBCORE_DIR}/platform/generic"
    "${WEBCORE_DIR}/platform/gtk"
    "${WEBCORE_DIR}/platform/graphics/egl"
    "${WEBCORE_DIR}/platform/graphics/glx"
    "${WEBCORE_DIR}/platform/graphics/gstreamer"
    "${WEBCORE_DIR}/platform/graphics/gtk"
    "${WEBCORE_DIR}/platform/graphics/opengl"
    "${WEBCORE_DIR}/platform/graphics/opentype"
    "${WEBCORE_DIR}/platform/graphics/wayland"
    "${WEBCORE_DIR}/platform/graphics/x11"
    "${WEBCORE_DIR}/platform/mediacapabilities"
    "${WEBCORE_DIR}/platform/mediastream/gtk"
    "${WEBCORE_DIR}/platform/mediastream/gstreamer"
    "${WEBCORE_DIR}/platform/mock/mediasource"
    "${WEBCORE_DIR}/platform/network/gtk"
    "${WEBCORE_DIR}/platform/text/gtk"
)

if (USE_ANGLE_WEBGL)
    list(APPEND CyberCore_PRIVATE_INCLUDE_DIRECTORIES
        "${WEBCORE_DIR}/platform/graphics/angle"
    )
endif ()

if (USE_WPE_RENDERER)
    list(APPEND CyberCore_INCLUDE_DIRECTORIES
        "${WEBCORE_DIR}/platform/graphics/libwpe"
    )
endif ()

list(APPEND CyberCore_PRIVATE_FRAMEWORK_HEADERS
    platform/adwaita/ScrollbarThemeAdwaita.h

    platform/graphics/x11/PlatformDisplayX11.h
    platform/graphics/x11/XErrorTrapper.h
    platform/graphics/x11/XUniquePtr.h
    platform/graphics/x11/XUniqueResource.h

    platform/gtk/GRefPtrGtk.h
    platform/gtk/GUniquePtrGtk.h
    platform/gtk/GtkUtilities.h
    platform/gtk/GtkVersioning.h
    platform/gtk/ScrollbarThemeGtk.h
    platform/gtk/SelectionData.h

    platform/text/enchant/TextCheckerEnchant.h
)

list(APPEND CyberCore_USER_AGENT_STYLE_SHEETS
    ${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsAdwaita.css
    ${WEBCORE_DIR}/css/themeAdwaita.css
)

set(CyberCore_USER_AGENT_SCRIPTS
    ${WEBCORE_DIR}/Modules/mediacontrols/mediaControlsAdwaita.js
)

set(CyberCore_USER_AGENT_SCRIPTS_DEPENDENCIES ${WEBCORE_DIR}/rendering/RenderThemeAdwaita.cpp)

list(APPEND CyberCore_LIBRARIES
    ${ATK_LIBRARIES}
    ${ENCHANT_LIBRARIES}
    ${GLIB_GIO_LIBRARIES}
    ${GLIB_GMODULE_LIBRARIES}
    ${GLIB_GOBJECT_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${LIBSECCOMP_LIBRARIES}
    ${LIBSECRET_LIBRARIES}
    ${LIBTASN1_LIBRARIES}
    ${HYPHEN_LIBRARIES}
    ${UPOWERGLIB_LIBRARIES}
    ${X11_X11_LIB}
    ${X11_Xcomposite_LIB}
    ${X11_Xdamage_LIB}
    ${X11_Xrender_LIB}
    ${X11_Xt_LIB}
    GTK::GTK
)

if (USE_WPE_RENDERER)
    list(APPEND CyberCore_LIBRARIES
        WPE::libwpe
    )
endif ()

list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${ATK_INCLUDE_DIRS}
    ${ENCHANT_INCLUDE_DIRS}
    ${GIO_UNIX_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBSECCOMP_INCLUDE_DIRS}
    ${LIBSECRET_INCLUDE_DIRS}
    ${LIBTASN1_INCLUDE_DIRS}
    ${UPOWERGLIB_INCLUDE_DIRS}
)

if (USE_OPENGL)
    list(APPEND CyberCore_SOURCES
        platform/graphics/OpenGLShims.cpp
    )
endif ()

if (USE_ANGLE_WEBGL)
    list(APPEND CyberCore_SOURCES
        platform/graphics/angle/ExtensionsGLANGLE.cpp
        platform/graphics/angle/GraphicsContextGLANGLE.cpp
        platform/graphics/angle/TemporaryANGLESetting.cpp
    )
else ()
    list(APPEND CyberCore_SOURCES
        platform/graphics/opengl/ExtensionsGLOpenGLCommon.cpp
        platform/graphics/opengl/GraphicsContextGLOpenGLCommon.cpp
        platform/graphics/opengl/TemporaryOpenGLSetting.cpp
    )

    if (USE_OPENGL_ES)
        list(APPEND CyberCore_SOURCES
            platform/graphics/opengl/ExtensionsGLOpenGLES.cpp
            platform/graphics/opengl/GraphicsContextGLOpenGLES.cpp
        )
    endif ()

    if (USE_OPENGL)
        list(APPEND CyberCore_SOURCES
            platform/graphics/opengl/ExtensionsGLOpenGL.cpp
            platform/graphics/opengl/GraphicsContextGLOpenGLBase.cpp
        )
    endif ()
endif ()

if (ENABLE_WAYLAND_TARGET)
    list(APPEND CyberCore_PRIVATE_FRAMEWORK_HEADERS
        platform/graphics/wayland/PlatformDisplayWayland.h
        platform/graphics/wayland/WlUniquePtr.h
    )
    list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
        ${WAYLAND_INCLUDE_DIRS}
    )
    list(APPEND CyberCore_LIBRARIES
        ${WAYLAND_LIBRARIES}
    )
endif ()

if (ENABLE_GAMEPAD)
    list(APPEND CyberCore_PRIVATE_FRAMEWORK_HEADERS
        platform/gamepad/manette/ManetteGamepadProvider.h
    )
    list(APPEND CyberCore_LIBRARIES
        Manette::Manette
    )
endif ()

include_directories(SYSTEM
    ${CyberCore_SYSTEM_INCLUDE_DIRECTORIES}
)

list(APPEND CyberCoreTestSupport_LIBRARIES PRIVATE GTK::GTK)

add_definitions(-DBUILDING_WEBKIT)

if (ENABLE_SMOOTH_SCROLLING)
    list(APPEND CyberCore_SOURCES
        platform/ScrollAnimationSmooth.cpp
    )
endif ()
