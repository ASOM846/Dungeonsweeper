#include "gridManager.hpp"

void GridManager::InitGrid() {
	gridGenerator.Init(grid);
}

void GridManager::Update(PlayerStats &playerStats) {
	gridInterpreter.Update(grid, playerStats);
}

void GridManager::Render(const TextureManager *textureManager,
						 const PlayerStats &playerStats) {
	gridRender.RenderGrid(textureManager, grid);
}
