#include "menu.hpp"
#include "../grid/grid.hpp"
#include <algorithm>
#include <raylib.h>

namespace {
struct MenuLayout {
	float x;
	float topY;
	float buttonW;
	float buttonH;
	float spacing;
};

MenuLayout ComputeMainMenuLayout(int screenW, int screenH) {
	float w = static_cast<float>(screenW);
	float h = static_cast<float>(screenH);

	float buttonW = std::clamp(w * 0.32f, 240.0f, 360.0f);
	float buttonH = std::clamp(h * 0.085f, 56.0f, 82.0f);
	float spacing = std::clamp(buttonH * 0.35f, 14.0f, 26.0f);

	float totalH = 3.0f * buttonH + 2.0f * spacing;
	float desiredTop = h * 0.38f;
	float minTop = 160.0f;
	float maxTop = h - totalH - 30.0f;
	float topY = desiredTop;
	if (topY < minTop) {
		topY = minTop;
	}
	if (maxTop < minTop) {
		topY = minTop;
	} else if (topY > maxTop) {
		topY = maxTop;
	}

	float x = (w - buttonW) * 0.5f;
	return MenuLayout{x, topY, buttonW, buttonH, spacing};
}
} // namespace

void Menu::Init() {
	currentState = MenuState::MainMenu;

	auto layout = ComputeMainMenuLayout(GetScreenWidth(), GetScreenHeight());

	easyGameButton = NewButton(layout.x, layout.topY, layout.buttonW,
							   layout.buttonH, "Easy Mode");

	mediumGameButton = NewButton(layout.x, layout.topY, layout.buttonW,
								 layout.buttonH, "Medium Mode");

	hardGameButton = NewButton(layout.x, layout.topY, layout.buttonW,
							   layout.buttonH, "Hard Mode");

	settingsButton = NewButton(layout.x, layout.topY, layout.buttonW,
							   layout.buttonH, "Settings");
}

void Menu::Update() {
	UpdateButtonsPosition();

	easyGameButton.Update();
	mediumGameButton.Update();
	hardGameButton.Update();

	settingsButton.Update();

	if (easyGameButton.IsClicked()) {
		currentState = MenuState::ClassicGameShoudlStart;
		selectedDifficulty = Difficulty::Easy;
		IsStartGame = true;
	} else if (mediumGameButton.IsClicked()) {
		currentState = MenuState::ClassicGameShoudlStart;
		selectedDifficulty = Difficulty::Medium;
		IsStartGame = true;
	} else if (hardGameButton.IsClicked()) {
		currentState = MenuState::ClassicGameShoudlStart;
		selectedDifficulty = Difficulty::Hard;
		IsStartGame = true;
	}

	if (settingsButton.IsClicked()) {
		currentState = MenuState::Settings;
	}
}

void Menu::Render(TextureManager &textureManager) {
	easyGameButton.Draw();
	mediumGameButton.Draw();
	hardGameButton.Draw();

	settingsButton.Draw();
}

void Menu::Reset() {
	IsStartGame = false;
	currentState = MenuState::MainMenu;
}

void Menu::UpdateButtonsPosition() {
	auto layout = ComputeMainMenuLayout(GetScreenWidth(), GetScreenHeight());

	easyGameButton.SetSize(layout.buttonW, layout.buttonH);
	mediumGameButton.SetSize(layout.buttonW, layout.buttonH);
	hardGameButton.SetSize(layout.buttonW, layout.buttonH);

	settingsButton.SetSize(layout.buttonW, layout.buttonH);

	easyGameButton.SetPosition(layout.x, layout.topY);

	mediumGameButton.SetPosition(
		layout.x, layout.topY + 1.0f * (layout.buttonH + layout.spacing));

	hardGameButton.SetPosition(
		layout.x, layout.topY + 2.0f * (layout.buttonH + layout.spacing));

	settingsButton.SetPosition(
		layout.x, layout.topY + 3.0f * (layout.buttonH + layout.spacing));
}

bool Menu::IsStartGamePressed() const {
	return IsStartGame;
}
