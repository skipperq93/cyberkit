#!/bin/sh

set -e

ARGS=("$@")

mkdir -p "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberCore"
cd "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberCore"

/bin/ln -sfh "${SRCROOT}" CyberCore
export CyberCore="CyberCore"
/bin/ln -sfh "${JAVASCRIPTCORE_PRIVATE_HEADERS_DIR}" CyberScriptCorePrivateHeaders
export CyberScriptCore_SCRIPTS_DIR="CyberScriptCorePrivateHeaders"

if [ ! "$CC" ]; then
    export CC="`xcrun -find clang`"
fi

if [ ! "$GPERF" ]; then
    export GPERF="`xcrun -find gperf`"
fi

MAKEFILE_INCLUDE_FLAGS=$(echo "${WEBKITADDITIONS_HEADER_SEARCH_PATHS}" | perl -e 'print "-I" . join(" -I", split(" ", <>));')

if [ "${ACTION}" = "analyze" -o "${ACTION}" = "build" -o "${ACTION}" = "install" -o "${ACTION}" = "installhdrs" -o "${ACTION}" = "installapi" ]; then
    make --no-builtin-rules ${MAKEFILE_INCLUDE_FLAGS} -f "CyberCore/DerivedSources.make" -j `/usr/sbin/sysctl -n hw.activecpu` SDKROOT="${SDKROOT}" "${ARGS[@]}"
fi
