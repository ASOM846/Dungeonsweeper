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
	void Update(int &hp, int &maxHp, int &pointsToEvo);
	void Render(const TextureManager *textureManager, int hp, int pointsToEvo, int maxHp);



  private:
	Grid grid;
	GridRender gridRender;
	GridGenerator gridGenerator;
};