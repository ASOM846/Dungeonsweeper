#pragma once
#include "../entity/playerStats.hpp"
#include "../grid/grid.hpp"
#include <iostream>
#include <raylib.h>

class Popup {
  public:
	Popup() = default;
	~Popup() = default;

	void Update(Grid &grid, PlayerStats &playerStats) {
		(void)grid;

		if (!playerStats.isSelectionPopup)
			return;

		position = GetMousePosition();
	}

	void Render(PlayerStats &playerStats) {
		if (!playerStats.isSelectionPopup)
			return;

		std::cout << "DRAWING POPUP" << position.x << position.y << std::endl;

		for (int y = 0; y < collumns; ++y) {
			for (int x = 0; x < rows; ++x) {
				Vector2 topLeftCorner = {position.x + x * size,
										 position.y + y * size};

				DrawRectangle(topLeftCorner.x, topLeftCorner.y, size, size,
							  RED);
			}
		}
	}

  private:
	Vector2 position;

	const short size = 30;

	const short collumns = 4;
	const short rows = 4;
};
