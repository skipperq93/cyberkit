# Rename contents of files
echo "(1/8) Replacing JavaScriptCore"
git grep -l 'JavaScriptCore' | LC_ALL=C xargs sed -i '' 's|JavaScriptCore|CyberScriptCore|g'
echo "(2/8) Replacing WebCore"
git grep -l 'WebCore' | LC_ALL=C xargs sed -i '' 's|WebCore|CyberCore|g'
echo "(3/8) Replacing WebKitLegacy"
git grep -l 'WebKitLegacy' | LC_ALL=C xargs sed -i '' 's|WebKitLegacy|CyberKitLegacy|g'
echo "(4/8) Replacing WebKit"
git grep -l 'WebKit' | LC_ALL=C xargs sed -i '' 's|WebKit|CyberKit|g'

# Rename some actual files
echo "(5/8) Renaming JavaScriptCore"
find . -name "*JavaScriptCore*" -exec rename 's|JavaScriptCore|CyberScriptCore|g' {} ";"
echo "(6/8) Renaming WebCore"
find . -name "*WebCore*" -exec rename 's|WebCore|CyberCore|g' {} ";"
echo "(7/8) Renaming WebKitLegacy"
find . -name "*WebKitLegacy*" -exec rename 's|WebKitLegacy|CyberKitLegacy|g' {} ";"
echo "(8/8) Renaming WebKit"
find . -name "*WebKit*" -exec rename 's|WebKit|CyberKit|g' {} ";"

# Update git to track this
git add *
