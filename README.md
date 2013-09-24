termboy
=======

A Game Boy emulator for your terminal


Weird Stuff
----------

The defintion for mvaddwstr is in ncurses/curses.h on Ubuntu, but it's just curses.h on Arch Linux.  For development purposes, Just create an ncurses folder in /usr/include and symlink ncurses/curses.h to curses.h.

The Makefile automatically installs libananke.so.  The Makefiles should be rewritten after we remove all GUI dependencies.
