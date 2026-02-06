#pragma once
#include "../entity/playerStats.hpp"
#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridUtils.hpp"
#include <raylib.h>

class GridRender {
  public:
	GridRender() = default;
	~GridRender() = default;

	void setPtr(const TextureManager &tm) { textureManager = &tm; }

	void RenderGrid(TextureManager const *textureManager, Grid &grid);
	void DrawEnemy(TextureManager const *textureManager, const Vector2 position,
				   const int type);

  private:
	void HidenCellRender();
	void ReveledCellRender(int x, int y, Grid &grid, Vector2 offset);
	void PointsNotTakenCellRender(int x, int y, Grid &grid, Vector2 offset);
	void HintingCellRender(int x, int y, Grid &grid, Vector2 offset,
						   float scale);
	void StartingCellRender(int x, int y, Grid &grid, Vector2 offset);

	const TextureManager *textureManager = nullptr;
};
