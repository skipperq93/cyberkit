#!/bin/sh

# Check initial conditions
if [[ $EUID -eq 0 ]]; then
  echo "[!] Please don't run this script as root!"
  exit
fi
if [[ $(command -v sudo -u brew) == "" ]]; then
    echo "[!] Hombrew not installed!"
    echo "[!] Please run the following command!"
    echo '[!] /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"'
    exit
else
    echo "[§] Found Homebrew"
    if brew ls --versions ldid > /dev/null; then
      echo "[§] Found ldid"
    else
      echo "[§] Attempting to install script dependencies"
      brew install ldid gnu-sed dpkg
    fi
fi

# Prepare payload directory
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
rm -rf $SCRIPT_DIR/../../CyberKitBuild/Debug-iphoneos/Payload
ipa=$SCRIPT_DIR/../../CyberKitBuild/Debug-iphoneos/MobileMiniBrowser.app
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
rm -rf $app/Frameworks/CyberKit.framework/XPCServices && mkdir $app/Frameworks/CyberKit.framework/XPCServices
cp -R ../*.xpc $app/Frameworks/CyberKit.framework/XPCServices
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Networking.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Plugin.32.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Plugin.64.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Development.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.xpc
plutil -convert xml1 $app/Info.plist
gsed -i -e 's/com.matthewbenedict.ios.Fennec/com.matthewbenedict.MobileMiniBrowser/g' $app/Info.plist
plutil -convert binary1 $app/Info.plist
else
echo "[!] No .ipa file supplied!"
fi
cp $SCRIPT_DIR/script_fakesigner.entitlements .

# Fakesign
echo "[1/12] Fakesigning com.matthewbenedict.CyberKit.Networking.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Networking.xpc"
echo "[2/12] Fakesigning com.matthewbenedict.CyberKit.WebContent.CaptivePortal.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Plugin.32.xpc"
echo "[3/12] Fakesigning com.matthewbenedict.CyberKit.WebContent.Crashy.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Plugin.64.xpc"
echo "[4/12] Fakesigning com.matthewbenedict.CyberKit.WebContent.Development.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Development.xpc"
echo "[5/12] Fakesigning com.matthewbenedict.CyberKit.WebContent.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.xpc"
echo "[6/12] Fakesigning libwebrtc.dylib"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/libwebrtc.dylib"

echo "[7/12] Fakesigning CyberCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberCore.framework/CyberCore"
echo "[8/12] Fakesigning CyberKit.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/CyberKit"
echo "[9/12] Fakesigning CyberKitLegacy.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKitLegacy.framework/CyberKitLegacy"
echo "[10/12] Fakesigning CyberScriptCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberScriptCore.framework/CyberScriptCore"
echo "[11/12] Fakesigning MobileMiniBrowser.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/MobileMiniBrowser.framework/MobileMiniBrowser"
echo "[12/12] Fakesigning MobileMiniBrowser"
ldid -S"script_fakesigner.entitlements" "$app/${app:0:${#app}-4}"
rm script_fakesigner.entitlements

# Setup for DEB packaging
cd ..
rm *.deb
DIR_NAME=$1
mkdir $DIR_NAME
if [[ "$DIR_NAME" != *"+"* ]]; then
	echo "[*] Using rootless application path"
	APPLICATION_PATH=$DIR_NAME/var/jb/Applications
	mkdir $DIR_NAME/var
    mkdir $DIR_NAME/var/jb
else
	echo "[*] Using rootful application path"
    APPLICATION_PATH=$DIR_NAME/Applications
fi

# Package into DEB
echo "[*] Creating DEB..."
DEBIAN_FILES=$SCRIPT_DIR/resources/$DIR_NAME
CYBERKIT_FRAMEWORK_VERSION=$2
EXTERNAL_FRAMEWORK_PATH=$DIR_NAME/var/mobile/Library/CyberKit/Frameworks/$CYBERKIT_FRAMEWORK_VERSION
echo "[*] Detected framework version:" $CYBERKIT_FRAMEWORK_VERSION

mv Payload $APPLICATION_PATH
mkdir $DIR_NAME/DEBIAN
cp $DEBIAN_FILES/control $DIR_NAME/DEBIAN
cp $DEBIAN_FILES/postinst $DIR_NAME/DEBIAN
find . -name ".DS_Store" -delete

echo "[*] Relocating frameworks"
mkdir $DIR_NAME/var
mkdir $DIR_NAME/var/mobile
mkdir $DIR_NAME/var/mobile/Library
mkdir $DIR_NAME/var/mobile/Library/CyberKit
mkdir $DIR_NAME/var/mobile/Library/CyberKit/Frameworks
mkdir $EXTERNAL_FRAMEWORK_PATH
mv $APPLICATION_PATH/$app/Frameworks/CyberCore.framework $EXTERNAL_FRAMEWORK_PATH/CyberCore.framework
mv $APPLICATION_PATH/$app/Frameworks/CyberKit.framework $EXTERNAL_FRAMEWORK_PATH/CyberKit.framework
mv $APPLICATION_PATH/$app/Frameworks/CyberKitLegacy.framework $EXTERNAL_FRAMEWORK_PATH/CyberKitLegacy.framework
mv $APPLICATION_PATH/$app/Frameworks/CyberScriptCore.framework $EXTERNAL_FRAMEWORK_PATH/CyberScriptCore.framework
mv $APPLICATION_PATH/$app/Frameworks/libwebrtc.dylib $EXTERNAL_FRAMEWORK_PATH/libwebrtc.dylib

if [[ "$DIR_NAME" == *"+10"* ]]; then
	echo "[*] Forcing gzip compression for legacy iOS"
	dpkg-deb -Zgzip --build $DIR_NAME && dpkg-name $DIR_NAME.deb
else
	echo "[*] Using default compression for non-legacy iOS"
	dpkg-deb -b $DIR_NAME && dpkg-name $DIR_NAME.deb
fi

echo "[*] Returning frameworks"
mv $EXTERNAL_FRAMEWORK_PATH/CyberCore.framework $APPLICATION_PATH/$app/Frameworks/CyberCore.framework
mv $EXTERNAL_FRAMEWORK_PATH/CyberKit.framework $APPLICATION_PATH/$app/Frameworks/CyberKit.framework
mv $EXTERNAL_FRAMEWORK_PATH/CyberKitLegacy.framework $APPLICATION_PATH/$app/Frameworks/CyberKitLegacy.framework
mv $EXTERNAL_FRAMEWORK_PATH/CyberScriptCore.framework $APPLICATION_PATH/$app/Frameworks/CyberScriptCore.framework
mv $EXTERNAL_FRAMEWORK_PATH/libwebrtc.dylib $APPLICATION_PATH/$app/Frameworks/libwebrtc.dylib

mv $APPLICATION_PATH Payload
rm -rf $DIR_NAME

# Package into IPA
IPA_NAME=$(echo *.deb | sed 's/_[a-z].*//' | sed 's/\+\([0-9]\)/\1/' | sed 's/-\([0-9]\)/\1/')
echo "[*] Creating IPA..."
rm -f "$IPA_NAME.ipa" || true
zip -r -y "$IPA_NAME.ipa" Payload
echo "[*] Created $IPA_NAME.ipa"
