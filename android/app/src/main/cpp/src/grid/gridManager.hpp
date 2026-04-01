#pragma once

#include "../entity/passiveItem.hpp"
#include "../entity/playerStats.hpp"
#include "../gameMode.hpp"
#include "../inputManager.hpp"
#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridGenerator.hpp"
#include "gridInterpreter.hpp"
#include "gridRender.hpp"
#include <raylib.h>

class GridManager {
  public:
	GridManager() = default;
	~GridManager() = default;

	void InitGrid(PlayerStats &playerStats, GameMode gameMode,
				  Difficulty difficulty);
	void Update(PlayerStats &playerStats, UI &ui, InputManager &inputManager);
	void Render(const TextureManager *textureManager, PlayerStats &playerStats,
				GameMode gameMode);
	void Clear(Grid *grid);
	void Clear();
	Grid &GetGrid() { return *currentGrid; }

  private:
	Grid *currentGrid;

	GridRender gridRender;
	GridInterpreter gridInterpreter;
	GridGenerator gridGenerator;
};
