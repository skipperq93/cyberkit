# Rename contents of files
echo $'(1/8) Replacing \u004A\u0061\u0076\u0061ScriptCore'
LC_ALL=C git grep -lz $'\u004A\u0061\u0076\u0061ScriptCore' . | xargs -0 gsed -i $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g'
echo $'(2/8) Replacing \u0057\u0065\u0062Core'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Core' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Core|CyberCore|g'
echo $'(3/8) Replacing \u0057\u0065\u0062KitLegacy'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062KitLegacy' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g'
echo $'(4/8) Replacing \u0057\u0065\u0062Kit'
LC_ALL=C git grep -lz $'\u0057\u0065\u0062Kit' . | xargs -0 gsed -i $'s|\u0057\u0065\u0062Kit|CyberKit|g'

# Rename some actual files
echo $'(5/8) Renaming \u004A\u0061\u0076\u0061ScriptCore'
find . -name $'*\u004A\u0061\u0076\u0061ScriptCore*' -exec rename $'s|\u004A\u0061\u0076\u0061ScriptCore|CyberScriptCore|g' {} ";"
echo $'(6/8) Renaming \u0057\u0065\u0062Core'
find . -name $'*\u0057\u0065\u0062Core*' -exec rename $'s|\u0057\u0065\u0062Core|CyberCore|g' {} ";"
echo $'(7/8) Renaming \u0057\u0065\u0062KitLegacy'
find . -name $'*\u0057\u0065\u0062KitLegacy*' -exec rename $'s|\u0057\u0065\u0062KitLegacy|CyberKitLegacy|g' {} ";"
echo $'(8/8) Renaming \u0057\u0065\u0062Kit'
find . -name $'*\u0057\u0065\u0062Kit*' -exec rename $'s|\u0057\u0065\u0062Kit|CyberKit|g' {} ";"

# Update git to track this
git add *
