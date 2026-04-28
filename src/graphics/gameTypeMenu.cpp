#include "gameTypeMenu.hpp"
#include <raylib.h>

void GameTypeMenu::Render() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	// Teksty opisujące tryby
	const char *descStandard = "Klasyczna rozgrywka";
	const char *descChallenge = "Prawdziwe wyzwanie";

	int fontSize = 20;

	// Obliczanie szerokości tekstu, aby go wyśrodkować
	int textStandardWidth = MeasureText(descStandard, fontSize);
	int textChallengeWidth = MeasureText(descChallenge, fontSize);

	// Środki lewej i prawej połówki ekranu
	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	// Wysokość, na której rysujemy tekst (np. nieco powyżej połowy ekranu)
	int centerY = screenHeight / 2;
	int textY = centerY - 80;

	// Rysowanie tekstów nad guzikami
	DrawText(descStandard, leftCenter - (textStandardWidth / 2), textY,
			 fontSize, LIGHTGRAY);
	DrawText(descChallenge, rightCenter - (textChallengeWidth / 2), textY,
			 fontSize, LIGHTGRAY);

	// Rysowanie guzików
	buttonStandard.Draw();
	buttonChallenge.Draw();
}

void GameTypeMenu::Update() {
	InitButtons();
	buttonChallenge.Update();
	buttonStandard.Update();
}

void GameTypeMenu::InitButtons() {
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	// Ustawiamy sztywne wymiary przycisku (możesz je dowolnie zmienić)
	int buttonW = 200;
	int buttonH = 60;

	// Środki lewej i prawej połówki ekranu
	int leftCenter = screenWidth / 4;
	int rightCenter = 3 * screenWidth / 4;

	// Obliczanie pozycji X dla lewego i prawego guzika, tak by były na środkach
	// swoich połówek
	int leftButtonX = leftCenter - (buttonW / 2);
	int rightButtonX = rightCenter - (buttonW / 2);

	// Obliczanie pozycji Y guzików (na środku ekranu w pionie)
	int centerY = screenHeight / 2;
	int buttonY = centerY - (buttonH / 2);

	// Inicjalizacja guzików
	buttonStandard =
		NewButton(leftButtonX, buttonY, buttonW, buttonH, "Standard Mode");
	buttonChallenge =
		NewButton(rightButtonX, buttonY, buttonW, buttonH, "Hard Mode");
}
