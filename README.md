# Retro Snake (C++ & Raylib)

A retro-styled grid Snake game built in C++ using the [Raylib](https://www.raylib.com/) library.

## Features & Additions

* **FIFO Input Queue:** Decoupled 60 FPS key-polling from the 0.2s movement tick using a `std::deque` buffer. Prevents rapid multi-key presses from causing illegal 180° self-collisions.
* **Xenzia Style Screen Wrapping:** Nokia-style boundary wrapping across all grid borders.
* **Score & Game Over State Machine:** Reset loop handling score tracking, food respawn collision validation, and game-over overlays without restarting the binary.

## Controls

| Key | Action |
|---|---|
| `Up` / `Down` / `Left` / `Right` | Steer Snake |
| `Space` / `Enter` | Restart (after Game Over) |
| `Escape` | Exit Game |

## Built With

* **Language:** C++
* **Framework:** [Raylib](https://www.raylib.com/)
* **Build System:** MinGW-w64 / Make

## Building & Running

### Prerequisites
* MinGW-w64 compiler
* Raylib installed (or using the pre-configured w64devkit environment)

### Run via VS Code
1. Open the project via `main.code-workspace`.
2. Press `F5` to build and run with debugging, or `Ctrl + F5` to run directly.

## Acknowledgments
* Base workspace configuration based on the [Raylib C++ Starter Template](https://github.com/educ8s/Raylib-CPP-Starter-Template-for-VSCODE) by **educ8s**.
* Inspired by the tutorial series by **Programming With Nick**.