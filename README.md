termboy
=======

A Game Boy emulator for your terminal

Compiling
---------
Required Tools:
* g++ (>4.7)

Required Libraries:
* ncurses
* alsa

Weird Stuff
-----------

The defintion for mvaddwstr is in ncurses/curses.h on Ubuntu, but it's just curses.h on Arch Linux.  For development purposes, Just create an ncurses folder in /usr/include and symlink ncurses/curses.h to curses.h.

TODO
----
* Keyboard input fix (multipress & key up/down events)
* Center display on screen
* Restore terminal settings after exit
* Accept parameters in run script; handle bad input
* Rename executable to 'termboy'
* GB Color support
* Continue deleting unused code
* Remove configuration files
* Check for boot.rom
