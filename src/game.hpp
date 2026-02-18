#pragma once
#include "entity/playerStats.hpp"
#include "gameMode.hpp"
#include "grid/gridManager.hpp"
#include "inputLocker.hpp"
#include "systems/evolutionSystem.hpp"
#include "textureManager.hpp"
#include <iostream>
#include <raylib.h>

class UI;

enum class GameState {
	Playing,
	Lose,
	ShoudlReturnToMenu,
};

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();
	void Reset();

	void Update();
	void Render();

	void InitClassicGame();
	void InitEndlessGame();

	bool ShouldReturnToMenu() const {
		return gameState == GameState::ShoudlReturnToMenu;
	}
	TextureManager &GetTextureManager() { return textureManager; }

  private:
	void UpdatePlaying();
	void RenderPlaying();

	void UpdateWin();
	void RenderWin();

	void UpdateLose();
	void RenderLose();

  private:
	GameState gameState;
	GameMode gameMode = GameMode::Classic;

	PlayerStats playerStats;
	EvolutionSystem evolutionSystem;
	GridManager gridManager;
	TextureManager textureManager;
	UI ui;

	InputLocker inputLocker;
};
