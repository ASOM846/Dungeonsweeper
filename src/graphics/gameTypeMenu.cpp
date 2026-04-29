#include "gameTypeMenu.hpp"
#include <raylib.h>

void GameTypeMenu::Render() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int fontSize = 20;
	int lineSpacing = fontSize + 6;

	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	int centerY = screenHeight / 2;
	int startY = centerY - 120;

	const char *standardLines[] = {"Standard", "No time limit",
								   "Classic gameplay"};

	for (int i = 0; i < 3; i++) {
		int textWidth = MeasureText(standardLines[i], fontSize);
		DrawText(standardLines[i], leftCenter - textWidth / 2,
				 startY + i * lineSpacing, fontSize, LIGHTGRAY);
	}

	const char *challengeLines[] = {"Challenge", "Start with 3:00",
									"+30 seconds per move", "Time cap: 6:00"};

	for (int i = 0; i < 3; i++) {
		int textWidth = MeasureText(challengeLines[i], fontSize);
		DrawText(challengeLines[i], rightCenter - textWidth / 2,
				 startY + i * lineSpacing, fontSize, LIGHTGRAY);
	}

	buttonStandard.Draw();
	buttonChallenge.Draw();
}

void GameTypeMenu::Update() {
	InitButtons();
	buttonChallenge.Update();
	buttonStandard.Update();

	if (buttonStandard.IsClicked()) {
		isPicked = true;
		selectedType = GameType::Normal;
	}

	if (buttonChallenge.IsClicked()) {
		isPicked = true;
		selectedType = GameType::Challenge;
	}
}

void GameTypeMenu::Reset() {
	isPicked = false;
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

	buttonStandard =
		NewButton(leftButtonX, buttonY, buttonW, buttonH, "Standard Mode");
	buttonChallenge =
		NewButton(rightButtonX, buttonY, buttonW, buttonH, "Challenge Mode");
}
