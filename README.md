A simple game engine written entirely in C++ as part of the Game Engineering course at EAE, University of Utah.

This project is a work in progress and currently contains the following modules:

Thread-safe Memory Management System:
-------------------------------------
- Provides a variable size allocator with byte alignment & defragmentation
- Provides a fixed size allocator for quick allocations with low overhead
- Provides overwrite detection by guardbanding memory for debug builds
- Provides a smart pointer suite, complete with implementation for shared, weak & unique pointers
- Provides usage statistics with respect to count and size of incoming allocations

Multi-threaded Job System:
--------------------------
- Provides multiple job queues, each with multiple workers
- Each worker contains an std::thread and waits for jobs to be posted in the corresponding queue
- Any kind and amount of jobs can be added to the job system, as long as a given job implements InterfaceJob.

Timer and Time-based Events:
----------------------------
- An updater that manages broadcasting the tick to all entities that need it
- The updater also maintains a list of timer events and broadcasts them
- A timer event contains a function object. It invokes this function object after a specified duration either once, multiple times or till it is destroyed

2D Collision System:
--------------------
- Provides 2D collision detection using a swept separating axis collision test
- Provides classification of physics objects using enum types and collision filtering using bit masks
- Provides default collision response using reflection
- Provides callbacks on collision detection

Miscellaneous:
--------------
- A logging and asserting utility
- A pooled and hashed strings utility
- A primer for a game object with corresponding geometry & *very* simple physics
- A set of optimized geometry classes using Intel SSE instructions
- A lua-parser for data driven development
- A thread-safe file caching utility

This project also contains a simple space invaders clone as a test harness for the engine in addition to module specific unit tests.

This project needs at least Visual Studio 2015 to compile and run.

Gameplay & Instructions:
------------------------
- You control a white ship that automatically shoots bullets.
- The objective of the game is to kill all the colored enemies on the screen.
- The game contains five levels, each increasing in difficulty.
- You have infinite ships but if your ship takes damage from a single enemy bullet, your level will restart.
- Completing the fifth level starts you back at the first level instead of quitting.
- Use the left and right arrow keys to move your ship.
- Hitting 'P' pauses the game.
- Hitting 'R' and 'Q' while the game is paused causes the level to restart and the game to quit respectively.