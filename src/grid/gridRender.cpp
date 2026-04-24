#include "gridRender.hpp"
#include "../entity/playerStats.hpp"
#include "grid.hpp"
#include "gridUtils.hpp"
#include <raylib.h>
#include <string>

namespace {
inline void DrawTextureExCentered(const Texture2D &texture, Vector2 position,
								  float rotation, float scale, Color tint) {
	Rectangle source{0.0f, 0.0f, static_cast<float>(texture.width),
					 static_cast<float>(texture.height)};

	const float destW = texture.width * scale;
	const float destH = texture.height * scale;

	Rectangle dest{position.x + destW * 0.5f, position.y + destH * 0.5f, destW,
				   destH};
	Vector2 origin{destW * 0.5f, destH * 0.5f};

	DrawTexturePro(texture, source, dest, origin, rotation, tint);
}
} // namespace

void GridRender::RenderGrid(TextureManager const *textureManager, Grid &grid,
							GameMode gameMode, PlayerStats &playerStats) {
	this->textureManager = textureManager;
	if (!this->textureManager) {
		return;
	}
	Vector2 offset = gUtils::GetOffset(grid);
	int size = Grid::CELL_SIZE;

	float scale =
		size / static_cast<float>(textureManager->get(TextureId::Floor1).width);

	for (size_t x = 0; x < grid.GetWidth(); ++x) {
		for (size_t y = 0; y < grid.GetHeight(); ++y) {
			TextureId floorTextureId =
				GetFloorTextureId(grid.cells[y][x].textureNumber);

			DrawTextureExCentered(textureManager->get(floorTextureId),
								  {x * size + offset.x, y * size + offset.y},
								  grid.cells[y][x].rotation * 90.0f, scale,
								  WHITE);

			if (grid.cells[y][x].flagVal != std::to_string(0)) {
				DrawText(grid.cells[y][x].flagVal.c_str(),
						 x * size + offset.x + size / 2 - size / 4,
						 y * size + offset.y + size / 2 - size / 4,
						 size / 2 + size / 4, Color{220, 200, 170, 255});
			}

			switch (grid.cells[y][x].state) {
			case Grid::CellState::Hidden:
				break;
			case Grid::CellState::Revealing:
				RevealingCellRender(static_cast<int>(x), static_cast<int>(y),
									grid, offset);
				break;
			case Grid::CellState::Revealed:
				ReveledCellRender(static_cast<int>(x), static_cast<int>(y),
								  grid, offset);
				break;
			case Grid::CellState::pointsNotTaken:
				PointsNotTakenCellRender(static_cast<int>(x),
										 static_cast<int>(y), grid, offset);
				break;
			case Grid::CellState::Hinting: {
				HintingCellRender(static_cast<int>(x), static_cast<int>(y),
								  grid, offset, scale);
				break;
			}
			default:
				break;
			}
			DrawRectangleLines(x * size + offset.x, y * size + offset.y, size,
							   size, BLACK);

			// int hint = gUtils::GetNeighboursSum(static_cast<int>(x),
			// 									static_cast<int>(y), grid);
			// if (hint != 0) {
			// 	std::string text2 = std::to_string(hint);
			// 	DrawText(text2.c_str(),
			// 			 static_cast<int>(x * size + offset.x + 28),
			// 			 static_cast<int>(y * size + offset.y + 8), 20, GREEN);
			// }
		}
	}

	Vector2 mousePos = GetMousePosition();
	int x = static_cast<int>((mousePos.x - offset.x) / size);
	int y = static_cast<int>((mousePos.y - offset.y) / size);

	if (x < 0 || x >= grid.GetWidth() || y < 0 || y >= grid.GetHeight() ||
		grid.cells[y][x].state == Grid::CellState::Hidden)
		return;
}

TextureId GridRender::GetFloorTextureId(int type) {
	switch (type) {
	case 1:
		return TextureId::Floor1;
	case 2:
		return TextureId::Floor2;
	case 3:
		return TextureId::Floor3;
	case 4:
		return TextureId::Floor4;
	case 5:
		return TextureId::Floor5;
	case 6:
		return TextureId::Floor6;
	case 7:
		return TextureId::Floor7;
	case 8:
		return TextureId::Floor8;
	default:
		return TextureId::Floor1;
		TextureId textureId;
	}
}

