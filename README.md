Termboy
=======

Termboy is a Nintendo Game Boy emulator...for your terminal.  
Termboy is based on [Higan](http://byuu.org/higan/), but uses ncurses to render the screen.

Compiling
---------
Required Tools:

* make
* g++ (>= 4.7)

Required Libraries:

* alsa
* ncurses

**Note:** The definition for mvaddwstr is in "ncurses/curses.h" on Ubuntu, but it's just "curses.h" on Arch Linux.  For development purposes, Just create an ncurses folder in /usr/include and symlink ncurses/curses.h to curses.h.

Usage
-----
After running `make install`, start Termboy by running `termboy <ROM.gb>` in a Linux virtual console.  Use ESDF for the D-pad, G/H for SELECT/START, and J/K for B/A.  Press ESC to quit.

TODO
----
* Allow in-game volume adjustment
* Accept parameters in run script; handle bad input (spaces in arguments)
* Detect terminal resolution
* Color filters
* Remove configuration files (low priority)
* Continue deleting unused code
* control customization 

Wishlist
--------
* Rewrite in ANSI C
* Reformat code to conform to [Linux kernel coding style](https://www.kernel.org/doc/Documentation/CodingStyle)
* Remove GCC-specific code
* Write man page
* Create Arch PKGBUILD
* Port to other Unices
* Optimize for Raspberry Pi
* Consolidate makefiles
