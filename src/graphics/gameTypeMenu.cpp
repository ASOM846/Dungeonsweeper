#include "gameTypeMenu.hpp"
#include <raylib.h>

void GameTypeMenu::Render() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	const char *descStandard = "Klasyczna rozgrywka";
	const char *descChallenge = "Prawdziwe wyzwanie";

	int fontSize = 20;

	int textStandardWidth = MeasureText(descStandard, fontSize);
	int textChallengeWidth = MeasureText(descChallenge, fontSize);

	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	int centerY = screenHeight / 2;
	int textY = centerY - 80;

	DrawText(descStandard, leftCenter - (textStandardWidth / 2), textY,
			 fontSize, LIGHTGRAY);
	DrawText(descChallenge, rightCenter - (textChallengeWidth / 2), textY,
			 fontSize, LIGHTGRAY);

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
		NewButton(rightButtonX, buttonY, buttonW, buttonH, "Hard Mode");
}