void GridRender::RevealingCellRender(int x, int y, Grid &grid, Vector2 offset) {
	Grid::Cell &current = grid.cells[y][x];

	const int framesSpeed =
		5; // <--- Szybkość animacji (im więcej, tym wolniej)

	// 1. Zwiększamy licznik klatek silnika
	current.framesCounter++;

	// 2. Jeśli licznik osiągnie limit, zmieniamy klatkę animacji
	if (current.framesCounter >= framesSpeed) {
		current.framesCounter = 0;
		current.animationFrame++;

		// 3. Sprawdzamy czy to koniec wybuchu
		if (current.animationFrame > 7) {
			current.state =
				Grid::CellState::Revealed; // Zmieniamy stan na docelowy
			current.animationFrame = 0;	   // Resetujemy na przyszłość
			current.framesCounter = 0;

			// Rysujemy od razu docelowy kafel, żeby nie mrugnęło
			ReveledCellRender(x, y, grid, offset);
			return;
		}
	}

	// 4. Rysowanie wybuchu
	// Założenie: W klasie TextureId dodałeś enumy dla wybuchów: Explosion0,
	// Explosion1, ... Explosion7 Jeśli są ułożone po kolei w enumie, możemy je
	// zrzutować rzutując na int:
	TextureId explosionTexId;

	switch (current.animationFrame) {
	case 0:
		explosionTexId = TextureId::Explosion0;
		break;
	case 1:
		explosionTexId = TextureId::Explosion1;
		break;
	case 2:
		explosionTexId = TextureId::Explosion2;
		break;
	case 3:
		explosionTexId = TextureId::Explosion3;
		break;
	case 4:
		explosionTexId = TextureId::Explosion4;
		break;
	case 5:
		explosionTexId = TextureId::Explosion5;
		break;
	case 6:
		explosionTexId = TextureId::Explosion6;
		break;
	case 7:
		explosionTexId = TextureId::Explosion7;
		break;
	default:
		explosionTexId = TextureId::Explosion0;
		break;
	}

	RenderTexture(x, y, explosionTexId, offset);
}

void GridRender::ReveledCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	Grid::Cell &current = grid.cells[y][x];

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::Heal) {
		RenderTexture(x, y, TextureId::HeartFull, offset);
		return;
	}

	if (grid.cells[y][x].specialFunction ==
		Grid::SpecialFunction::Necromancer) {
		DrawEnemy(textureManager, {x * size + offset.x, y * size + offset.y},
				  grid.cells[y][x].val, grid.cells[y][x].specialFunction);
	}

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::Mana) {
		RenderTexture(x, y, TextureId::Coin, offset);
		return;
	}

	if (grid.cells[y][x].specialFunction ==
		Grid::SpecialFunction::UncoverEnemiesVal1) {
		RenderTexture(x, y, TextureId::FlaskGreen, offset);
		return;
	}

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::Ladder) {
		RenderTexture(x, y, TextureId::Ladder, offset);
		DrawRectangleLinesEx({x * size + offset.x, y * size + offset.y,
							  static_cast<float>(size),
							  static_cast<float>(size)},
							 10.0f, BLUE);
		return;
	}

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::GoUpGrid) {
		RenderTexture(x, y, TextureId::Ladder, offset);
		DrawRectangleLinesEx({x * size + offset.x, y * size + offset.y,
							  static_cast<float>(size),
							  static_cast<float>(size)},
							 10.0f, GREEN);
	}

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::Wizzard) {
		RenderTexture(x, y, TextureId::BlueWizzard, offset);
	}

	if (grid.cells[y][x].specialFunction == Grid::SpecialFunction::Starting) {
		StartingCellRender(x, y, grid, offset);
	}

	if (current.specialFunction == Grid::SpecialFunction::Chest) {
		RenderTexture(x, y, TextureId::Chest, offset);
		return;
	}

	if (current.specialFunction == Grid::SpecialFunction::ChestKey) {
		RenderTexture(x, y, TextureId::Key, offset);
		return;
	}

	if (grid.cells[y][x].val > 0) {
		std::string text = std::to_string(grid.cells[y][x].val);
		DrawEnemy(textureManager, {x * size + offset.x, y * size + offset.y},
				  grid.cells[y][x].val, grid.cells[y][x].specialFunction);
		DrawText(text.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 8), 20, RED);
	}
}

