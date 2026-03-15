#include "menu.hpp"

#include <algorithm>

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
	float desiredTop = h * 0.42f;
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

	classicGameButton =
		NewButton(layout.x, layout.topY, layout.buttonW, layout.buttonH,
				  "Classic Game");
	endlessGameButton =
		NewButton(layout.x, layout.topY + layout.buttonH + layout.spacing,
				  layout.buttonW, layout.buttonH, "Endless Game");
	settingsButton =
		NewButton(layout.x,
				  layout.topY + 2.0f * (layout.buttonH + layout.spacing),
				  layout.buttonW, layout.buttonH, "Settings");
}

void Menu::Update() {
	UpdateButtonsPosition();

	classicGameButton.Update();
	endlessGameButton.Update();
	settingsButton.Update();

	if (classicGameButton.IsClicked()) {
		currentState = MenuState::ClassicGameShouldStart;
		IsStartGame = true;
	}
	if (endlessGameButton.IsClicked()) {
		currentState = MenuState::EndlessGameShouldStart;
		IsStartGame = true;
	}
	if (settingsButton.IsClicked()) {
		currentState = MenuState::Settings;
	}
}

void Menu::Render(TextureManager &textureManager) {
	RenderBackground(textureManager);
	classicGameButton.Draw();
	endlessGameButton.Draw();
	settingsButton.Draw();
}

void Menu::Reset() {
	IsStartGame = false;
	currentState = MenuState::MainMenu;
}

void Menu::UpdateButtonsPosition() {
	auto layout = ComputeMainMenuLayout(GetScreenWidth(), GetScreenHeight());

	classicGameButton.SetSize(layout.buttonW, layout.buttonH);
	endlessGameButton.SetSize(layout.buttonW, layout.buttonH);
	settingsButton.SetSize(layout.buttonW, layout.buttonH);

	classicGameButton.SetPosition(layout.x, layout.topY);
	endlessGameButton.SetPosition(layout.x,
						  layout.topY + layout.buttonH + layout.spacing);
	settingsButton.SetPosition(layout.x,
						 layout.topY + 2.0f * (layout.buttonH + layout.spacing));
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
