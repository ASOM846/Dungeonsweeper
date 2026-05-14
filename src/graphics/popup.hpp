#pragma once
#include "../entity/playerStats.hpp"
#include "../grid/grid.hpp"
#include "../grid/gridUtils.hpp"
#include <raylib.h>
#include <string>

class Popup {
  public:
	Popup() = default;
	~Popup() = default;

	void Init() {
		for (int i = 0; i < 12; i++) {
			contains.emplace_back(std::to_string(i));
		}

		contains.emplace_back("!");
		contains.emplace_back("F");
		contains.emplace_back("?");
		contains.emplace_back("*");
	}

	void Update(Grid &grid, PlayerStats &playerStats) {
		if (!playerStats.isSelectionPopup) { ///////wymaga optymalizacji, niech
											 /// update selected cell sie nie
											 /// wywoluje gdy nie ma focusu
											 ///(!playerStats.isSelectionPopup)
			position = GetMousePosition();
			UpdateSelectedCell(position, gUtils::GetOffset(grid),
							   grid.CELL_SIZE);
			return;
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
			position = GetMousePosition();
			UpdateSelectedCell(position, gUtils::GetOffset(grid),
							   grid.CELL_SIZE);
		}

		int counter = 0;
		const Vector2 mousePos = GetMousePosition();

		for (int y = 0; y < collumns; ++y) {
			for (int x = 0; x < rows; ++x) {
				if (!IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT))
					return;

				Vector2 topLeftCorner = {position.x + x * size,
										 position.y + y * size};
				Rectangle rec = {
					topLeftCorner.x,
					topLeftCorner.y,
					static_cast<float>(size),
					static_cast<float>(size),
				};

				if (CheckCollisionPointRec(mousePos, rec))
					grid.cells[cellY][cellX].flagVal = contains[y * 4 + x];
				else
					playerStats.isSelectionPopup = false;

				++counter;
			}
		}
	}

	void Render(PlayerStats &playerStats) {
		if (!playerStats.isSelectionPopup)
			return;

		int counter = 0;

		for (int y = 0; y < collumns; ++y) {
			for (int x = 0; x < rows; ++x) {
				Vector2 topLeftCorner = {position.x + x * size,
										 position.y + y * size};

				Color borderColor = Color{110, 90, 70, 255};
				Color bgColor = Color{28, 24, 20, 255};
				Color textColor = Color{220, 200, 170, 255};

				DrawRectangle(static_cast<int>(topLeftCorner.x),
							  static_cast<int>(topLeftCorner.y), size, size,
							  bgColor);

				DrawRectangleLines(static_cast<int>(topLeftCorner.x),
								   static_cast<int>(topLeftCorner.y), size,
								   size, borderColor);

				DrawText(
					contains[y * 4 + x].c_str(),
					static_cast<int>(topLeftCorner.x + size / 2 - size / 4),
					static_cast<int>(topLeftCorner.y + size / 2 - size / 4),
					size / 2 + size / 4, textColor);

				++counter;
			}
		}
	}

	void UpdateSelectedCell(Vector2 mousePos, Vector2 gridOffset,
							int CellSize) {
		cellX = static_cast<size_t>((mousePos.x - gridOffset.x)) /
				static_cast<size_t>(CellSize);
		cellY = static_cast<size_t>((mousePos.y - gridOffset.y)) /
				static_cast<size_t>(CellSize);
	}

  private:
	Vector2 position;

	std::vector<std::string> contains;

	size_t cellX;
	size_t cellY;

	const short size = 40;

	const short collumns = 4;
	const short rows = 4;
};
