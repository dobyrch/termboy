termboy
=======

A Game Boy emulator for your terminal

Compiling
---------
Required Tools:
* g++ (>4.7)

Required Libraries:
* ncurses
* X11
* openGL
* Xv
* SDL
* ao
* openAL
* pulseaudio
Note:  Most of these libraries will eventually be removed.  Termboy should only require ncurses and alsa; the other libraries are artifacts from the original emulator that have not yet been stripped out.

Weird Stuff
-----------

The defintion for mvaddwstr is in ncurses/curses.h on Ubuntu, but it's just curses.h on Arch Linux.  For development purposes, Just create an ncurses folder in /usr/include and symlink ncurses/curses.h to curses.h.

The Makefile automatically installs libananke.so.  The Makefiles should be rewritten after we remove all GUI dependencies.

TODO
----
* Keyboard input fix (multipress & key up/down events)
* Center display on screen
* Restore terminal settings after exit
* Accept parameters in run script; handle bad input
* Rename executable to 'termboy'
* Statically link ananke
* GB Color support
* Remove unused libraries
* Continue deleting unused code
* Remove configuration files
* Check for boot.rom
