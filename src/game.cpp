#include "game.hpp"
#include "entity/passiveItem.hpp"
#include "entity/playerStats.hpp"
#include "gameMode.hpp"
#include "graphics/ui.hpp"
#include "grid/grid.hpp"
#include "utils/randomUtils.hpp"
#include <iostream>
#include <raylib.h>

void Game::Init() {
	textureManager.loadAll();
	ui.Init(&textureManager);
	itemChooser.Init();
	popup.Init();
	Reset();
	std::cout << "zainicjowano Game Init\n";
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
	ui.CloseMessageBox();
	gameState = GameState::Playing;
	std::cout << "zresetowano gre Game::Reset() \n";
}

void Game::RunGame(GameConfig conf) {
	currentDiff = conf.difficulty;
	gridManager.InitGrid(playerStats, conf.mode, conf.difficulty);
	playerStats = PlayerStats();

	if (conf.type == GameType::Challenge) {
		std::cout << "Challenge mode initialized\n";
		GameTime gameTime = GetGameTime(conf.difficulty);

		playerStats.SetTimer(gameTime.starting);
		playerStats.SetTimeLimit(gameTime.cap);
		playerStats.SetTimeAdd(gameTime.onClick);
		playerStats.SetTimerState(true);
	} else {
		std::cout << "normal mode initialized\n";
		playerStats.SetTimer(0.0F);
		playerStats.SetTimeLimit(0.0F);
		playerStats.SetTimeAdd(0.0F);
		playerStats.SetTimerState(false);
	}
}

void Game::UpdatePlaying() {
	if (ui.IsMessageBoxOpen()) {
		return;
	}

	evolutionSystem.Update(playerStats);
	if (!playerStats.isSelectionPopup) {
		gridManager.Update(playerStats, ui, inputManager);
	}

	popup.Update(gridManager.GetGrid(), playerStats);

	if (playerStats.shoudlNewLevelStart) {
		playerStats.shoudlNewLevelStart = false;
		gridManager.InitGrid(playerStats, gameMode, Difficulty::Medium);
	}

	if (playerStats.hp < 0 || playerStats.timer < 0) {
		ui.TriggerMessageBox("You have lost! \n Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (playerStats.gameWon) {
		ui.TriggerMessageBox("You have won! \n Press R to return to menu.");
		gameState = GameState::Lose;
	}

	if (IsKeyPressed(KEY_J)) {
		playerStats.isChoosePending = true;
	}

	passiveItemManager.Update(gridManager.GetGrid(), playerStats);
	playerStats.Update();
}

void Game::RenderPlaying() {
	gridManager.Render(&textureManager, playerStats, gameMode);
	ui.RenderUi(playerStats, gameState);
	ui.RenderTimeBar(playerStats);

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
