#!/bin/sh

set -e

ARGS=("$@")

mkdir -p "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberScriptCore"
cd "${BUILT_PRODUCTS_DIR}/DerivedSources/CyberScriptCore"

/bin/ln -sfh "${SRCROOT}" CyberScriptCore
export CyberScriptCore="CyberScriptCore"
export BUILT_PRODUCTS_DIR="../.."

if [ ! "$CC" ]; then
    export CC="`xcrun -find clang`"
fi

make --no-builtin-rules -f "CyberScriptCore/DerivedSources.make" -j `/usr/sbin/sysctl -n hw.ncpu` SDKROOT="${SDKROOT}" "${ARGS[@]}"
