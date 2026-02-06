#pragma once
#include "entity/playerStats.hpp"
#include "grid/gridManager.hpp"
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

	PlayerStats playerStats;
	EvolutionSystem evolutionSystem;
	GridManager gridManager;
	TextureManager textureManager;
	UI ui;
};
