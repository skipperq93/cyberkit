#!/bin/zsh

# Rename contents of files
echo $'(1/16) Replacing \u004A\u0061\u0076\u0061ScriptCore'
LC_ALL=C git grep -lz $'\u004A\u0061\u0076\u0061ScriptCore' . | xargs -0 gsed -i $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g'
echo $'(2/16) Replacing \u0057\u0065\u0062Core'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Core' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Core|CyberCore|g'
echo $'(3/16) Replacing \u0057\u0065\u0062KitLegacy'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062KitLegacy' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g'
echo $'(4/16) Replacing \u0057\u0065\u0062Kit'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Kit' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Kit|CyberKit|g'

# Because the file contents are renamed, the CyberKit equivalents are correct here
echo $'(5/16) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore|com.matthewbenedict.CyberScriptCore|g'
echo $'(6/16) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberCore|com.matthewbenedict.CyberCore|g'
echo $'(7/16) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy|com.matthewbenedict.CyberKitLegacy|g'
echo $'(8/16) Replacing com.\u0061\u0070\u0070\u006C\u0065.CyberKit'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.CyberKit' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKit|com.matthewbenedict.CyberKit|g'

# Rename some actual files
echo $'(9/16) Renaming \u004A\u0061\u0076\u0061ScriptCore'
find . -name $'*\u004A\u0061\u0076\u0061ScriptCore*' -exec /usr/local/bin/rename $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g' {} ";"
echo $'(10/16) Renaming \u0057\u0065\u0062Core'
find . -name $'*\u0057\u0065\u0062Core*' -exec /usr/local/bin/rename $'s|\u0057\u0065\u0062Core|CyberCore|g' {} ";"
echo $'(11/16) Renaming \u0057\u0065\u0062KitLegacy'
find . -name $'*\u0057\u0065\u0062KitLegacy*' -exec /usr/local/bin/rename $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g' {} ";"
echo $'(12/16) Renaming \u0057\u0065\u0062Kit'
find . -name $'*\u0057\u0065\u0062Kit*' -exec /usr/local/bin/rename $'s|\u0057\u0065\u0062Kit|CyberKit|g' {} ";"

# Because the files are renamed, the CyberKit equivalents are correct here
echo $'(13/16) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore*' \
	-exec /usr/local/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberScriptCore|com.matthewbenedict.CyberScriptCore|g' {} ";"
echo $'(14/16) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberCore*' \
	-exec /usr/local/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberCore|com.matthewbenedict.CyberCore|g' {} ";"
echo $'(15/16) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy*' \
	-exec /usr/local/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKitLegacy|com.matthewbenedict.CyberKitLegacy|g' {} ";"
echo $'(16/16) Renaming com.\u0061\u0070\u0070\u006C\u0065.CyberKit'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.CyberKit*' \
	-exec /usr/local/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.CyberKit|com.matthewbenedict.CyberKit|g' {} ";"

# Update git to track this
git add *
