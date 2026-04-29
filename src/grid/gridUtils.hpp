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

	result.x =
		static_cast<float>(GetScreenWidth() - mapWidth - UI::UI_BAR_WIDTH) / 2;
	result.y = static_cast<float>(GetScreenHeight() - mapHeight) / 2;

	return result;
}

inline int GetNeighboursSum(int x, int y, Grid &grid) {

	std::vector<std::vector<Grid::Cell>> &cells = grid.cells;

	int sum = 0;
	size_t bX = x;
	size_t bY = y;

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

inline void UncoverNeighouring0(int x, int y, Grid &grid) {

	std::vector<std::vector<Grid::Cell>> &cells = grid.cells;

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

			if (cells[bY][bX].state != Grid::CellState::Revealed) {
				cells[bY][bX].state = Grid::CellState::Revealed;

				if (gUtils::GetNeighboursSum(bX, bY, grid) == 0) {
					UncoverNeighouring0(bX, bY, grid);
				}
			}
			bY++;
		}
		bY = y - 1;
		bX++;
	}
}
} // namespace gUtils
