# DungeonSweeper

<p align="center">
  <strong>Welcome to DungeonSweeper. An 2D minesweeper like game using C++ and Raylib.</strong>
</p>

## Building the project

```
  sudo pacman -Syu --needed git base-devel cmake ninja ccache sccache pkgconf raylib
  git clone https://github.com/ASOM846/Dungeonsweeper.git
  cd Dungeonsweeper
  cmake -S . -B build -G Ninja
  cmake --build build --parallel $(nproc)
```

Copy assets folder to your build directory and run

`./build/mineswepper`

## To-Do

### High priority

#### Code

- [x] Popup flag menu
- [x] Fix web build

#### Gameplay

- [x] Working passive abilities
- [ ] Working classic mode with score
- [ ] Add more passive abilities

---

### Low priority

#### Code

- [ ] Switch from manual to unique pointers
- [ ] Change hardcoded values to const
- [ ] Change if statements to switch cases in grid managers

#### Gameplay

- [ ] Different bosses/modes
- [ ] Quests

## Bugs

- [ ] Clicks are counted when changing grids
