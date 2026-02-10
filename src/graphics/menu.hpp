#pragma once

#include "../grid/grid.hpp"
#include "../textureManager.hpp"
#include "button.hpp"
#include <raylib.h>

enum class MenuState {
	MainMenu,
	Settings,
	Modes,
	ClassicGameShoudlStart,
	EndlessGameShouldStart,
};

class Menu {
  public:
	Menu() : IsStartGame(false), currentState(MenuState::MainMenu) {}
	~Menu() = default;

	void Init();
	void Reset();
	void Update();
	void Render(TextureManager &textureManager);

	bool IsStartGamePressed() const;

	MenuState GetCurrentState() const { return currentState; }

  private:
	void UpdateButtonsPosition();
	void RenderBackground(const TextureManager &TextureManager) const;

  private:
	NewButton classicGameButton;
	NewButton endlessGameButton;
	NewButton settingsButton;

	bool IsStartGame;

	MenuState currentState;
	const int bigButtonHeight = 70;
	const int bigButtonWidth = 300;
	const int mainButtionsSpacing = 20;
};
