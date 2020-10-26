![test](https://github.com/ryukinix/hazard-memory/workflows/test/badge.svg)

# hazard-memory
An experimental  SDL2 Platform Game written in C++

# Requirements

- [SDL2]
- [SDL2_gfx]
- [g++]

[SDL2]: https://www.libsdl.org/download-2.0.php
[SDL2_gfx]: https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/index.html
[g++]: https://gcc.gnu.org/

# Roadmap
## Game Engine

- [ ] Setup basic scaffold.
- [ ] Implementation of a ECS system.
- [ ] Implementation of Hero, NPCs, Items and Enemies Entities.
- [ ] Implementation of Hero, NPCs, Items and Enemies Behaviours.
- [ ] Implementation of Animation system (Using Quads).
- [ ] Implementation of GUI basics, made to be easily extensible (Animation is optional).
- [ ] Implementation of Configuration files system load. (JSON, .INI or a created encrypted file extension)
- [ ] Implementation of Configuration in-game setup (Resolution scaling, Input configuration: Joystick or Keyboard).
- [ ] Implementation of Conversation load files and system of control and render flow. (Text could be animated).
- [ ] Implementation of Tile system. (Including parsing and load files with levels data).
- [ ] Implementation of Basic collision system.
- [ ] Implementation of a Battle system.
- [ ] Implementation of Items bag and usage.
- [ ] Implementation of Gameplay Load/Save system. (Checkpoint or Save State system).

## Game Design

- [ ] Define a theme for the world.
- [ ] Define a main plot.
- [ ] Draw graphical user interface assets.
- [ ] Draw assets for the hero, enemies(including bosses), and stages.
- [ ] Draw assets for items and armor.
- [ ] Draw assets for the world and dungeons.
- [ ] Draw a game ending scene.
