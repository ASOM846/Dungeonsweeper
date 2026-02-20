#include "gridRender.hpp"
#include "../graphics/shopPopup.hpp"
#include "gridUtils.hpp"
#include <raylib.h>

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
							GameMode gameMode) {
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

			if (grid.cells[y][x].flagged) {
				DrawTextureExCentered(
					textureManager->get(TextureId::Skull),
					{x * size + offset.x, y * size + offset.y}, 0.0f, scale,
					WHITE);
			}

			switch (grid.cells[y][x].state) {
			case Grid::CellState::Hidden:
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
		}
	}

	Vector2 mousePos = GetMousePosition();
	int x = static_cast<int>((mousePos.x - offset.x) / size);
	int y = static_cast<int>((mousePos.y - offset.y) / size);

	if (x < 0 || x >= grid.GetWidth() || y < 0 || y >= grid.GetHeight() ||
		grid.cells[y][x].state == Grid::CellState::Hidden)
		return;

	if (grid.cells[y][x].specialFunction != Grid::SpecialFunction::None ||
		grid.cells[y][x].val > 0) {
		ShopPopupInfo info;
		info.description = "info";
		info.icon = textureManager->get(TextureId::Coin);
		info.name = "nigger";
		if (grid.cells[y][x].val > 0)
			;
		info.price = grid.cells[y][x].val;
		if (grid.cells[y][x].itemPrice > 0)
			info.price = grid.cells[y][x].val;
		info.valid = true;

		DrawShopPopup(info, GetMousePosition(),
					  textureManager->get(TextureId::Coin));
	}
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

void GridRender::ReveledCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	Grid::Cell &current = grid.cells[y][x];

	ShopPopupInfo popupInfo;

	if (current.specialFunction == Grid::SpecialFunction::ItemCell) {
		ShopCellRender(x, y, grid, gUtils::GetOffset(grid));
		return;
	}

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

	if (current.specialFunction == Grid::SpecialFunction::SwordRed) {
		RenderTexture(x, y, TextureId::SwordRed, offset, 45.0f);
		return;
	}

	if (current.specialFunction == Grid::SpecialFunction::SwordGreen) {
		RenderTexture(x, y, TextureId::SwordGreen, offset, 45.0f);
		return;
	}

	if (current.specialFunction == Grid::SpecialFunction::SwordGold) {
		RenderTexture(x, y, TextureId::SwordGold, offset, 45.0f);
		return;
	}

	if (current.specialFunction == Grid::SpecialFunction::MiniDungeonEntry) {
		RenderTexture(x, y, TextureId::Hole, offset);
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

void GridRender::ShopCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	Grid::Cell &current = grid.cells[y][x];

	if (current.itemType == ItemType::HpUp) {
		// Card background (rounded, dark, gold border)
		Rectangle cardRect{x * size + offset.x + size * 0.08f,
						   y * size + offset.y + size * 0.08f, size * 0.84f,
						   size * 0.84f};
		DrawRectangleRounded(cardRect, 0.22f, 6, Color{34, 30, 26, 255});
		DrawRectangleLinesEx(cardRect, 2.0f, Color{210, 175, 70, 255});

		// Draw item texture centered
		const Texture2D &tex = textureManager->get(TextureId::HeartFull);
		float scale = (size * 0.48f) / tex.width;
		float cx = cardRect.x + (cardRect.width - tex.width * scale) * 0.5f;
		float cy = cardRect.y +
				   (cardRect.height - tex.height * scale - size * 0.18f) * 0.5f;
		DrawTextureEx(tex, Vector2{cx, cy}, 0.0f, scale, WHITE);

		// Price bar at bottom
		// Price bar at bottom
		float priceBarH = size * 0.22f;
		Rectangle priceBar{cardRect.x, cardRect.y + cardRect.height - priceBarH,
						   cardRect.width, priceBarH};
		DrawRectangleRounded(priceBar, 0.18f, 4, Color{210, 175, 70, 230});
		DrawRectangleLinesEx(priceBar, 1.5f, Color{120, 96, 72, 255});

		// Price text and coin icon
		std::string priceText = std::to_string(current.itemPrice);
		int fontSize = static_cast<int>(priceBarH * 0.95f); // much larger
		int textW = MeasureText(priceText.c_str(), fontSize);

		// Coin icon
		const Texture2D &coinTex = textureManager->get(TextureId::Coin);
		float coinScale = priceBarH * 0.7f / coinTex.height;
		float coinW = coinTex.width * coinScale;
		float coinH = coinTex.height * coinScale;

		// Center both as a group
		float totalW = coinW + 6 + textW;
		float startX = priceBar.x + (priceBar.width - totalW) * 0.5f;
		float coinY = priceBar.y + (priceBar.height - coinH) * 0.5f;
		DrawTextureEx(coinTex, Vector2{startX, coinY}, 0.0f, coinScale, WHITE);

		// Draw price text next to coin
		DrawText(
			priceText.c_str(), static_cast<int>(startX + coinW + 6),
			static_cast<int>(priceBar.y + (priceBar.height - fontSize) * 0.5f),
			fontSize, Color{34, 30, 26, 255}); // Price text centered

		return;
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
