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
	void ResetInput();

  private:
	void OnHidenClick(int x, int y, Grid &grid, PlayerStats &playerStats);
	void OnRevealedClick(int x, int y, Grid &grid, PlayerStats &playerStats);
	void OnPointsNotTakenClick(int x, int y, Grid &grid, int &pointsToEvo);
	void OnHintingClick(int x, int y, Grid &grid);
	void OnStartingClick(int x, int y, Grid &grid);
	void OnNecromancerClick(int x, int y, Grid &grid, PlayerStats &playerStats);

	void OnHealthClick(int x, int y, Grid &grid, int &hp, int &maxHp);

	void UncoverStartingCellNeighbors(int x, int y, Grid &grid);
	void RecalculateHints(Grid &grid);

	UI *ui = nullptr;
	bool mouseInputArmed = false;
};
