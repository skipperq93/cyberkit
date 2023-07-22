#!/bin/sh

set -e

ARGS=("$@")

mkdir -p "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKitTestRunner"
cd "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberKitTestRunner"

export CyberKitTestRunner="${SRCROOT}"
export CyberCoreScripts="${WEBCORE_PRIVATE_HEADERS_DIR}"

if [ ! $CC ]; then
    export CC="`xcrun -find clang`"
fi

if [ "${ACTION}" = "build" -o "${ACTION}" = "install" -o "${ACTION}" = "installhdrs" ]; then
    make -f "${CyberKitTestRunner}/DerivedSources.make" -j `/usr/sbin/sysctl -n hw.activecpu` "${ARGS[@]}"
fi
