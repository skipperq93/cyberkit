#!/bin/bash

# Make sure that Homebrew is in the PATH if it could be
PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH"

# Check initial conditions
if [[ $EUID -eq 0 ]]; then
    echo "[!] Please don't run this script as root!"
    exit
fi

if [[ $(command -v brew) == "" ]]; then
    echo "[!] Homebrew not installed!"
    echo "[!] PATH is $PATH"
    echo "[!] Please run the following command!"
    echo '[!] /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"'
    exit
else
    echo "[§] Found Homebrew"
    if brew ls --versions make >/dev/null; then
        echo "[§] Found GNU make"
    else
        echo "[§] Attempting to install script dependencies"
        brew install make
    fi
fi

# Compile new source
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
cd $SCRIPT_DIR

# Run the makefile to do the compilation
# Note that the makefile also specifies a deployment target and release to check out
DEPLOYMENT_TARGET=11.0
CURRENT_VERSION=76.1
rm -rf build.noindex unicode
gmake clone build-host ios-arm64
rm -rf icu-clone.noindex
ln -s build.noindex/icu/include/unicode
xcrun -sdk iphoneos clang++ -miphoneos-version-min=$DEPLOYMENT_TARGET -Wl,-current_version,$CURRENT_VERSION -Wl,-compatibility_version,1.0 -fpic -shared -Wl,-all_load build.noindex/ios_arm64/lib/*.a -o libunicode.dylib
git add -A

