#include "menuBackground.hpp"
#include "../grid/grid.hpp"

void MenuBackground::UpdatePositions() {
	const int fontSize = 60;
	auto textDungeonLength =
		static_cast<float>(MeasureText("Dungeon", fontSize));
	auto textSweeperLength =
		static_cast<float>(MeasureText("Sweeper", fontSize));

	const int fireSpacing = 50;
	fire1.SetPosition(
		{GetScreenWidth() / 2 - textDungeonLength / 2 - fireSpacing, 180});

	fire2.SetPosition(
		{GetScreenWidth() / 2 + textDungeonLength / 2 + fireSpacing, 180});
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
}

void MenuBackground::RenderBcg(const TextureManager &textureManager) const {
	const int numberOfTilesX = GetScreenWidth() / Grid::CELL_SIZE + 1;
	const int numberOfTilesY = GetScreenHeight() / Grid::CELL_SIZE + 1;

	float scale =
		Grid::CELL_SIZE /
		static_cast<float>(textureManager.get(TextureId::Floor1).width);

	for (int i = 0; i < numberOfTilesX; ++i) {
		for (int j = 0; j < numberOfTilesY; ++j) {
			Vector2 position = {static_cast<float>(i * Grid::CELL_SIZE),
								static_cast<float>(j * Grid::CELL_SIZE)};
			DrawTextureEx(textureManager.get(TextureId::Floor1), position, 0.0F,
						  scale, WHITE);
		}
	}

	const int fontSize = 60;
	const int offset = 20;

	auto textDungeonLength =
		static_cast<float>(MeasureText("Dungeon", fontSize));
	auto textSweeperLength =
		static_cast<float>(MeasureText("Sweeper", fontSize));

	DrawText("Dungeon", GetScreenWidth() / 2 - textDungeonLength / 2 - offset,
			 100, fontSize, Color{200, 170, 140, 255});
	DrawText("Sweeper", GetScreenWidth() / 2 - textSweeperLength / 2 + offset,
			 150, fontSize, Color{200, 170, 140, 255});
}
