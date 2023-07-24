cmake_minimum_required(VERSION 2.8.12)
include(CyberKitCommon)

add_definitions(
    -DUSE_CF=1
    -DD3D_DEBUG_INFO
    -DBUILDING_WKQCA
    -DDISABLE_COREIMAGE
    -DDISABLE_FRONTEND
    -DDISABLE_IOSURFACE
    -DDISABLE_RENDERSERVER
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(CyberKitQuartzCoreAdditions_SOURCES
    API/WKCACFImage.cpp
    API/WKCACFView.cpp
    API/CyberKitQuartzCoreAdditions.cpp

    CAD3DRenderer.cpp
    CAView.cpp
    CVDisplayLink.cpp
    Image.cpp
    ImageConversion.cpp
    ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions/InvertColorPS.h
)

file(MAKE_DIRECTORY "${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions")
add_custom_command(
    OUTPUT ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions/InvertColorPS.h
    MAIN_DEPENDENCY InvertColorPS.hlsl
    COMMAND fxc /E "InvertColorPS" /T ps_2_0 /Fh "${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions/InvertColorPS.h" "${CMAKE_CURRENT_SOURCE_DIR}/InvertColorPS.hlsl"
    VERBATIM)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/API/CyberKitQuartzCoreAdditionsBase.h DESTINATION ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/API/WKCACFImage.h DESTINATION ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/API/WKCACFImageInternal.h DESTINATION ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/API/WKCACFView.h DESTINATION ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/API/WKCACFViewPrivate.h DESTINATION ${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions)

WEBKIT_ADD_PRECOMPILED_HEADER("CyberKitQuartzCoreAdditionsPrefix.h" "CyberKitQuartzCoreAdditionsPrefix.cpp" CyberKitQuartzCoreAdditions_SOURCES)

include_directories(
    .
    "${DERIVED_SOURCES_DIR}/ForwardingHeaders"
    "${DERIVED_SOURCES_DIR}/CyberKitQuartzCoreAdditions"
    "${CMAKE_BINARY_DIR}/../include/private"
)

add_library(CyberKitQuartzCoreAdditions SHARED ${CyberKitQuartzCoreAdditions_SOURCES})
set_target_properties(CyberKitQuartzCoreAdditions PROPERTIES OUTPUT_NAME CyberKitQuartzCoreAdditions${DEBUG_SUFFIX})
target_link_libraries(CyberKitQuartzCoreAdditions
    D3d9
    CyberKit::WTF
    CoreFoundation${DEBUG_SUFFIX}
    CoreGraphics${DEBUG_SUFFIX}
    QuartzCore${DEBUG_SUFFIX}
)
