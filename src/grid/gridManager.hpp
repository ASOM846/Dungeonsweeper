#pragma once

#include "../entity/playerStats.hpp"
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

	void Init();
	void Update(PlayerStats &playerStats);
	void Render(const TextureManager *textureManager,
				const PlayerStats &playerStats);

  private:
	Grid grid;

	GridRender gridRender;
	GridInterpreter gridInterpreter;
	GridGenerator gridGenerator;
};
