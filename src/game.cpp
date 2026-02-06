#include "game.hpp"
#include "graphics/ui.hpp"
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);

	Reset();
}

void Game::Update() {
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

void Game::Reset() {
	playerStats = PlayerStats();
	gameState = GameState::Playing;
	gridManager.InitGrid();
}

void Game::UpdatePlaying() {
	evolutionSystem.Update(playerStats);
	gridManager.Update(playerStats);

	if (playerStats.hp < 0) {
		gameState = GameState::Lose;
	}

	std::cout << "HP: " << playerStats.hp << "/" << playerStats.maxHp
			  << " | Evo: " << playerStats.currentPointsToEvo << "/"
			  << playerStats.pointsToEvo << std::endl;
}

void Game::RenderPlaying() {
	gridManager.Render(&textureManager, playerStats);
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
