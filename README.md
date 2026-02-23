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

- [ ] Different characaters (meta progress)
- [ ] PopupInfo
- [ ] Pick up items
- [ ] Item shop and money
- [ ] Quests
