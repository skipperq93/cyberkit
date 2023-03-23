# Rename contents of files
git grep -l 'JavaScriptCore' | LC_ALL=C xargs sed -i '' 's|JavaScriptCore|CyberScriptCore|g'
git grep -l 'WebCore' | LC_ALL=C xargs sed -i '' 's|WebCore|CyberCore|g'
git grep -l 'WebKitLegacy' | LC_ALL=C xargs sed -i '' 's|WebKitLegacy|CyberKitLegacy|g'
git grep -l 'CyberKitLibraries' | LC_ALL=C xargs sed -i '' 's|CyberKitLibraries|CyberKitLibraries|g'
git grep -l 'WebKit' | LC_ALL=C xargs sed -i '' 's|WebKit|CyberKit|g'

# Rename some actual files
find . -name "*JavaScriptCore*" -exec rename 's|JavaScriptCore|CyberScriptCore|g' {} ";"
find . -name "*WebCore*" -exec rename 's|WebCore|CyberCore|g' {} ";"
find . -name "*WebKitLegacy*" -exec rename 's|WebKitLegacy|CyberKitLegacy|g' {} ";"
find . -name "*CyberKitLibraries*" -exec rename 's|CyberKitLibraries|CyberKitLibraries|g' {} ";"
find . -name "*WebKit*" -exec rename 's|WebKit|CyberKit|g' {} ";"

# Update git to track this
git add *