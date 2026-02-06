#include "game.hpp"

void Game::Init() {
	// Ensure player stats are value-initialized to sensible defaults
	playerStats = PlayerStats();

	textureManager.loadAll();
	gridManager.Init();
	ui.Init(&textureManager);
}

void Game::Update() {
	evolutionSystem.Update(playerStats);
	gridManager.Update(playerStats);

	std::cout << "HP: " << playerStats.hp << "/" << playerStats.maxHp
			  << " | Evo: " << playerStats.currentPointsToEvo << "/"
			  << playerStats.pointsToEvo << std::endl;
}

void Game::Render() {
	gridManager.Render(&textureManager, playerStats);
	ui.RenderUi(playerStats);
}
