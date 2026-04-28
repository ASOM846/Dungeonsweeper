#pragma once

#include "../gameMode.hpp"
#include "button.hpp"
#include <raylib.h>

class GameTypeMenu {
  public:
	GameTypeMenu() : isPicked{false} {}
	~GameTypeMenu() = default;

	void Render();
	void Update();

	void InitButtons();

	[[nodiscard]] GameType GetSelectedType() const { return selectedType; }
	[[nodiscard]] bool IsPicked() const { return isPicked; }

  private:
	bool isPicked;

	GameType selectedType;

	NewButton buttonStandard;
	NewButton buttonChallenge;
};
