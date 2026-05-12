#include "menu.hpp"
#include "../grid/grid.hpp"
#include "card.hpp"
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

	float totalH = 4.0f * buttonH + 3.0f * spacing;

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

	easyGameButton = NewButton(0, 0, 0, 0, "Easy Mode");

	mediumGameButton = NewButton(0, 0, 0, 0, "Medium Mode");

	hardGameButton = NewButton(0, 0, 0, 0, "Hard Mode");

	settingsButton = NewButton(0, 0, 0, 0, "Settings");

	dailyCard = SideCard(0, 0, 0, 0, "DAILY", "CHALLENGE", "One Seed.",
						 "One Attempt.", "Coming soon.", "PLAY NOW", false);

	dailyCard.SetColors(Color{184, 134, 11, 200}, Color{255, 215, 0, 255},
						Color{50, 40, 30, 240});

	placeholderCard =
		SideCard(0, 0, 0, 0, "ENDLESS", "DUNGEON", "New modifiers.",
				 "Special rewards.", "Coming soon.", "LOCKED", false);

	placeholderCard.SetColors(Color{100, 110, 120, 200},
							  Color{180, 200, 220, 255},
							  Color{40, 50, 60, 240});
}

void Menu::UpdateButtonsPosition() {
	float w = static_cast<float>(GetScreenWidth());

	float h = static_cast<float>(GetScreenHeight());

	auto layout = ComputeMainMenuLayout(w, h);

	float cardW = std::clamp(w * 0.25f, 260.0f, 350.0f);

	float cardH = std::clamp(h * 0.40f, 300.0f, 400.0f);

	float cardY =
		layout.topY -
		(cardH - (layout.buttonH * 3.0f + layout.spacing * 3.0f)) / 2.0f;

	float marginX = w * 0.05f;

	placeholderCard.SetPosition(marginX, cardY);

	placeholderCard.SetSize(cardW, cardH);

	dailyCard.SetPosition(w - cardW - marginX, cardY);

	dailyCard.SetSize(cardW, cardH);

	easyGameButton.SetSize(layout.buttonW, layout.buttonH);

	easyGameButton.SetPosition(layout.x, layout.topY);

	mediumGameButton.SetSize(layout.buttonW, layout.buttonH);

	mediumGameButton.SetPosition(
		layout.x, layout.topY + 1.0f * (layout.buttonH + layout.spacing));

	hardGameButton.SetSize(layout.buttonW, layout.buttonH);

	hardGameButton.SetPosition(
		layout.x, layout.topY + 2.0f * (layout.buttonH + layout.spacing));

	settingsButton.SetSize(layout.buttonW, layout.buttonH);

	settingsButton.SetPosition(
		layout.x, layout.topY + 3.0f * (layout.buttonH + layout.spacing));
}

void Menu::Update() {
	UpdateButtonsPosition();

	easyGameButton.Update();
	mediumGameButton.Update();
	hardGameButton.Update();
	settingsButton.Update();

	dailyCard.Update();
	placeholderCard.Update();

	selectedMode = GameMode::Classic;

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
	} else if (dailyCard.IsClicked()) {

	} else if (placeholderCard.IsClicked()) {
		selectedMode = GameMode::Endless;
		selectedDifficulty = Difficulty::Medium;
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

	dailyCard.Render();
	placeholderCard.Render();
}

void Menu::Reset() {
	IsStartGame = false;

	currentState = MenuState::MainMenu;
}

bool Menu::IsStartGamePressed() const {
	return IsStartGame;
}
