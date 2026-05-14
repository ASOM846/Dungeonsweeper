#include "menuBackground.hpp"
#include "../grid/grid.hpp"
#include <raylib.h>

void MenuBackground::UpdatePositions() {
	const int fontSize = 60;

	const int fireSpacing = 50;
	fire1.SetPosition(
		{GetScreenWidth() / 2 - DungeonLength / 2 - fireSpacing, 180});

	fire2.SetPosition(
		{GetScreenWidth() / 2 + DungeonLength / 2 + fireSpacing, 180});
}

void MenuBackground::Update() {
	UpdatePositions();

	fire1.Update(GetScreenWidth(), GetScreenHeight());
	fire2.Update(GetScreenWidth(), GetScreenHeight());
}

void MenuBackground::Render(const TextureManager &textureManager) {
	RenderBcg(textureManager);
	fire1.Draw();
	fire2.Draw();
}

void MenuBackground::Init() {
	fire1.SetType(ParticleSystem::FIRE);
	fire2.SetType(ParticleSystem::FIRE);

	const int numberOfTilesX = GetScreenWidth() / Grid::CELL_SIZE + 1;
	const int numberOfTilesY = GetScreenHeight() / Grid::CELL_SIZE + 1;

	int allTiles = numberOfTilesX * numberOfTilesY;
	bcgTextures.resize(allTiles);

	for (auto &t : bcgTextures) {
		if (GetRandomValue(0, 4) == 0)
			t.textureNumber = GetRandomValue(0, 7);
		else
			t.textureNumber = 0;

		t.rotation = GetRandomValue(0, 3);
	}
}

void MenuBackground::RenderBcg(const TextureManager &textureManager) {
	const int numberOfTilesX = (GetScreenWidth() / Grid::CELL_SIZE) + 1;

	for (size_t i = 0; i < bcgTextures.size(); ++i) {
		int gridX = i % numberOfTilesX;
		int gridY = i / numberOfTilesX;

		auto currentTile = static_cast<TextureId>(
			static_cast<int>(TextureId::Floor1) + bcgTextures[i].textureNumber);

		Texture2D tex = textureManager.get(currentTile);

		float rotation = bcgTextures[i].rotation * 90.0f;

		Rectangle source = {0.0f, 0.0f, static_cast<float>(tex.width),
							static_cast<float>(tex.height)};

		Rectangle dest = {.x = static_cast<float>(gridX * Grid::CELL_SIZE) +
							   (Grid::CELL_SIZE / 2.0f),
						  .y = static_cast<float>(gridY * Grid::CELL_SIZE) +
							   (Grid::CELL_SIZE / 2.0f),
						  .width = static_cast<float>(Grid::CELL_SIZE),
						  .height = static_cast<float>(Grid::CELL_SIZE)};

		Vector2 origin = {Grid::CELL_SIZE / 2.0f, Grid::CELL_SIZE / 2.0f};

		DrawTexturePro(tex, source, dest, origin, rotation, WHITE);
	}

	const int fontSize = 60;
	const int offset = 20;
	const float spacing = 3.0F;

	Font font = textureManager.getDefaultFont();

	Vector2 dungeonSize = MeasureTextEx(font, "Dungeon", fontSize, spacing);
	DungeonLength = dungeonSize.x;

	Vector2 sweeperSize = MeasureTextEx(font, "Sweeper", fontSize, spacing);

	DrawTextEx(
		font, "Dungeon",
		{GetScreenWidth() / 2.0f - dungeonSize.x / 2.0f - offset, 100.0f},
		fontSize, spacing, Color{200, 170, 140, 255});

	DrawTextEx(
		font, "Sweeper",
		{GetScreenWidth() / 2.0f - sweeperSize.x / 2.0f + offset, 160.0f},
		fontSize, spacing, Color{200, 170, 140, 255});
}
