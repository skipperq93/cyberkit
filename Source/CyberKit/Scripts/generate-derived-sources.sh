#!/bin/sh

set -e

ARGS=("$@")

mkdir -p "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKit2"
cd "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKit2"

export CyberKit2="${SRCROOT}"

/bin/ln -sfh "${JAVASCRIPTCORE_PRIVATE_HEADERS_DIR}" CyberScriptCorePrivateHeaders
export CyberScriptCore_SCRIPTS_DIR="CyberScriptCorePrivateHeaders"

if [ ! $CC ]; then
    export CC="`xcrun -find clang`"
fi

MAKEFILE_INCLUDE_FLAGS=$(echo "${WEBKITADDITIONS_HEADER_SEARCH_PATHS}" | perl -e 'print "-I" . join(" -I", split(" ", <>));')

if [ "${ACTION}" = "build" -o "${ACTION}" = "install" -o "${ACTION}" = "installhdrs" -o "${ACTION}" = "installapi" ]; then
    make --no-builtin-rules ${MAKEFILE_INCLUDE_FLAGS} -f "${CyberKit2}/DerivedSources.make" -j `/usr/sbin/sysctl -n hw.activecpu` SDKROOT=${SDKROOT} "${ARGS[@]}"
fi
