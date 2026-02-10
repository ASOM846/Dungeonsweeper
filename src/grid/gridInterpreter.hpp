#pragma once

#include <array>
#include <cstddef>
#include <raylib.h>

#include "../entity/playerStats.hpp"
#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridUtils.hpp"

class Grid;

using std::size_t;

class GridInterpreter {
  public:
	using CellState = Grid::CellState;
	using Cell = Grid::Cell;
	using SpecialFunction = Grid::SpecialFunction;

	GridInterpreter() = default;
	~GridInterpreter() = default;

	void Update(Grid &grid, PlayerStats &playerStats, UI &ui);

  private:
	void OnHidenClick(int x, int y, Grid &grid, int &hp);
	void OnRevealedClick(int x, int y, Grid &grid, PlayerStats &playerStats);
	void OnPointsNotTakenClick(int x, int y, Grid &grid, int &pointsToEvo);
	void OnHintingClick(int x, int y, Grid &grid);
	void OnStartingClick(int x, int y, Grid &grid);

	void OnHealthClick(int x, int y, Grid &grid, int &hp, int &maxHp);

	void UncoverStartingCellNeighbors(
		int x, int y,
		std::array<std::array<Grid::Cell, Grid::WIDTH>, Grid::HEIGHT> &cells);
	void RecalculateHints(Grid &grid);

	UI *ui;
};
