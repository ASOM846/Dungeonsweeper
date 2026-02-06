#include "game.hpp"

void Game::Init() {
	textureManager.loadAll();
	gridManager.Init();
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
	shoudlReturnToMenu = false;
	gameState = GameState::Playing;
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
	ui.RenderUi(playerStats, false);
}

void Game::UpdateLose() {
	UpdatePlaying();
}

void Game::RenderLose() {
	RenderPlaying();
	std::cout << "LOSE" << std::endl;
}
