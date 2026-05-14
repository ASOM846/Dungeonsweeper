#pragma once
#include <cstdint>

#include "../gameMode.hpp"
#include "../modules/particles.hpp"
#include "../textureManager.hpp"
#include "button.hpp"
#include "card.hpp"
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

	void Init(TextureManager &tm);
	void Reset();
	void Update();
	void Render(TextureManager &textureManager);

	[[nodiscard]] bool IsStartGamePressed() const;

	[[nodiscard]] MenuState GetCurrentState() const { return currentState; }

	[[nodiscard]] Difficulty GetSelectedDifficulty() const {
		return selectedDifficulty;
	}

	[[nodiscard]] GameMode GetSelectedMode() const { return selectedMode; }

  private:
	void UpdateButtonsPosition();

	NewButton easyGameButton;
	NewButton mediumGameButton;
	NewButton hardGameButton;
	NewButton settingsButton;

	SideCard dailyCard;
	SideCard endlessCard;

	Difficulty selectedDifficulty;
	GameMode selectedMode;

	bool IsStartGame;

	MenuState currentState;

	const int bigButtonHeight = 70;
	const int bigButtonWidth = 300;
	const int mainButtionsSpacing = 20;
};
