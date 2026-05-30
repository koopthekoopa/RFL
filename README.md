Revolution Face Library
=======================

A decompilation of the Revolution Face Library.  

The library is responsible for handling Miis on the Nintendo Wii.  

This repository does not contain any of the original objects. An existing copy of the game [`[SC5PGN]`](https://wiki.dolphin-emu.org/index.php?title=SC5PGN) *Challenge Me: Word Puzzles* is required. (See [Building](#building) below.)

Supported versions:
- 20080306 (from Challenge Me: Word Puzzles; **Default**)
- 20080216 (from Wii Menu 4.3)
- 20070609 (from Wii Sports; USA Rev.1)

> [!WARNING]
> Building is only supported for version **20080306** (Challenge Me: Word Puzzles)  

Progress
========

Currently, the release builds are fully matching! However, a few functions for debug builds are not matching (but are 1:1 logically).  

The following functions not matching for debug builds are:  
* `RFL_MakeTex.c`/`RFLiMakeTexture`
* `RFL_Model.c`/`RFLiInitShapeRes`

Dependencies
============

Windows
--------

On Windows, it's **highly recommended** to use native tooling. WSL or msys2 are **not** required.  
When running under WSL, [objdiff](#diffing) is unable to get filesystem notifications for automatic rebuilds.

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

macOS
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages):

  ```sh
  brew install ninja
  ```

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Linux
------

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).

[wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Building
========

- Clone the repository:

  ```sh
  git clone https://github.com/koopthekoopa/RFL.git
  ```

- Copy your disc image of *Challenge Me: Word Puzzles* to `orig`.
  - Supported formats: ISO (GCM), RVZ, WIA, WBFS, CISO, NFS, GCZ, TGC
  - After the initial build, the disc image can be deleted to save space.

- Configure:

  ```sh
  python configure.py
  ```

  To use a version other than `20080306_DBG`, specify it with `--version`.

- Build:

  ```sh
  ninja
  ```

Diffing
=======

Once the initial build succeeds, an `objdiff.json` should exist in the project root.

Download the latest release from [encounter/objdiff](https://github.com/encounter/objdiff). Under project settings, set `Project directory`. The configuration should be loaded automatically.

Select an object from the left sidebar to begin diffing. Changes to the project will rebuild automatically: changes to source files, headers, `configure.py`, `splits.txt` or `symbols.txt`.

![](assets/objdiff.png)
