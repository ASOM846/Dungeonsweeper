#include "game.hpp"
#include "graphics/ui.hpp"
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);
	Reset();
}

void Game::Update() {
	inputManager.Update(GetFrameTime());

	if (IsKeyDown(KEY_L)) {
		inputManager.LockFor(5);
	}

	switch (gameState) {
	case GameState::Playing:
		UpdatePlaying();
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

void Game::InitClassicGame() {
	gameMode = GameMode::Classic;
}

void Game::InitEndlessGame() {
	gameMode = GameMode::Endless;
}

void Game::Reset() {
	playerStats = PlayerStats();
	ui.CloseMessageBox();
	gameState = GameState::Playing;
	gridManager.InitGrid(playerStats, gameMode);
}

void Game::UpdatePlaying() {
	if (ui.IsMessageBoxOpen()) {
		return;
	}

	ui.UpdateInventoryClick(playerStats);

	evolutionSystem.Update(playerStats);
	gridManager.Update(playerStats, ui, inputManager);

	if (IsKeyPressed(KEY_Y))
		playerStats.inventory.push_back(Item(Item::ItemType::HpUp));

	if (IsKeyPressed(KEY_U))
		playerStats.inventory.push_back(Item(Item::ItemType::Uncover2x2));

	if (playerStats.hp < 0) {
		ui.TriggerMessageBox("You have lost! Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (playerStats.shoudlNewLevelStart) {
		playerStats.shoudlNewLevelStart = false;
		gridManager.InitGrid(playerStats, gameMode);
	}

	std::cout << "HP: " << playerStats.GetHp() << "/" << playerStats.GetMaxHp()
			  << " | Evo: " << playerStats.currentPointsToEvo << "/"
			  << playerStats.pointsToEvo << std::endl;
}

void Game::RenderPlaying() {
	gridManager.Render(&textureManager, playerStats, gameMode);
	ui.RenderUi(playerStats, gameState);
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
