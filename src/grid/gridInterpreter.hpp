#pragma once

#include "../entity/playerStats.hpp"
#include "../inputManager.hpp"
#include "../textureManager.hpp"
#include "grid.hpp"
#include "gridUtils.hpp"
#include <cstddef>
#include <raylib.h>

class Grid;

using std::size_t;

class GridInterpreter {
  public:
	using CellState = Grid::CellState;
	using Cell = Grid::Cell;
	using SpecialFunction = Grid::SpecialFunction;

	GridInterpreter() = default;
	~GridInterpreter() = default;

	void Update(Grid *&grid, PlayerStats &playerStats, UI &ui,
				InputManager &inputManager);

  private:
	void OnHidenClick(int x, int y, Grid *&grid, PlayerStats &playerStats,
					  InputManager &inputManager, UI &ui);
	void OnRevealedClick(int x, int y, Grid *&grid, PlayerStats &playerStats,
						 UI &ui);
	void OnPointsNotTakenClick(int x, int y, Grid &grid, int &pointsToEvo);
	void OnShopCellClick(int x, int y, Grid &grid, PlayerStats &playerStats);

	void OnHintingClick(int x, int y, Grid &grid);
	void OnStartingClick(int x, int y, Grid &grid);
	void OnNecromancerClick(int x, int y, Grid &grid, PlayerStats &playerStats);

	void OnHealthClick(int x, int y, Grid &grid, int &hp, int &maxHp);

	void UncoverStartingCellNeighbors(int x, int y, Grid &grid);
	void RecalculateHints(Grid &grid);

	void ApplyItem(int x, int y, Grid &grid, PlayerStats &playerStats);
	void Apply2x2(int x, int y, Grid &grid);

	UI *ui = nullptr;
};
