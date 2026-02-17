#pragma once

#include "../graphics/ui.hpp"
#include "grid.hpp"
#include <cstddef>
#include <raylib.h>
#include <vector>

namespace gUtils {
inline Vector2 GetOffset(Grid &grid) {
	Vector2 result;

	const int tileSize = Grid::CELL_SIZE;
	const int mapWidth = static_cast<int>(grid.cells[0].size()) * tileSize;
	const int mapHeight = static_cast<int>(grid.cells.size()) * tileSize;

	result.x = (GetScreenWidth() - mapWidth - UI::UI_BAR_WIDTH) / 2;
	result.y = (GetScreenHeight() - mapHeight) / 2;

	return result;
}

inline int GetNeighboursSum(int x, int y,
							std::vector<std::vector<Grid::Cell>> &cells,
							Grid &grid) {
	int sum = 0;
	int bX = x;
	int bY = y;

	bX--;
	bY--;

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (bX == x && bY == y) {
				bY++;
				continue;
			}

			if (bX < 0 || bY < 0 || bX >= grid.GetWidth() ||
				bY >= grid.GetHeight()) {
				bY++;
				continue;
			}
			if (cells[bY][bX].defeted) {
				bY++;
				continue;
			}
			sum += cells[bY][bX].val;
			bY++;
		}
		bY = y - 1;
		bX++;
	}

	return sum;
}

inline void UncoverNeighouring0(
	int x, int y,
	std::array<std::array<Grid::Cell, Grid::WIDTH>, Grid::HEIGHT> &cells) {

	int bX = x;
	int bY = y;

	bX--;
	bY--;

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (bX == x && bY == y) {
				bY++;
				continue;
			}

			if (bX < 0 || bY < 0 || bX >= Grid::WIDTH || bY >= Grid::HEIGHT) {
				bY++;
				continue;
			}

			if (cells[bY][bX].state != Grid::CellState::Revealed) {
				cells[bY][bX].state = Grid::CellState::Revealed;

				if (gUtils::GetNeighboursSum(bX, bY, cells) == 0) {
					UncoverNeighouring0(bX, bY, cells);
				}
			}
			bY++;
		}
		bY = y - 1;
		bX++;
	}
}
} // namespace gUtils
