#include "gridRender.hpp"

void GridRender::RenderGrid(TextureManager const *textureManager, Grid &grid) {
	this->textureManager = textureManager;
	if (!this->textureManager) {
		return;
	}
	Vector2 offset = gUtils::GetOffset(grid);
	int size = Grid::CELL_SIZE;

	float scale =
		size / static_cast<float>(textureManager->get(TextureId::Floor1).width);

	for (size_t x = 0; x < Grid::WIDTH; ++x) {
		for (size_t y = 0; y < Grid::HEIGHT; ++y) {
			DrawTextureEx(textureManager->get(TextureId::Floor1),
						  {x * size + offset.x, y * size + offset.y}, 0.0f,
						  scale, WHITE);

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
								  grid, offset);
				break;
			}
			case Grid::CellState::Starting:
				StartingCellRender(static_cast<int>(x), static_cast<int>(y),
								   grid, offset);
				break;
            case Grid::CellState::HpUp:
                // No special rendering for HpUp cells
                break;
            default:
                break;
			}
			DrawRectangleLines(x * size + offset.x, y * size + offset.y, size,
							   size, BLACK);
		}
	}
}

void GridRender::ReveledCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	if (grid.cells[y][x].val > 0) {
		std::string text = std::to_string(grid.cells[y][x].val);
		DrawEnemy(textureManager, {x * size + offset.x, y * size + offset.y},
				  grid.cells[y][x].val);
		DrawText(text.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 8), 20, RED);
	} else {
		grid.cells[y][x].state = Grid::CellState::Hinting;
	}
}

void GridRender::PointsNotTakenCellRender(int x, int y, Grid &grid,
										  Vector2 offset) {
	int size = Grid::CELL_SIZE;
	DrawEnemy(textureManager, {x * size + offset.x, y * size + offset.y},
			  grid.cells[y][x].val);
	DrawRectangleLinesEx({x * size + offset.x, y * size + offset.y,
						  static_cast<float>(size), static_cast<float>(size)},
						 2.0f, ORANGE);
	if (grid.cells[y][x].val > 0) {
		std::string text = std::to_string(grid.cells[y][x].val);
		DrawText(text.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 2), 20, BLACK);
	}
}

void GridRender::HintingCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	int hint = gUtils::GetNeighboursSum(static_cast<int>(x),
										static_cast<int>(y), grid.cells);
	if (hint != 0) {
		std::string text2 = std::to_string(hint);
		DrawText(text2.c_str(), static_cast<int>(x * size + offset.x + 8),
				 static_cast<int>(y * size + offset.y + 8), 20, GREEN);
	}
}

void GridRender::StartingCellRender(int x, int y, Grid &grid, Vector2 offset) {
	int size = Grid::CELL_SIZE;
	DrawTextureEx(textureManager->get(TextureId::BeginButton),
                  {x * size + offset.x, y * size + offset.y}, 0.0f,
                  size / static_cast<float>(textureManager->get(TextureId::Floor1).width), WHITE);
}

void GridRender::RenderUi(const int hp, const int pointsToEvo,
						  const int maxHp) {
	std::string text3 = std::to_string(hp);
	DrawText(("HP: " + text3).c_str(), 10, 10, 20, BLUE);

	text3 = std::to_string(pointsToEvo);
	std::string text4 = std::to_string(maxHp);
	DrawText(("EV: " + text3 + "/" + text4).c_str(), 10, 50, 20, BLUE);
}

void GridRender::DrawEnemy(TextureManager const *textureManager,
						   const Vector2 position, const int type) {
	if (!textureManager) {
		return;
	}

	TextureId textureId;
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
	default:
		return; // Unknown type
	}

	const Texture2D &tex = textureManager->get(textureId);
	const float scale = Grid::CELL_SIZE / static_cast<float>(tex.height);
	const float scaledW = tex.width * scale;
	const float scaledH = tex.height * scale;
	const Vector2 drawPos{position.x + (Grid::CELL_SIZE - scaledW) * 0.5f,
						  position.y + (Grid::CELL_SIZE - scaledH) * 0.5f};

	DrawTextureEx(tex, drawPos, 0.0f, scale, WHITE);
}