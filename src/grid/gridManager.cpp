#include "gridManager.hpp"

void GridManager::InitGrid(PlayerStats &playerStats, GameMode gameMode) {
	currentGrid = new Grid(13, 10, nullptr);
	gridGenerator.Init(*currentGrid, playerStats, gameMode);
}

void GridManager::Update(PlayerStats &playerStats, UI &ui,
						 InputManager &inputManager) {
	gridInterpreter.Update(*currentGrid, playerStats, ui, inputManager);
}

void GridManager::Render(const TextureManager *textureManager,
						 const PlayerStats &playerStats, GameMode gameMode) {
	gridRender.RenderGrid(textureManager, *currentGrid, gameMode);
}
