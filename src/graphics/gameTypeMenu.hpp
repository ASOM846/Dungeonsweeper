#pragma once

#include "../gameMode.hpp"
#include "button.hpp"
#include <raylib.h>

class GameTypeMenu {
  public:
	GameTypeMenu() : isPicked{false}, shoudlReturnToMenu{false} {}
	~GameTypeMenu() = default;

	void Render();
	void Update();

	void Init(const Difficulty &diff);

	void Reset();

	void InitButtons();

	[[nodiscard]] GameType GetSelectedType() const { return selectedType; }
	[[nodiscard]] bool IsPicked() const { return isPicked; }
	[[nodiscard]] bool ShoudlReturnToMenu() const { return shoudlReturnToMenu; }

  private:
	bool isPicked;
	bool shoudlReturnToMenu;

	GameType selectedType;
	Difficulty selectedDiff;

	NewButton buttonStandard;
	NewButton buttonChallenge;

	NewButton buttonGoBack;
};
