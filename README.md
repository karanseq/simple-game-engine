A simple game engine written entirely in C++ as part of the Game Engineering course at EAE, University of Utah.
This project is a work in progress and currently contains the following modules:
- A logging & asserting utility
- A primer for a game object in 3D space with corresponding geometry
- A memory management system with the following features:
	- Provides a variable size allocator with byte alignment & defragmentation
	- Provides a fixed size allocator for quick allocations with low overhead
	- Provides overwrite detection by guardbanding memory for debug builds

This project needs at least Visual Studio 2015 to compile and run.