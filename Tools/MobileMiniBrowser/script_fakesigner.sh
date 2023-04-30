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
      echo "[!] ldid not found!"
      echo "[!] Please install ldid with the following command"
      echo "[!] brew install ldid"
    fi
fi

# Prepare payload directory
SCRIPT_DIR=$(realpath "$(dirname "${BASH_SOURCE[0]}")")
rm -rf $SCRIPT_DIR/../../CyberKitBuild/Debug-iphoneos/Payload
ipa=$SCRIPT_DIR/../../CyberKitBuild/Debug-iphoneos/MobileMiniBrowser.app
if [[ $ipa == *.ipa ]]; then
echo [*] unpacking..
cd $(dirname $ipa)
unzip "$ipa"
cd Payload
app=$(ls -1 -d *.app)
elif [[ $ipa == *.app ]]; then
cd $(dirname $ipa)
mkdir Payload
cp -R $ipa $(dirname $ipa)/Payload
app=$(ls -1 -d *.app)
cd Payload
rm -rf $app/Frameworks/CyberKit.framework/XPCServices && mkdir $app/Frameworks/CyberKit.framework/XPCServices
rm -rf $app/Frameworks/CyberKit.framework/Daemons && mkdir $app/Frameworks/CyberKit.framework/Daemons
cp -R ../*.xpc $app/Frameworks/CyberKit.framework/XPCServices
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.GPU.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Networking.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.CaptivePortal.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Crashy.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Development.xpc
ln -s ../../../../Frameworks $app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.xpc
cp ../adattributiond $app/Frameworks/CyberKit.framework/Daemons
cp ../webpushd $app/Frameworks/CyberKit.framework/Daemons
plutil -convert xml1 $app/Info.plist
gsed -i -e 's/com.matthewbenedict.ios.Fennec/com.matthewbenedict.MobileMiniBrowser/g' $app/Info.plist
plutil -convert binary1 $app/Info.plist
else
echo "[!] No .ipa file supplied!"
fi
cp $SCRIPT_DIR/script_fakesigner.entitlements .

# Fakesign
echo "[1/15] Fakesigning com.matthewbenedict.CyberKit.GPU.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.GPU.xpc"
echo "[2/15] Fakesigning com.matthewbenedict.CyberKit.Networking.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.Networking.xpc"
echo "[3/15] Fakesigning com.matthewbenedict.CyberKit.WebContent.CaptivePortal.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.CaptivePortal.xpc"
echo "[4/15] Fakesigning com.matthewbenedict.CyberKit.WebContent.Crashy.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Crashy.xpc"
echo "[5/15] Fakesigning com.matthewbenedict.CyberKit.WebContent.Development.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.Development.xpc"
echo "[6/15] Fakesigning com.matthewbenedict.CyberKit.WebContent.xpc"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/XPCServices/com.matthewbenedict.CyberKit.WebContent.xpc"
echo "[7/15] Fakesigning libANGLE-shared.dylib"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/libANGLE-shared.dylib"
echo "[8/15] Fakesigning libwebrtc.dylib"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/libwebrtc.dylib"

echo "[9/15] Fakesigning CyberCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberCore.framework/CyberCore"
echo "[10/15] Fakesigning CyberKit.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKit.framework/CyberKit"
echo "[11/15] Fakesigning CyberKitLegacy.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberKitLegacy.framework/CyberKitLegacy"
echo "[12/15] Fakesigning CyberScriptCore.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/CyberScriptCore.framework/CyberScriptCore"
echo "[13/15] Fakesigning MobileMiniBrowser.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/MobileMiniBrowser.framework/MobileMiniBrowser"
echo "[14/15] Fakesigning WebGPU.framework"
ldid -S"script_fakesigner.entitlements" "$app/Frameworks/WebGPU.framework/WebGPU"
echo "[15/15] Fakesigning MobileMiniBrowser"
ldid -S"script_fakesigner.entitlements" "$app/${app:0:${#app}-4}"
rm script_fakesigner.entitlements

# Package into IPA
cd ..
echo "[*] packaging.."
rm -f "$ipa.zip" || true
zip -r -y "$ipa.zip" Payload
echo "[*] Created $ipa.zip"