#include "game.hpp"
#include "entity/passiveItem.hpp"
#include "graphics/ui.hpp"
#include <memory>
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);
	itemChooser.Init();
	Reset();
}

void Game::Update() {
	inputManager.Update(GetFrameTime());
	if (IsKeyDown(KEY_L)) {
		inputManager.LockFor(5);
	}
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
	gridManager.Update(playerStats, ui, inputManager);

	if (IsKeyPressed(KEY_Y))
		playerStats.passiveItems.push_back(std::make_unique<Regen>());

	if (IsKeyPressed(KEY_U))
		playerStats.passiveItems.push_back(std::make_unique<PointsToEvo>());

	if (IsKeyPressed(KEY_I))
		playerStats.isChoosePending = true;

	if (IsKeyPressed(KEY_O))
		playerStats.passiveItems.push_back(
			std::make_unique<UncoverRandomRare>());

	if (playerStats.hp < 0) {
		ui.TriggerMessageBox("You have lost! Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (playerStats.shoudlNewLevelStart) {
		playerStats.shoudlNewLevelStart = false;
		gridManager.InitGrid(playerStats, gameMode);
	}

	std::cout << "clicks:     " << playerStats.curretTurn << std::endl;

	// std::cout << "HP: " << playerStats.GetHp() << "/" <<
	// playerStats.GetMaxHp()
	// 		  << " | Evo: " << playerStats.currentPointsToEvo << "/"
	// 		  << playerStats.pointsToEvo << std::endl;
	passiveItemManager.Update(gridManager.GetGrid(), playerStats);
	playerStats.EveryFrameReset();
}

void Game::RenderPlaying() {
	gridManager.Render(&textureManager, playerStats, gameMode);
	ui.RenderUi(playerStats, gameState);
	if (playerStats.isChoosePending) {
		itemChooser.Render(playerStats);
	}
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
