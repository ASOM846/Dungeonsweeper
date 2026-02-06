#pragma once
#include <array>
#include <raylib.h>

struct Grid {
  public:
	static constexpr int WIDTH = 10;
	static constexpr int HEIGHT = 10;
	static constexpr int CELL_SIZE = 56;
	//static constexpr int CELL_SIZE = 80;

	enum class CellState {
		// main states
		Hidden,			// not giving info
		Revealed,		// showing cell hp(value)
		pointsNotTaken, // showing cell val and different colour
		Hinting,		// showing hint value
		Starting,
	};

	enum class SpecialFunction {
		None,
		Heal,
		Mana,
	};

	struct Cell {
		int hint = 0;
		int val = 0; // base hp and texture determiner
		bool defeted = false;
		CellState state = CellState::Hidden;
		SpecialFunction specialFunction = SpecialFunction::None;
	};

	std::array<std::array<Cell, WIDTH>, HEIGHT> cells;
};
