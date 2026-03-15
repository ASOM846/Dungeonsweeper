# DungeonSweeper

<p align="center">
  <strong>Welcome to DungeonSweeper — a 2D dungeon-themed Minesweeper-like game built with C++ and Raylib.</strong>
</p>

## Building the project

**Arch Linux**
```sh
sudo pacman -Syu --needed git base-devel cmake ninja ccache sccache pkgconf raylib
git clone https://github.com/ASOM846/Dungeonsweeper.git
cd Dungeonsweeper
cmake -S . -B build -G Ninja
cmake --build build --parallel $(nproc)
```

**Other Linux (no system raylib package)**
```sh
git clone --recurse-submodules https://github.com/ASOM846/Dungeonsweeper.git
cd Dungeonsweeper
cmake -S . -B build -G Ninja -DRAYLIB_USE_SUBMODULE=ON
cmake --build build --parallel $(nproc)
```

Then copy the `assets/` folder next to the binary and run:

```sh
cp -r assets/ build/
./build/dungeonsweeper
```

## How to play

| Action | Input |
|--------|-------|
| Reveal cell | Left click |
| Flag dangerous cell | Right click |
| Collect evolution points | Left click on a defeated enemy |
| Evolve (when bar is full) | `E` |
| Return to menu (on lose screen) | `R` |

- Defeat enemies to earn evolution points. Evolve to increase your max HP.
- Collect keys to open chests. Chests grant evolution points and coins.
- Find all three swords before entering the Mini-Dungeon.
- **Classic mode**: find and defeat the Wizard to win.
- **Endless mode**: climb the Ladder to reach ever-harder floors.

## Architecture overview

```
main.cpp
  └─ WindowManager          core loop, switches between Menu and Game
       ├─ Menu              main-menu state machine + responsive button layout
       └─ Game              top-level gameplay coordinator
            ├─ GridManager          owns the current Grid pointer
            │    ├─ GridGenerator   procedural level generation
            │    ├─ GridInterpreter handles all click logic & cell transitions
            │    └─ GridRender      draws every cell, enemy, and item sprite
            ├─ PlayerStats          plain data struct (HP, keys, inventory…)
            ├─ EvolutionSystem      manages the evolution threshold & stat scaling
            ├─ PassiveItemManager   ticks passive items every N player turns
            ├─ ItemChooser          popup UI for picking a passive item on evolution
            ├─ InputManager         global click-lock to prevent double-processing
            ├─ TextureManager       loads and caches all PNG assets at startup
            └─ UI                   HUD (HP bar, evo bar, keys, inventory)
```

### Key data types

| Type | File | Purpose |
|------|------|---------|
| `Grid` | `src/grid/grid.hpp` | 2-D array of `Cell`s; holds width/height, grid type, pointer to parent grid |
| `Grid::Cell` | `src/grid/grid.hpp` | One tile: state, value, special function, texture, flag, child grid pointer |
| `Grid::CellState` | `src/grid/grid.hpp` | `Hidden` → `Revealed` → `pointsNotTaken` → `Hinting` |
| `Grid::SpecialFunction` | `src/grid/grid.hpp` | What a cell *is*: monster, sword, chest, ladder, mini-dungeon entry, … |
| `PlayerStats` | `src/entity/playerStats.hpp` | All mutable player state; passed by reference everywhere |
| `PassiveItem` | `src/entity/passiveItem.hpp` | Abstract base; `Regen`, `PointsToEvo`, `UncoverRandomRare` |

### Click flow (the most important path to understand)

```
Mouse click
  └─ GridInterpreter::Update()
       ├─ bounds-check mouse → grid coordinates
       ├─ right-click  → toggle flag
       └─ left-click   → dispatch on cell.state
            ├─ Hidden          → OnHiddenClick  (reveal or forward to OnRevealedClick)
            ├─ Revealed        → OnRevealedClick (apply special function: heal, sword, chest, dungeon…)
            ├─ pointsNotTaken  → OnPointsNotTakenClick (collect evo points)
            └─ Hinting         → no-op
```

## To-Do

- [ ] Different characters (meta progression)
- [ ] Pick up active items
- [ ] Quests
- [ ] Popup info on hover (code exists in `GridRender`, currently commented out)
- [ ] Win state rendering (`UpdateWin` / `RenderWin` are stubbed — only Classic mode needs this)
- [ ] Settings screen (button exists in the menu, state machine entry is present, no content yet)
- [ ] Replace raw `Grid*` pointer in `GridManager` with `std::unique_ptr<Grid>`
- [ ] Add unit tests for grid generation and evolution logic

## Bugs

- [ ] Mini-dungeon: clicking on `GoUpGrid` still increments the turn counter (click-lock fires after the grid swap)
- [ ] `GridManager::Clear()` is recursive and manually `delete`s — can leak if an exception is thrown mid-tree
