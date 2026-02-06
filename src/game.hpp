#pragma once
#include "entity/playerStats.hpp"
#include "graphics/ui.hpp"
#include "grid/gridManager.hpp"
#include "systems/evolutionSystem.hpp"
#include "textureManager.hpp"
#include <iostream>
#include <raylib.h>

enum class GameState {
	Playing,
	Win,
	Lose,
};

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();

	void Run();

	void Update();
	void Render();

	TextureManager &GetTextureManager() { return textureManager; }

  private:
	void UpdatePlaying();
	void RenderPlaying();

	void UpdateWin();
	void RenderWin();

	void UpdateLose();
	void RenderLose();
	void Reset();

  private:
	GameState gameState;

	bool shoudlReturnToMenu;

	PlayerStats playerStats;
	EvolutionSystem evolutionSystem;
	GridManager gridManager;
	TextureManager textureManager;
	UI ui;
};
