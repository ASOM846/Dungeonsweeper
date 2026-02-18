#pragma once

#include "../entity/playerStats.hpp"
#include "../gameMode.hpp"
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

	void InitGrid(PlayerStats &playerStats, GameMode gameMode);
	void Update(PlayerStats &playerStats, UI &ui);
	void Render(const TextureManager *textureManager,
				const PlayerStats &playerStats, GameMode gameMode);

  private:
	Grid grid = Grid(13, 10);

	GridRender gridRender;
	GridInterpreter gridInterpreter;
	GridGenerator gridGenerator;
};
