#include "game.hpp"
#include "graphics/ui.hpp"
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);
	Reset();
}

void Game::Update() {
	inputLocker.Update(GetFrameTime());

	if (inputLocker.IsLocked())
		std::cout << "input still locked" << std::endl;

	switch (gameState) {
	case GameState::Playing:
		if (!inputLocker.IsLocked())
			UpdatePlaying();
		break;
	case GameState::Lose:
		UpdateLose();
		break;
	default:
		return;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) ||
		IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		inputLocker.LockFor(1.4);
		std::cout << "input locked" << std::endl;
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

	evolutionSystem.Update(playerStats);
	gridManager.Update(playerStats, ui);

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
	}
}

void Game::RenderLose() {
	RenderPlaying();
	std::cout << "LOSE" << std::endl;
}
