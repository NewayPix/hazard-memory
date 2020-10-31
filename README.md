![test](https://github.com/ryukinix/hazard-memory/workflows/test/badge.svg)

# hazard-memory
An experimental game engine for 2D games based in SDL2 written in C++. 

# Requirements

- [SDL2]
- [SDL2_gfx]
- [g++]
- [GNU Make]

[SDL2]: https://www.libsdl.org/download-2.0.php
[SDL2_gfx]: https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/index.html
[g++]: https://gcc.gnu.org/
[GNU Make]: https://www.gnu.org/software/make/manual/make.html 

## How to install dependencies

### Ubuntu

```bash
sudo apt install libsdl2-dev libsdl2-gfx-dev gcc
```

### Arch Linux

```bash
sudo pacman -Syu gcc sdl2 sdl2_gfx
```

### Fedora

```bash
sudo dnf install gcc-c++ SDL2-devel SDL2_gfx-devel
```

# How to run examples

Run all examples:

```bash
make examples
```

Run a specific example:

```bash
make examples/collision/square-platform.cpp
```

# License
MIT
