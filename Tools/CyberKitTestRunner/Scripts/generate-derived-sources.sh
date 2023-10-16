#!/bin/sh

set -e

ARGS=("$@")

mkdir -p "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKitTestRunner"
cd "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKitTestRunner"

export CyberKitTestRunner="${SRCROOT}"
/bin/ln -sfh "${WEBCORE_PRIVATE_HEADERS_DIR}" CyberCorePrivateHeaders
export CyberCoreScripts="CyberCorePrivateHeaders"

if [ ! "$CC" ]; then
    export CC="`xcrun -find clang`"
fi

if [ "${ACTION}" = "analyze" -o "${ACTION}" = "build" -o "${ACTION}" = "install" -o "${ACTION}" = "installhdrs" ]; then
    make -f "${CyberKitTestRunner}/DerivedSources.make" -j `/usr/sbin/sysctl -n hw.activecpu` SDKROOT="${SDKROOT}" "${ARGS[@]}"
fi
