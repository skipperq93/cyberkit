git grep -l 'JavaScriptCore' | xargs sed -i '' 's|JavaScriptCore|CyberScriptCore|g'
git grep -l 'WebCore' | xargs sed -i '' 's|WebCore|CyberCore|g'
git grep -l 'WebKitLegacy' | xargs sed -i '' 's|WebKitLegacy|CyberKitLegacy|g'
git grep -l 'WebKit' | xargs sed -i '' 's|WebKit|CyberKit|g'
