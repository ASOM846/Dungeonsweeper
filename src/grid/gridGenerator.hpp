#pragma once

#include <array>
#include <cstddef>
#include <raylib.h>
#include <string>
#include <utility>

#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridUtils.hpp"

namespace {
constexpr std::array<std::pair<int, int>, 12> kMonsterW{
	{{1, 11},
	 {2, 11},
	 {3, 10},
	 {4, 8},
	 {5, 7},
	 {6, 5},
	 {7, 4},
	 {8, 5},
	 {9, 2},
	 {10, 1},
	 {11, 1},
	 {100, 9}}}; // hp increaser
template <size_t N> int PickW(const std::array<std::pair<int, int>, N> &t) {
	int total = 0;
	for (auto &e : t)
		total += e.second;
	int r = GetRandomValue(1, total), acc = 0;
	for (auto &e : t) {
		acc += e.second;
		if (r <= acc)
			return e.first;
	}
	return t.back().first;
}
} // namespace

using std::size_t;

class GridGenerator {
  public:
	using CellState = Grid::CellState;
	using Cell = Grid::Cell;
	using SpecialFunction = Grid::SpecialFunction;

	void Init(Grid &grid);

	void Update(Grid &grid, int &hp, int &maxHp, int &pointsToEvo);
	void Render(const TextureManager *textureManager, Grid &grid);

  private:
	SpecialFunction GetSpecialFunction();
};
