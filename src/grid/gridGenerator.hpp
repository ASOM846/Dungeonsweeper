#pragma once
#include "../entity/playerStats.hpp"
#include "../gameMode.hpp"
#include "../textureManager.hpp"
#include "grid.hpp"
#include <array>
#include <cstddef>
#include <raylib.h>
#include <utility>

namespace {
constexpr std::array<std::pair<int, int>, 12> kMonsterW{{{1, 11},
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
														 {100, 6}}};

constexpr std::array<std::pair<int, int>, 12> kMonsterW2{{{1, 9},
														  {2, 9},
														  {3, 8},
														  {4, 5},
														  {5, 6},
														  {6, 5},
														  {7, 7},
														  {8, 8},
														  {9, 9},
														  {10, 6},
														  {11, 6},
														  {100, 15}}};

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

	void Init(Grid &grid, PlayerStats &playerStats, GameMode gameMode,
			  GridConfig config);

	void Update(Grid &grid, int &hp, int &maxHp, int &pointsToEvo);
	void Render(const TextureManager *textureManager, Grid &grid);

  private:
	Grid::GridType GetGridTypeForGameMode(const GameMode &gm, const Grid &grid);
	static void PlaceSpecialFunction(Grid &grid, Grid::SpecialFunction funct,
									 int count, bool shouldBeInCenter);
	void InitOgre(Grid &grid);
	int GetFlorTextureNumber();
	SpecialFunction GetSpecialFunction();

	friend class GridInterpreter;
};
