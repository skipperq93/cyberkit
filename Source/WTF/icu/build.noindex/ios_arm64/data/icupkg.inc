GENCCODE_ASSEMBLY_TYPE=-a gcc-darwin
SO=dylib
SOBJ=dylib
A=a
LIBPREFIX=lib
LIB_EXT_ORDER=.76.1.dylib
COMPILE=cc -DU_ALL_IMPLEMENTATION -DU_ATTRIBUTE_DEPRECATED=   -DU_HAVE_STRTOD_L=1 -DU_HAVE_XLOCALE_H=1  -DU_DISABLE_RENAMING=1 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk -I/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk/usr/include/ -I./include/ -arch arm64 -miphoneos-version-min=11.0 -I/Users/matthewbenedict/Documents/git/CyberKit/Source/WTF/icu/build.noindex/icu/include/ -O3 -std=c11 -Wall -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings   -fno-common -c
LIBFLAGS=-I/Users/matthewbenedict/Documents/git/CyberKit/Source/WTF/icu/icu-clone.noindex/icu/icu4c/source/common -I../common  -dynamic
GENLIB=cc -dynamiclib -dynamic -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk -I/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk/usr/include/ -I./include/ -arch arm64 -miphoneos-version-min=11.0 -I/Users/matthewbenedict/Documents/git/CyberKit/Source/WTF/icu/build.noindex/icu/include/ -O3 -std=c11 -Wall -pedantic -Wshadow -Wpointer-arith -Wmissing-prototypes -Wwrite-strings   -stdlib=libc++ -L/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk/usr/lib/ -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS17.2.sdk -Wl,-dead_strip -miphoneos-version-min=11.0 -lstdc++  
LDICUDTFLAGS=
LD_SONAME=-Wl,-compatibility_version -Wl,76 -Wl,-current_version -Wl,76.1 -install_name  
RPATH_FLAGS=
BIR_LDFLAGS=
AR=ar
ARFLAGS=r -c
RANLIB=ranlib
INSTALL_CMD=/opt/homebrew/bin/ginstall -c
