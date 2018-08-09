
Debian
====================
This directory contains files used to package amnesiad/amnesia-qt
for Debian-based Linux systems. If you compile amnesiad/amnesia-qt yourself, there are some useful files here.

## amnesia: URI support ##


amnesia-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install amnesia-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your amnesiaqt binary to `/usr/bin`
and the `../../share/pixmaps/amnesia128.png` to `/usr/share/pixmaps`

amnesia-qt.protocol (KDE)