void GridRender::PointsNotTakenCellRender(int x, int y, Grid &grid,
										  Vector2 offset) {
	int size = Grid::CELL_SIZE;
	DrawEnemy(textureManager, {x * size + offset.x, y * size + offset.y},
			  grid.cells[y][x].val, grid.cells[y][x].specialFunction);
	DrawRectangleLinesEx({x * size + offset.x, y * size + offset.y,
						  static_cast<float>(size), static_cast<float>(size)},
						 2.0f, ORANGE);
	if (grid.cells[y][x].val > 0) {
		std::string text = std::to_string(grid.cells[y][x].val);
		DrawText(text.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 2), 20, BLACK);
	}
}

void GridRender::HintingCellRender(int x, int y, Grid &grid, Vector2 offset,
								   float scale) {
	int size = Grid::CELL_SIZE;
	TextureId floorTextureId =
		GetFloorTextureId(grid.cells[y][x].textureNumber);

	DrawTextureExCentered(textureManager->get(floorTextureId),
						  {x * size + offset.x, y * size + offset.y},
						  grid.cells[y][x].rotation * 90.0f, scale,
						  {100, 100, 100, 255});
	int hint = gUtils::GetNeighboursSum(static_cast<int>(x),
										static_cast<int>(y), grid);
	if (hint != 0) {
		std::string text2 = std::to_string(hint);
		DrawText(text2.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 8), 20, GREEN);
	}
}

void GridRender::StartingCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	DrawTextureEx(
		textureManager->get(TextureId::BeginButton),
		{x * size + offset.x, y * size + offset.y}, 0.0f,
		size / static_cast<float>(textureManager->get(TextureId::Floor1).width),
		WHITE);
}

void GridRender::RenderTexture(int x, int y, const TextureId &texId,
							   const Vector2 offset, float rotation) {
	const int size = Grid::CELL_SIZE;

	const Texture2D &tex = textureManager->get(texId);
	const float scale = (size * 0.6f) / static_cast<float>(tex.width);
	const float scaledW = tex.width * scale;
	const float scaledH = tex.height * scale;
	Vector2 pos = {x * size + offset.x + (size - scaledW) * 0.5f,
				   y * size + offset.y + (size - scaledH) * 0.5f};

	DrawTextureExCentered(tex, pos, rotation, scale, WHITE);
}

void GridRender::DrawEnemy(TextureManager const *textureManager,
						   const Vector2 position, const int type,
						   Grid::SpecialFunction specialFunction) {
	if (!textureManager) {
		return;
	}

	TextureId textureId;

	if (specialFunction == Grid::SpecialFunction::OgreBig) {
		textureId = TextureId::OgreBig;
	} else if (specialFunction == Grid::SpecialFunction::OgreSmall) {
		textureId = TextureId::OgreSmall;
	} else if (specialFunction == Grid::SpecialFunction::Necromancer) {
		textureId = TextureId::Necromancer;
	} else {
		switch (type) {
		case 1:
			textureId = TextureId::Enemy1;
			break;
		case 2:
			textureId = TextureId::Enemy2;
			break;
		case 3:
			textureId = TextureId::Enemy3;
			break;
		case 4:
			textureId = TextureId::Enemy4;
			break;
		case 5:
			textureId = TextureId::Enemy5;
			break;
		case 6:
			textureId = TextureId::Enemy6;
			break;
		case 7:
			textureId = TextureId::Enemy7;
			break;
		case 8:
			textureId = TextureId::Enemy8;
			break;
		case 9:
			textureId = TextureId::Enemy9;
			break;
		case 10:
			textureId = TextureId::Enemy10;
			break;
		case 11:
			textureId = TextureId::Enemy11;
			break;
		case 100:
			textureId = TextureId::Bomb;
			break;
		default:
			return; // Unknown type
		}
	}

	const Texture2D &tex = textureManager->get(textureId);
	const float scale = Grid::CELL_SIZE / static_cast<float>(tex.height);
	const float scaledW = tex.width * scale;
	const float scaledH = tex.height * scale;
	const Vector2 drawPos{position.x + (Grid::CELL_SIZE - scaledW) * 0.5f,
						  position.y + (Grid::CELL_SIZE - scaledH) * 0.5f};

	DrawTextureEx(tex, drawPos, 0.0f, scale, WHITE);
}
