#include "gridManager.hpp"

void GridManager::InitGrid(PlayerStats &playerStats, GameMode gameMode) {
	gridGenerator.Init(grid, playerStats, gameMode);
}

void GridManager::Update(PlayerStats &playerStats, UI &ui) {
	gridInterpreter.Update(grid, playerStats, ui);
}

void GridManager::Render(const TextureManager *textureManager,
						 const PlayerStats &playerStats, GameMode gameMode) {
	gridRender.RenderGrid(textureManager, grid, gameMode);
}
