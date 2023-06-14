list(APPEND CyberCore_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/platform/image-decoders"
    "${WEBCORE_DIR}/platform/image-decoders/bmp"
    "${WEBCORE_DIR}/platform/image-decoders/gif"
    "${WEBCORE_DIR}/platform/image-decoders/ico"
    "${WEBCORE_DIR}/platform/image-decoders/jpeg"
    "${WEBCORE_DIR}/platform/image-decoders/png"
    "${WEBCORE_DIR}/platform/image-decoders/webp"
)

list(APPEND CyberCore_SOURCES
    platform/image-decoders/ScalableImageDecoder.cpp
    platform/image-decoders/ScalableImageDecoderFrame.cpp

    platform/image-decoders/bmp/BMPImageDecoder.cpp
    platform/image-decoders/bmp/BMPImageReader.cpp

    platform/image-decoders/gif/GIFImageDecoder.cpp
    platform/image-decoders/gif/GIFImageReader.cpp

    platform/image-decoders/ico/ICOImageDecoder.cpp

    platform/image-decoders/jpeg/JPEGImageDecoder.cpp

    platform/image-decoders/png/PNGImageDecoder.cpp

    platform/image-decoders/webp/WEBPImageDecoder.cpp
)

list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${JPEG_INCLUDE_DIR}
    ${PNG_INCLUDE_DIRS}
)
list(APPEND CyberCore_LIBRARIES
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
)

if (WEBP_FOUND)
    list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
        ${WEBP_INCLUDE_DIRS}
    )
    list(APPEND CyberCore_LIBRARIES
        ${WEBP_LIBRARIES}
    )
endif ()

if (USE_CAIRO)
    list(APPEND CyberCore_SOURCES
        platform/image-decoders/cairo/ImageBackingStoreCairo.cpp
    )
endif ()
