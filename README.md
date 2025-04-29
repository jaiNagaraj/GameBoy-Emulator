# GheithBoy Emulator

A Game Boy emulator written in C++ utilizing SDL2 library for graphics.

## Features

*   CPU Emulation (SM83 core)
*   Semi asynchronous main loop
*   PPU (Picture Processing Unit) with background, window, and sprite rendering
*   Timer and Interrupt Handling
*   Input Handling
*   SDL2 for display and input
*   No MBC (can still run multiple games present in this repo)

## Dependencies

*   **C++ Compiler**
*   **Make**
*   **SDL2**

## Install SDL2
* For Mac: brew install sdl2
* For Windows, Visual Studio Community (very complicated, sorry): Follow the guide at [Lazy Foo Productions](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/index.php)
* For Windows, VS Code (also complicated, sorry): Follow the instructions in [this video](https://youtu.be/9Ca-RVPwnBE)

## How to run game (not using VS Code)
* `make run` will run with tetris.gb by default
* To compile:  make
* To clean:    make clean
* To run with specific ROM: `make run <game_file>.gb`
