#include "game.hpp"
#include "entity/passiveItem.hpp"
#include "gameMode.hpp"
#include "graphics/ui.hpp"
#include "grid/grid.hpp"
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);
	itemChooser.Init();
	Reset();
}

void Game::Update() {
	inputManager.Update(GetFrameTime());
	switch (gameState) {
	case GameState::Playing:
		if (playerStats.isChoosePending) {
			itemChooser.Update(playerStats);
		} else {
			UpdatePlaying();
		}
		break;
	case GameState::Lose:
		UpdateLose();
		break;
	default:
		return;
	}
}

void Game::Render() {
	switch (gameState) {
	case GameState::Playing:
		RenderPlaying();
		break;
	case GameState::Lose:
		RenderLose();
		break;
	default:
		return;
	}
}

void Game::Reset() {
	playerStats = PlayerStats();
	ui.CloseMessageBox();
	gameState = GameState::Playing;
}

void Game::RunGame(Difficulty diff) {
	currentDiff = diff;
	gridManager.InitGrid(playerStats, gameMode, diff);
}

void Game::UpdatePlaying() {
	if (ui.IsMessageBoxOpen()) {
		return;
	}

	evolutionSystem.Update(playerStats);
	if (!playerStats.isSelectionPopup)
		gridManager.Update(playerStats, ui, inputManager);

	popup.Update(gridManager.GetGrid(), playerStats);

	if (playerStats.hp < 0) {
		ui.TriggerMessageBox("You have lost! \n Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (playerStats.gameWon) {
		ui.TriggerMessageBox("You have won! \n Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (IsKeyPressed(KEY_C)) {
		for (size_t y = 0; y < gridManager.GetGrid().GetHeight(); ++y) {
			for (size_t x = 0; x < gridManager.GetGrid().GetWidth(); ++x) {
				if (gridManager.GetGrid().cells[y][x].specialFunction ==
					Grid::SpecialFunction::UncoverEnemiesVal1)
					gridManager.GetGrid().cells[y][x].state =
						Grid::CellState::Revealed;
			}
		}
	}

	if (IsKeyPressed(KEY_H))
		playerStats.currentPointsToEvo += 50;

	passiveItemManager.Update(gridManager.GetGrid(), playerStats);
	playerStats.EveryFrameReset();
}

void Game::RenderPlaying() {
	gridManager.Render(&textureManager, playerStats, gameMode);
	ui.RenderUi(playerStats, gameState);

	if (playerStats.isChoosePending) {
		itemChooser.Render(playerStats);
	}

	popup.Render(playerStats);
}

void Game::UpdateLose() {
	if (IsKeyDown(KEY_R)) {
		gameState = GameState::ShoudlReturnToMenu;
		gridManager.Clear();
	}
}

void Game::RenderLose() {
	RenderPlaying();
}
