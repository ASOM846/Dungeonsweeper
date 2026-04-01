#include "gridManager.hpp"
#include "../gameMode.hpp"

void GridManager::InitGrid(PlayerStats &playerStats, GameMode gameMode,
						   Difficulty difficulty) {
	(void)gameMode;

	currentGrid = new Grid(13, 10, nullptr);

	GridConfig config = GetGridConfig(difficulty);

	gridGenerator.Init(*currentGrid, playerStats, gameMode, config);
}

void GridManager::Update(PlayerStats &playerStats, UI &ui,
						 InputManager &inputManager) {
	gridInterpreter.Update(currentGrid, playerStats, ui, inputManager);
}

void GridManager::Render(const TextureManager *textureManager,
						 PlayerStats &playerStats, GameMode gameMode) {
	gridRender.RenderGrid(textureManager, *currentGrid, gameMode, playerStats);
}

void GridManager::Clear(Grid *grid) {
	if (!grid)
		return;

	while (currentGrid->UpperGrid != nullptr) {
		currentGrid = currentGrid->UpperGrid;
	}

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

void GridManager::Clear() {
	Clear(currentGrid);
}
