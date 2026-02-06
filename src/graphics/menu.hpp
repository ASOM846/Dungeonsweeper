#pragma once

#include "../grid/grid.hpp"
#include "../textureManager.hpp"
#include "button.hpp"
#include <raylib.h>

enum class MenuState {
	MainMenu,
	Settings,
	Modes,
	GameShoudlStart,
};

class Menu {
  public:
	Menu() : IsStartGame(false) {}
	~Menu() = default;

	void Init();
	void Reset();
	void Update();
	void Render(TextureManager &textureManager);

	bool IsStartGamePressed() const;

  private:
	void UpdateButtonsPosition();
	void RenderBackground(const TextureManager &TextureManager) const;

  private:
	NewButton startGameButton;
	NewButton settingsButton;

	bool IsStartGame;

	MenuState currentState;
	const int bigButtonHeight = 70;
	const int bigButtonWidth = 300;
	const int mainButtionsSpacing = 20;
};
