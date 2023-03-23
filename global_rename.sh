# Rename contents of files
echo "(1/8) Replacing CyberScriptCore"
LC_ALL=C git grep -lz "CyberScriptCore" . | xargs -0 gsed -i 's|CyberScriptCore|CyberScriptCore|g'
echo "(2/8) Replacing CyberCore"
LC_ALL=C git grep -lz "CyberCore" . | xargs -0 gsed -i 's|CyberCore|CyberCore|g'
echo "(3/8) Replacing CyberKitLegacy"
LC_ALL=C git grep -lz "CyberKitLegacy" . | xargs -0 gsed -i 's|CyberKitLegacy|CyberKitLegacy|g'
echo "(4/8) Replacing CyberKit"
LC_ALL=C git grep -lz "CyberKit" . | xargs -0 gsed -i 's|CyberKit|CyberKit|g'

# Rename some actual files
echo "(5/8) Renaming CyberScriptCore"
find . -name "*CyberScriptCore*" -exec rename 's|CyberScriptCore|CyberScriptCore|g' {} ";"
echo "(6/8) Renaming CyberCore"
find . -name "*CyberCore*" -exec rename 's|CyberCore|CyberCore|g' {} ";"
echo "(7/8) Renaming CyberKitLegacy"
find . -name "*CyberKitLegacy*" -exec rename 's|CyberKitLegacy|CyberKitLegacy|g' {} ";"
echo "(8/8) Renaming CyberKit"
find . -name "*CyberKit*" -exec rename 's|CyberKit|CyberKit|g' {} ";"

# Update git to track this
git add *
