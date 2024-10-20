#!/bin/zsh

# Check initial conditions
if [[ $EUID -eq 0 ]]; then
    echo "[!] Please don't run this script as root!"
    exit
fi
export PATH="/opt/homebrew/bin:$PATH"
if [[ $(command -v brew) == "" ]]; then
    echo "[!] Homebrew not installed!"
    echo "[!] PATH is $PATH"
    echo "[!] Please run the following command!"
    echo '[!] /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"'
    exit
else
    echo "[§] Found Homebrew"
    if brew ls --versions ldid >/dev/null; then
        echo "[§] Found ldid"
    else
        echo "[§] Attempting to install script dependencies"
        brew install coreutils ldid gnu-sed dpkg
    fi
fi

# Prepare payload directory
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
rm -rf $SCRIPT_DIR/../../WebKitBuild/Debug-iphoneos/Payload
ipa=$SCRIPT_DIR/../../WebKitBuild/Debug-iphoneos/MobileMiniBrowser.app
if [[ $ipa == *.ipa ]]; then
    echo [*] unpacking..
    cd $(dirname $ipa) || exit 1
    unzip "$ipa"
    cd Payload
    app=$(ls -1 -d *.app)
elif [[ $ipa == *.app ]]; then
    cd $(dirname $ipa) || exit 1
    mkdir Payload
    cp -R $ipa $(dirname $ipa)/Payload
    app=$(ls -1 -d *.app)
    cd Payload

    cp -R ../WebCore.framework $app/Frameworks
    cp -R ../WebKit.framework $app/Frameworks
    cp -R ../WebKitLegacy.framework $app/Frameworks
    cp -R ../JavaScriptCore.framework $app/Frameworks
    cp -R ../WebGPU.framework $app/Frameworks
    cp -R ../libANGLE-shared.dylib $app/Frameworks
    cp -R ../libwebrtc.dylib $app/Frameworks

    rm -rf $app/Frameworks/WebKit.framework/XPCServices && mkdir $app/Frameworks/WebKit.framework/XPCServices
    rm -rf $app/Frameworks/WebKit.framework/Daemons && mkdir $app/Frameworks/WebKit.framework/Daemons
    cp -R ../*.xpc $app/Frameworks/WebKit.framework/XPCServices
    cp ../adattributiond $app/Frameworks/WebKit.framework/Daemons
    cp ../webpushd $app/Frameworks/WebKit.framework/Daemons
    cp ../../../Source/WTF/icu/unicode/data/out/*.dat $app/Frameworks/WebKit.framework/XPCServices

    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.GPU.xpc
    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.Networking.xpc
    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.CaptivePortal.xpc
    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.Crashy.xpc
    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.Development.xpc
    ln -s ../../../../Frameworks $app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.xpc

    install_name_tool -change /System/Library/Frameworks/WebKit.framework/WebKit @rpath/WebKit.framework/WebKit $app/MobileMiniBrowser
    install_name_tool -change /System/Library/Frameworks/WebKit.framework/WebKit @rpath/WebKit.framework/WebKit $app/Frameworks/MobileMiniBrowser.framework/MobileMiniBrowser
else
    echo "[!] No .ipa file supplied!"
fi
cp $SCRIPT_DIR/script_fakesigner.entitlements .

# Fakesign
echo "[1/15] Fakesigning com.matthewbenedict.WebKit.GPU.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.GPU.xpc"
echo "[2/15] Fakesigning com.matthewbenedict.WebKit.Networking.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.Networking.xpc"
echo "[3/15] Fakesigning com.matthewbenedict.WebKit.WebContent.CaptivePortal.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.CaptivePortal.xpc"
echo "[4/15] Fakesigning com.matthewbenedict.WebKit.WebContent.Crashy.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.Crashy.xpc"
echo "[5/15] Fakesigning com.matthewbenedict.WebKit.WebContent.Development.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.Development.xpc"
echo "[6/15] Fakesigning com.matthewbenedict.WebKit.WebContent.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/XPCServices/com.matthewbenedict.WebKit.WebContent.xpc"
echo "[7/15] Fakesigning libANGLE-shared.dylib"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/libANGLE-shared.dylib"
echo "[8/15] Fakesigning libwebrtc.dylib"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/libwebrtc.dylib"

echo "[9/15] Fakesigning WebCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebCore.framework/WebCore"
echo "[10/15] Fakesigning WebKit.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKit.framework/WebKit"
echo "[11/15] Fakesigning WebKitLegacy.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebKitLegacy.framework/WebKitLegacy"
echo "[12/15] Fakesigning JavaScriptCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/JavaScriptCore.framework/JavaScriptCore"
echo "[13/15] Fakesigning MobileMiniBrowser.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/MobileMiniBrowser.framework/MobileMiniBrowser"
echo "[14/15] Fakesigning WebGPU.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebGPU.framework/WebGPU"
echo "[15/15] Fakesigning MobileMiniBrowser"
ldid -S"script_fakesigner.entitlements" "$app/${app:0:${#app}-4}"
rm script_fakesigner.entitlements

# Setup for DEB packaging
cd ..
rm -f *.deb || true
DIR_NAME=$1
if [[ "$DIR_NAME" == *"+"* ]]; then
    if [[ ${DIR_NAME#*+} -le 10 ]]; then
        # We manually stash WebKit on legacy iOS because it doesn't fit in the root partition
        echo "[*] Using stashed application path"
        APPLICATION_PATH=$DIR_NAME/private/var/mobile/Containers/Bundle/Application/DEADBEEF-DEAD-BEEF-DEAD-BEEFC1B34800
    else
        # Put WebKit in the rootful application path
        echo "[*] Using rootful application path"
        APPLICATION_PATH=$DIR_NAME/Applications
    fi
else
    echo "[*] Using rootless application path"
    APPLICATION_PATH=$DIR_NAME/private/var/jb/Applications
fi

# Package into DEB
echo "[*] Creating DEB..."
DEBIAN_FILES=$SCRIPT_DIR/resources/$DIR_NAME
mkdir $DIR_NAME
if [[ "$DIR_NAME" == *"+"* ]] && [[ ${DIR_NAME#*+} -le 10 ]]; then
    mkdir $DIR_NAME/private
    mkdir $DIR_NAME/private/var
    mkdir $DIR_NAME/private/var/mobile
    mkdir $DIR_NAME/private/var/mobile/Containers
    mkdir $DIR_NAME/private/var/mobile/Containers/Bundle
    mkdir $DIR_NAME/private/var/mobile/Containers/Bundle/Application
elif [[ "$DIR_NAME" != *"+"* ]]; then
    mkdir $DIR_NAME/private
    mkdir $DIR_NAME/private/var
    mkdir $DIR_NAME/private/var/jb
fi

mv Payload $APPLICATION_PATH
mkdir $DIR_NAME/DEBIAN
cp -R $DEBIAN_FILES/* $DIR_NAME/DEBIAN
find . -name ".DS_Store" -delete

if [[ "$DIR_NAME" == *"+"* ]] && [[ ${DIR_NAME#* } -le 10 ]]; then
    cp $SCRIPT_DIR/uicache/cyberuicache $APPLICATION_PATH
    cp $SCRIPT_DIR/script_fakesigner.entitlements $APPLICATION_PATH
    cp $SCRIPT_DIR/uicache/.com.apple.mobile_container_manager.metadata.plist $APPLICATION_PATH
    echo "[*] Forcing gzip compression for legacy iOS"
    dpkg-deb -Zgzip --build $DIR_NAME && dpkg-name $DIR_NAME.deb
else
    echo "[*] Using default compression for non-legacy iOS"
    dpkg-deb -b $DIR_NAME && dpkg-name $DIR_NAME.deb
fi
mv $APPLICATION_PATH Payload
rm -rf $DIR_NAME

# Package into IPA
IPA_NAME=$(echo *.deb | sed 's/_[a-z].*//' | sed 's/\+\([0-9]\)/\1/' | sed 's/-\([0-9]\)/\1/')
echo "[*] Creating IPA..."
rm -f *.tipa || true
zip -r -y "$IPA_NAME.tipa" Payload
echo "[*] Created $IPA_NAME.tipa"
