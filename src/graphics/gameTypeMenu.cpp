#include "gameTypeMenu.hpp"
#include "button.hpp"
#include <iostream>
#include <raylib.h>

void GameTypeMenu::Render() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int fontSize = 20;
	int lineSpacing = fontSize + 6;

	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	int centerY = screenHeight / 2;
	int startY = centerY - 160;

	const char *standardLines[] = {"Standard", "No time limit",
								   "Classic gameplay"};

	for (int i = 0; i < 3; i++) {
		int textWidth = MeasureText(standardLines[i], fontSize);
		DrawText(standardLines[i], leftCenter - textWidth / 2,
				 startY + i * lineSpacing, fontSize, LIGHTGRAY);
	}

	const GameTime gameTime = GetGameTime(selectedDiff);

	std::string startLine =
		"Start with " +
		std::to_string((static_cast<int>(gameTime.starting) / 60)) + " minutes";
	std::string secondLine =
		"+" + std::to_string(static_cast<int>(gameTime.onClick)) +
		" seconds per move";
	std::string thirdLine =
		"Time cap: " + std::to_string(static_cast<int>(gameTime.cap / 60)) +
		" minutes";

	const char *challengeLines[] = {"Challenge", startLine.c_str(),
									secondLine.c_str(), thirdLine.c_str()};

	for (int i = 0; i < 4; i++) {
		int textWidth = MeasureText(challengeLines[i], fontSize);
		DrawText(challengeLines[i], rightCenter - (textWidth / 2),
				 startY + (i * lineSpacing), fontSize, LIGHTGRAY);
	}

	buttonStandard.Draw();
	buttonChallenge.Draw();
	buttonGoBack.Draw();
}

void GameTypeMenu::Update() {
	InitButtons();
	buttonChallenge.Update();
	buttonStandard.Update();
	buttonGoBack.Update();

	if (buttonStandard.IsClicked()) {
		isPicked = true;
		selectedType = GameType::Normal;
	}

	if (buttonChallenge.IsClicked()) {
		isPicked = true;
		selectedType = GameType::Challenge;
	}

	if (buttonGoBack.IsClicked()) {
		shoudlReturnToMenu = true;
	}
}

void GameTypeMenu::Init(const Difficulty &diff) {
	selectedDiff = diff;
}

void GameTypeMenu::Reset() {
	isPicked = false;
	shoudlReturnToMenu = false;
	selectedDiff = Difficulty();
}

void GameTypeMenu::InitButtons() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int buttonW = 200;
	int buttonH = 60;

	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	int leftButtonX = leftCenter - (buttonW / 2);
	int rightButtonX = rightCenter - (buttonW / 2);

	int centerY = screenHeight / 2;
	int buttonY = centerY - (buttonH / 2);

	int centerX = screenWidth / 2;

	buttonStandard =
		NewButton(leftButtonX, buttonY, buttonW, buttonH, "Standard Mode");
	buttonChallenge =
		NewButton(rightButtonX, buttonY, buttonW, buttonH, "Challenge Mode");
	buttonGoBack =
		NewButton(centerX - buttonW / 2, centerY + 100, buttonW, buttonH, "<");
}
