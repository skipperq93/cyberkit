This is a modification of the [Procursus implementation](https://github.com/ProcursusTeam/uikittools-ng/blob/91abeceb451638a3fd992c4a3406a2208ec511a1/uicache.m) of UICache.

For arm64 devices (iPhone 5S and newer):

```
xcrun --sdk iphoneos clang uicache.m -F /System/Library/PrivateFrameworks -framework MobileContainerManager -framework MobileCoreServices -framework Foundation -framework Security -o cyberuicache64 -arch arm64
```

For armv7 devices:

```
xcrun --sdk iphoneos clang uicache.m -F /System/Library/PrivateFrameworks -framework MobileContainerManager -framework MobileCoreServices -framework Foundation -framework Security -o cyberuicache -arch armv7
```

Make sure you sign it:

```
ldid -Suicache.plist cyberuicache
```
