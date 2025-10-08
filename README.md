Revolution Face Library
=======================

A decompilation of the Revolution Face Library (rev. March 6 2008). The library is responsible for handling Miis on the Nintendo Wii.  

This repository is based on left over libraries in [Challenge Me: Word Puzzles](https://wiki.dolphin-emu.org/index.php?title=SC5PGN)
* `build/libs/Release/RVLFaceLib.a`
* `build/libs/Debug/RVLFaceLibD.a`

Progress
--------

✔️ = Matching  
❌ = Not matching  

|          Source         | Debug | Release |
|-------------------------|-------|---------|
| `RFL_System.c`          | ❌ | ✔️ |
| `RFL_NANDLoader.c`      | ✔️ | ✔️ |
| `RFL_NANDAccess.c`      | ❌ | ❌ |
| `RFL_Model.c`           | ❌ | ❌ |
| `RFL_MakeTex.c`         | ❌ | ❌ |
| `RFL_Icon.c`            | ❌ | ❌ |
| `RFL_HiddenDatabase.c`  | ❌ | ❌ |
| `RFL_Database.c`        | ❌ | ❌ |
| `RFL_Controller.c`      | ❌ | ❌ |
| `RFL_MiddleDatabase.c`  | ❌ | ❌ |
| `RFL_MakeRandomFace.c`  | ❌ | ❌ |
| `RFL_DefaultDatabase.c` | ❌ | ❌ |
| `RFL_DataUtility.c`     | ✔️ | ✔️ |
| `RFL_NWC24.c`           | ❌ | ❌ |
| `RFL_Format.c`          | ✔️ | ✔️ |

Resources used
--------------
[doldecomp/dolsdk2004](https://www.github.com/doldecomp/dolsdk2004)  
[kiwi515/ogws](https://www.github.com/kiwi515/ogws)  
