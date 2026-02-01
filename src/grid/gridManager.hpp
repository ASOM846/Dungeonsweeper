#pragma once

#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridGenerator.hpp"
#include "gridRender.hpp"
#include <raylib.h>

class GridManager {
  public:
	GridManager() = default;
	~GridManager() = default;

	void Init();
	void Update();
	void Render(const TextureManager *textureManager);

	int hp = 4;
	int maxHp = 4;
	int pointsToEvo = 0;

  private:
	Grid grid;
	GridRender gridRender;
	GridGenerator gridGenerator;
};