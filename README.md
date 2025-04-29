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

*   **C++ Compiler:**
*   **Make:**
*   **SDL2:**

# Install SDL2
brew install sdl2

# To run game: make run ROM=[game_file].gb
#              make run (will run tetris.gb) by default
# To compile:  make
# To clean:    make clean
