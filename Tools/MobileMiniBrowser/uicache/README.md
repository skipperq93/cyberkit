This is a modification of the [Procursus implementation](https://github.com/ProcursusTeam/uikittools-ng/blob/91abeceb451638a3fd992c4a3406a2208ec511a1/uicache.m) of UICache.

Build instructions:

```
#!/bin/zsh

xcrun --sdk iphoneos11.4 clang uicache.m -F /System/Library/PrivateFrameworks -framework MobileContainerManager -framework MobileCoreServices -framework Foundation -framework Security -miphoneos-version-min=9.0 -o cyberuicache64 -arch arm64;
xcrun --sdk iphoneos11.4 clang uicache.m -F /System/Library/PrivateFrameworks -framework MobileContainerManager -framework MobileCoreServices -framework Foundation -framework Security -miphoneos-version-min=9.0 -o cyberuicachev7 -arch armv7;
lipo cyberuicachev7 cyberuicache64 -create -output cyberuicache;
ldid -Suicache.plist cyberuicache;
```
