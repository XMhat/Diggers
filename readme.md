# Diggers for Windows v0.83

A fan-remake of the classic strategy and platform game for MS-DOS and the Commodore Amiga.

This version is extremely old, obsolete and for reference only. It is **NOT** the latest version of this game and was only designed for `Windows 9x/XP` 32-bit systems and will not run properly on Windows 7 and later because of Windows Explorer highjacking 8-bit palette changes.

Visit [https://github.com/XMhat/MSEngine/tree/master/diggers](https://github.com/XMhat/MSEngine/tree/master/diggers) for the latest version of this game that works on all modern operating systems!

Originally based on Diggers v1.49 for PC-DOS (Floppy disk version).
Uses music, textures and sound from Amiga CD32 version.

## Requirements

* Pentium 1* class CPU (more recommended).
* 16* megabytes of memory (more recommended).
* 32 or 64-Bit Windows 9x, ME, NT, 2K, XP, Vista.
* DirectX version 3* or later.
* DirectDraw compatible 2D Graphics Accellerator.
* DirectSound compatible sound card.
* DirectInput compatible mouse and keyboard.
* DirectShow MPEG Layer3 filter for MP3 playback.

## Progress

* Sound = 100% (All done)
* Music = 99% (All done, need to ditch DirectShow though)
* Levels = 100% (All converted, populated and done)
* Gameplay = 95% (Pretty much done, few bugs)
* Netplay = 1% (Only WSAStartup() added)
* Front-end = 0% (There are no front-end menus or title screens yet)
* Controls = 95% (Need keyboard controls)
* Graphics = 99% (Some DirectDraw bugs need fixing, if possible?)

## Differences to original version

* Take your Digger home and you can rest it to full health. Costs money.
* High-resolution textures.
* .MP3 addons.

## Limitations and bugs

* No title screens or game menu's, just pure gameplay.
* Currently in Alpha status, may never be finished and will have bugs.
* There are no keyboard shortcuts to control the Diggers yet.
* Initiation of flooding sometimes fails to spawn water.
* Jumping while jumping bugs a little bit.
* FPS issues on different systems because the game doesn't use high-resolution timers. This means a lot of work! :-(.
* If you have ffdShow installed. The system tray gets spammed with ffdShow icons because the game uses DirectShow to play the .mp3's.
* Transparency issues in Windows 7. I have no idea how to fix this. But I think it happens because the graphics driver or something doesn't support 8 bits per pixel textures anymore which this game uses.
* Sometimes when you win or lose the opposite result occurs ;-).
* You can hear teleportation sounds when the level switches.
* Fighting isn't perfect.
* AI is obscenely afraid of dying and teleports out too much.
* No 64-bit support due to no 64-bit UFMod.

## Bugs fixed since last version

* Fixed errors and dialogs not showing up. Made interface to common controls library and modified manifest.
* Workaround on VirtualBox for Mac relating to vertical blank.
* Added this text file.
* Disabled keys used for debugging (cheating!) ;-).

## Commands

* `ESCAPE ` = Quit.
* `F1` = Previous Level.
* `F2` = Next level.
* `1-5` = Select Digger.
* `Mouse` = Move cursor.
* `Left Mouse` = Select object or deselect object.
* `Right Mouse` = Show menu for object.
* `F5` = Config menu.
* `F6` = Toggle MP3 streaming (download .mp3's from website).
* `F7` = Toggle tips.
* `F8` = Toggle sound effects.
* `F9` = Toggle music.
* `F10` = Toggle full-screen/window mode.
* `F11` = Video mode/Texture scale cycler.
* `F12` = Screenshot (.bmp where .exe is).

## Program

You're going to have to build the program yourself, it was designed to be compiled with 32-bit v15 of the Microsoft Visual C++ compiler and linker but it WILL compile on the latest Visual C++ compiler and linker with a few warnings.

Compile: `rc diggers.rc` to build the resource second and then `cl /Ox diggers.cpp diggers.res` to build the executable.

Usage: `DIGGERS.EXE [Parameters]`.

* `/D` = Enable debug mode (Map editor, you can't save maps yet).
* `/d` = Disable debug mode.
* `/E` = Enable sound effects.
* `/e` = Disable sound effects.
* `/Ln` = Start on zone number n.
  * Specify an index between 0 and 33.
* `/M` = Enable music.
* `/m` = Disable music.
* `/Rn` = Start with race number n.
  * 0 = F'Targ (+Stamina)
  * 1 = Habbish (+Intelligence)
  * 2 = Grablin (+Dexterity)
  * 3 = Quarrior (+Strength)
* `/S` = Enable streaming (Requires DirectShow MP3 filter).
* `/s` = Disable streaming.
* `/Vn` = Use alternate video mode n.
  * 0 =  320x240x256 (1X scale, original version)
  * 1 =  640x480x256 (2X scale, default)
  * 2 =  960x720x256 (3X scale)
  * 3 = 1280x960x256 (4X scale, fast 2D GPU needed)
* `/X` = Enable sound mixing.
* `/x` = Disable sound mixing.
* `/?` = Show this list of parameters
* `/!` = Display version information.

## Troubleshoot

Please delete `DIGGERS.DAT` if you have problems running the game and then try again, this is the binary formatted configuration file.

## Credits

* All code and conversion...
  * Copyright © MS-Design, 2011.
* Original version sound, music and graphics....
  * Copyright © Millenium Interactive, 1994.

* Uses `UFMOD` to play `.XM` modules.
  * Copyright © Asterix and Quantum, 2011.
* Uses `ScaleX` to create high resolution textures.
  * Copyright © Andrea Mazzoleni, 2011.
* Uses DirectX 3 with DirectDraw, DirectSound, DirectInput and DirectShow.
  * Copyright © Microsoft Corporation, 2011.

## Licence

See [licence.md](licence.md).
