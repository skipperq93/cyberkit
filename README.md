# CyberKit

CyberKit is a backport of WebKit to older iOS.

## Development Branch History
* [safari-7615.1.15-branch](https://github.com/UInt2048/CyberKit/tree/safari-7615.1.15-branch): 31 January 2023 - 24 February 2023
    * _Diverged 11 December 2022; built in Xcode 14.2_
* [safari-7616.1.3-branch](https://github.com/UInt2048/CyberKit/tree/safari-7616.1.3-branch): 25 February 2023
    * _Diverged 18 February 2023; built in Xcode 14.2_
* [safari-7616.1.4pre-branch](https://github.com/UInt2048/CyberKit/tree/safari-7616.1.4pre-branch): 25 February 2023 - 5 March 2023
    * _Diverged 25 February 2023; built in Xcode 14.2_
* [safari-7616.1.4-branch](https://github.com/UInt2048/CyberKit/tree/safari-7616.1.4-branch): 5 March 2023 - 24 March 2023
    * _Diverged 25 February 2023; built in Xcode 14.2_
* [safari-7616.1.7-branch](https://github.com/UInt2048/CyberKit/tree/safari-7616.1.7-branch): 24 March 2023 - 11 May 2023
    * _Diverged 19 March 2023; built in Xcode 14.2 before [fa2170c](https://github.com/UInt2048/CyberKit/commit/fa2170c3604b4ccacbc4f2475f91c0638ac7a2a0) (committed 7 April 2023), 14.3 starting with fa2170c_
* [safari-7616.1.12-branch](https://github.com/UInt2048/CyberKit/tree/safari-7616.1.12-branch): 11 May 2023 - present
    * _Diverged 22 April 2023; built in Xcode 14.3_

## Reference Implementations
* [safari-607-branch](https://github.com/UInt2048/CyberKit/tree/safari-607-branch): 19 May 2023 - 21 July 2023
    * _Diverged 7 January 2019; built in Xcode 11.7_
* [safari-608-branch](https://github.com/UInt2048/CyberKit/tree/safari-608-branch): 21 July 2023 - 22 July 2023
    * _Diverged 15 July 2019; built in Xcode 11.7_
* [safari-609-branch](https://github.com/UInt2048/CyberKit/tree/safari-609-branch): 22 July 2023 - 23 July 2023
    * _Diverged 3 January 2020; built in Xcode 11.7_
* [safari-610-branch](https://github.com/UInt2048/CyberKit/tree/safari-610-branch): 23 July 2023
    * _Diverged 21 August 2020; built in Xcode 11.7_

## Building from Source
If building yourself, you may wish to use the version of Xcode used at the time to build the commit you are building.

Ensure to run `Tools/Scripts/configure-xcode-for-embedded-development` before 1st build or any time you reinstall/update Xcode or its SDKs.

You will build the "Everything up to CyberKit" target then the appropriate app.

### Development Environment Notes

* Development is performed on macOS using Xcode.
* The rename script requires `gnu-sed` and `rename`, which can be obtained using Homebrew.
* Beginning with [fa2170c](https://github.com/UInt2048/CyberKit/commit/fa2170c3604b4ccacbc4f2475f91c0638ac7a2a0) [7 April 2023], Xcode 14.3 is used.
* Before this, Xcode 14.2 was used.

## Official OS Support
Definitions:

|  Symbol  | Definition
| :------: | :-------
| &#x2705; | Supported
| &#x274e; | Partially supported (one installation method fails)
| &#x2753; | Unknown behavior (please confirm)
| &#x23f3; | Not supported (installation always succeeds, but CyberKit fails)
| &#x23f0; | Not supported (installation exists, but never succeeds)
| &#x274c; | Not supported
| + | Rootful
| - | Rootless

This table only applies if you are jailbroken or have TrollStore.

Please note that iOS and iPadOS are considered equivalent in CyberKit.

Please note that your mileage may vary from the official values in the table. Please report any discrepancies.


| Release Name |  iOS 12  |  iOS 13  |  iOS 14  | iOS 15+  | iOS 15-  | iOS 16+  | iOS 16-
| :----------: | :------: | :------: | :------: | :------: | :------: | :------: | :------:
| v0.0.1 alpha | &#x274c; | &#x274c; | &#x2705; | &#x2705; | &#x23f0; | &#x2705; | &#x23f0;
| v0.0.2 alpha | &#x274c; | &#x274c; | &#x2705; | &#x2705; | &#x23f0; | &#x2705; | &#x23f0;
| v0.0.3 alpha | &#x274c; | &#x274c; | &#x2705; | &#x2705; | &#x274e; | &#x2705; | &#x274e;
| v0.0.4 alpha | &#x274c; | &#x274c; | &#x2705; | &#x2705; | &#x274e; | &#x2705; | &#x274e;
| v0.0.5 alpha | &#x274c; | &#x23f3; | &#x2705; | &#x2705; | &#x274e; | &#x2705; | &#x274e;
| v0.0.6 alpha | &#x274c; | &#x23f3; | &#x2705; | &#x2705; | &#x274e; | &#x2705; | &#x274e;
| v0.0.7 alpha | &#x23f3; | &#x23f3; | &#x2705; | &#x2705; | &#x2753; | &#x2705; | &#x2753;
| v0.0.8 alpha | &#x2705; | &#x2705; | &#x2705; | &#x2705; | &#x2753; | &#x2705; | &#x2753;
| (Dev branch) | &#x2705; | &#x2705; | &#x2705; | &#x2705; | &#x2753; | &#x2705; | &#x2753;

## Installation
Please visit [the releases](https://github.com/UInt2048/CyberKit/releases).
