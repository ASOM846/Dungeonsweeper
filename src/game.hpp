#pragma once
#include "entity/passiveItem.hpp"
#include "entity/playerStats.hpp"
#include "gameMode.hpp"
#include "graphics/popup.hpp"
#include "grid/gridManager.hpp"
#include "inputManager.hpp"
#include "systems/ItemChooser.hpp"
#include "systems/evolutionSystem.hpp"
#include "textureManager.hpp"
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

	void RunGame(GameConfig conf);

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

	GameState gameState;
	GameMode gameMode = GameMode::Classic;
	Difficulty currentDiff;

	Popup popup;
	ItemChooser itemChooser;
	InputManager inputManager;
	PlayerStats playerStats;
	PassiveItemManager passiveItemManager;
	EvolutionSystem evolutionSystem;
	GridManager gridManager;
	TextureManager textureManager;
	UI ui;
};
