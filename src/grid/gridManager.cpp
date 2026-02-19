#include "gridManager.hpp"

void GridManager::InitGrid(PlayerStats &playerStats, GameMode gameMode) {
	currentGrid = new Grid(13, 10, nullptr);
	gridGenerator.Init(*currentGrid, playerStats, gameMode);
}

void GridManager::Update(PlayerStats &playerStats, UI &ui,
						 InputManager &inputManager) {
	gridInterpreter.Update(currentGrid, playerStats, ui, inputManager);
}

void GridManager::Render(const TextureManager *textureManager,
						 const PlayerStats &playerStats, GameMode gameMode) {
	gridRender.RenderGrid(textureManager, *currentGrid, gameMode);
}

void GridManager::Clear(Grid *grid) {
	if (!grid)
		return;
	for (int x = 0; x < grid->GetWidth(); ++x) {
		for (int y = 0; y < grid->GetHeight(); ++y) {
			Vector2 pos = {static_cast<float>(x), static_cast<float>(y)};
			Grid::Cell &cell = grid->GetCell(pos);
			if (cell.LowerGrid) {
				Clear(cell.LowerGrid);
				cell.LowerGrid = nullptr;
			}
		}
	}
	delete grid;
}
