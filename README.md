Termboy
=======

Termboy is a Nintendo Game Boy emulator...for your terminal.  
Termboy is based on [Higan](http://byuu.org/emulation/higan/), but uses ncurses to render the screen.

An WIP implementation written in Go is also [available](http://github.com/dobyrch/termboy-go), which requires no external dependencies (not even ncurses!).

Compiling
---------
Required Tools:

* make (GNU)
* g++ (>= 4.7)

Required Libraries:

* alsa
* ncurses

**Note:** The definition for mvaddwstr is in "ncursesw/curses.h" on Ubuntu, but it's just "curses.h" on Arch Linux.  For development purposes, Just create an ncurses folder in /usr/include and symlink ncursesw/curses.h to curses.h.

Usage
-----
After running `make install`, start Termboy by running `termboy <ROM.gb>` in a Linux virtual console.  Use ESDF for the D-pad, G/H for SELECT/START, and J/K for B/A.  Press ESC to quit.

TODO
----
* Allow in-game volume adjustment
* Accept parameters in run script; handle bad input (spaces in arguments)
* Detect terminal resolution
* Color filters
* Continue deleting unused code
* control customization
* Remove/consolidate configuration files
* Consolidate makefiles

Wishlist
--------
* Rewrite in ANSI C
* Reformat code to conform to [Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle)
* Remove gcc-specific code (compile with clang)
* Write man page
* Create Arch PKGBUILD
* Port to other Unices
* Optimize for Raspberry Pi
