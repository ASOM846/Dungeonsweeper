#include "menu.hpp"

void Menu::Init() {
	startGameButton = NewButton(GetScreenWidth() / 2 - bigButtonWidth / 2,
								GetScreenHeight() / 2 - bigButtonHeight / 2,
								bigButtonWidth, bigButtonHeight, "Start Game");

	settingsButton = NewButton(GetScreenWidth() / 2 - bigButtonWidth / 2,
							   GetScreenHeight() / 2 + bigButtonHeight / 2 +
								   mainButtionsSpacing,
							   bigButtonWidth, bigButtonHeight, "Settings");
}

void Menu::Update() {
	UpdateButtonsPosition();
	startGameButton.Update();
	settingsButton.Update();

	if (startGameButton.IsClicked()) {
		IsStartGame = true;
	}
}

void Menu::Render(TextureManager &textureManager) {
	RenderBackground(textureManager);
	startGameButton.Draw();
	settingsButton.Draw();
}

void Menu::Reset() {
	IsStartGame = false;
}

void Menu::UpdateButtonsPosition() {
	startGameButton.SetPosition(GetScreenWidth() / 2 - bigButtonWidth / 2,
								GetScreenHeight() / 2 - bigButtonHeight / 2);
	settingsButton.SetPosition(GetScreenWidth() / 2 - bigButtonWidth / 2,
							   GetScreenHeight() / 2 + bigButtonHeight / 2 +
								   mainButtionsSpacing);
}

void Menu::RenderBackground(const TextureManager &textureManager) const {
	const int numberOfTilesX = GetScreenWidth() / Grid::CELL_SIZE + 1;
	const int numberOfTilesY = GetScreenHeight() / Grid::CELL_SIZE + 1;

	float scale =
		Grid::CELL_SIZE /
		static_cast<float>(textureManager.get(TextureId::Floor1).width);

	for (int i = 0; i < numberOfTilesX; ++i) {
		for (int j = 0; j < numberOfTilesY; ++j) {
			Vector2 position = {static_cast<float>(i * Grid::CELL_SIZE),
								static_cast<float>(j * Grid::CELL_SIZE)};
			DrawTextureEx(textureManager.get(TextureId::Floor1), position, 0.0f,
						  scale, WHITE);
		}
	}

	const int fontSize = 60;
	float textLength = MeasureText("DungeonSweeper", fontSize);
	DrawText("DungeonSweeper", GetScreenWidth() / 2 - textLength / 2, 100,
			 fontSize, Color{200, 170, 140, 255});
}

bool Menu::IsStartGamePressed() const {
	return IsStartGame;
}
