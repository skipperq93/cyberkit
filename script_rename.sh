#!/bin/zsh

# Replace bundle identifiers in file contents
echo $'(1/9) Replacing com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore|com.matthewbenedict.JavaScriptCore|g'
echo $'(2/9) Replacing com.\u0061\u0070\u0070\u006C\u0065.WebCore'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.WebCore' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.WebCore|com.matthewbenedict.WebCore|g'
echo $'(3/9) Replacing com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy|com.matthewbenedict.WebKitLegacy|g'
echo $'(4/9) Replacing com.\u0061\u0070\u0070\u006C\u0065.WebKit'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.WebKit' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.WebKit|com.matthewbenedict.WebKit|g'
echo $'(5/9) Replacing com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)'
LC_ALL=C git grep -lz $'com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)' . |\
	xargs -0 gsed -i $'s|com.\u0061\u0070\u0070\u006C\u0065.\$(PRODUCT_NAME:rfc1034identifier)|com.matthewbenedict.\$(PRODUCT_NAME:rfc1034identifier)|g'

# Rename bundle identifiers in filenames
echo $'(6/9) Renaming com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore*' \
	-exec /opt/homebrew/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.JavaScriptCore|com.matthewbenedict.JavaScriptCore|g' {} ";"
echo $'(7/9) Renaming com.\u0061\u0070\u0070\u006C\u0065.WebCore'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.WebCore*' \
	-exec /opt/homebrew/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.WebCore|com.matthewbenedict.WebCore|g' {} ";"
echo $'(8/9) Renaming com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy*' \
	-exec /opt/homebrew/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.WebKitLegacy|com.matthewbenedict.WebKitLegacy|g' {} ";"
echo $'(9/9) Renaming com.\u0061\u0070\u0070\u006C\u0065.WebKit'
find . -name $'*com.\u0061\u0070\u0070\u006C\u0065.WebKit*' \
	-exec /opt/homebrew/bin/rename $'s|com.\u0061\u0070\u0070\u006C\u0065.WebKit|com.matthewbenedict.WebKit|g' {} ";"

# Update git to track this
git add -A
