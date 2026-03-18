#pragma once
#include "../entity/playerStats.hpp"
#include "../grid/grid.hpp"
#include <iostream>
#include <raylib.h>
#include <string>

class Popup {
  public:
	Popup() = default;
	~Popup() = default;

	void Update(Grid &grid, PlayerStats &playerStats) {
		(void)grid;

		if (!playerStats.isSelectionPopup) {
			position = GetMousePosition();
			return;
		}

		int counter = 1;

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

				if (CheckCollisionPointRec(GetMousePosition(), rec))
					std::cout << "CLICKED:::      " << counter << std::endl;
				else
					playerStats.isSelectionPopup = false;

				++counter;
			}
		}
	}

	void Render(PlayerStats &playerStats) {
		if (!playerStats.isSelectionPopup)
			return;

		int counter = 1;

		for (int y = 0; y < collumns; ++y) {
			for (int x = 0; x < rows; ++x) {
				Vector2 topLeftCorner = {position.x + x * size,
										 position.y + y * size};

				DrawRectangleLines(static_cast<int>(topLeftCorner.x),
								   static_cast<int>(topLeftCorner.y), size,
								   size, RED);

				DrawText(std::to_string(counter).c_str(),
						 topLeftCorner.x + size / 2 - size / 4,
						 topLeftCorner.y + size / 2 - size / 4,
						 size / 2 + size / 4, RED);

				++counter;
			}
		}
	}

  private:
	Vector2 position;

	const short size = 40;

	const short collumns = 4;
	const short rows = 4;
};
