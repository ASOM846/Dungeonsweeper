#pragma once

#include "../gameMode.hpp"
#include "../textureManager.hpp"
#include "button.hpp"
#include <raylib.h>

enum class MenuState {
	MainMenu,
	Settings,
	Modes,
	ClassicGameShoudlStart,
};

class Menu {
  public:
	Menu() : IsStartGame(false), currentState(MenuState::MainMenu) {}
	~Menu() = default;

	void Init();
	void Reset();
	void Update();
	void Render(TextureManager &textureManager);

	[[nodiscard]] bool IsStartGamePressed() const;

	[[nodiscard]] MenuState GetCurrentState() const { return currentState; }

	[[nodiscard]] Difficulty GetSelectedDifficulty() const {
		return selectedDifficulty;
	}

  private:
	void UpdateButtonsPosition();
	void RenderBackground(const TextureManager &TextureManager) const;

  private:
	NewButton easyGameButton;
	NewButton mediumGameButton;
	NewButton hardGameButton;
	NewButton settingsButton;

	Difficulty selectedDifficulty;

	bool IsStartGame;

	MenuState currentState;
	const int bigButtonHeight = 70;
	const int bigButtonWidth = 300;
	const int mainButtionsSpacing = 20;
};
