#!/bin/zsh

PATH="/opt/homebrew/bin:/opt/homebrew/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:$PATH"

if [ -x /opt/homebrew/bin/rename ]; then
    echo "Found /opt/homebrew/bin/rename"
elif [ -x /usr/local/bin/rename ]; then
    echo "Found /usr/local/bin/rename"
else
    echo "Please install rename with Homebrew"
    exit 1
fi

# Rename contents of files
echo $'(1/17) Replacing \u004A\u0061\u0076\u0061ScriptCore'
LC_ALL=C git grep -lz $'\u004A\u0061\u0076\u0061ScriptCore' . | xargs -0 gsed -i $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g'
echo $'(2/17) Replacing \u0057\u0065\u0062Core'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Core' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Core|CyberCore|g'
echo $'(3/17) Replacing \u0057\u0065\u0062KitLegacy'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062KitLegacy' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g'
echo $'(4/17) Replacing \u0057\u0065\u0062Kit'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Kit' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Kit|CyberKit|g'

# Fix bundle identifiers
echo $'(5/17) Replacing com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)|com.matthewbenedict.\$(PRODUCT_NAME:rfc1034identifier)|g'

# Because the file contents are renamed, the CyberKit equivalents are correct here
echo $'(6/17) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore|com.matthewbenedict.CyberScriptCore|g'
echo $'(7/17) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberCore|com.matthewbenedict.CyberCore|g'
echo $'(8/17) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy|com.matthewbenedict.CyberKitLegacy|g'
echo $'(9/17) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberKit'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberKit' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKit|com.matthewbenedict.CyberKit|g'

# Rename some actual files
echo $'(10/17) Renaming \u004A\u0061\u0076\u0061ScriptCore'
find . -name $'*\u004A\u0061\u0076\u0061ScriptCore*' -exec rename $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g' {} ";"
echo $'(11/17) Renaming \u0057\u0065\u0062Core'
find . -name $'*\u0057\u0065\u0062Core*' -exec rename $'s|\u0057\u0065\u0062Core|CyberCore|g' {} ";"
echo $'(12/17) Renaming \u0057\u0065\u0062KitLegacy'
find . -name $'*\u0057\u0065\u0062KitLegacy*' -exec rename $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g' {} ";"
echo $'(13/17) Renaming \u0057\u0065\u0062Kit'
find . -name $'*\u0057\u0065\u0062Kit*' -exec rename $'s|\u0057\u0065\u0062Kit|CyberKit|g' {} ";"

# Because the files are renamed, the CyberKit equivalents are correct here
echo $'(14/17) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore*' \
	-exec rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore|com.matthewbenedict.CyberScriptCore|g' {} ";"
echo $'(15/17) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberCore*' \
	-exec rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberCore|com.matthewbenedict.CyberCore|g' {} ";"
echo $'(16/17) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy*' \
	-exec rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy|com.matthewbenedict.CyberKitLegacy|g' {} ";"
echo $'(17/17) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberKit'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberKit*' \
	-exec rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKit|com.matthewbenedict.CyberKit|g' {} ";"

# Update git to track this
git add *
