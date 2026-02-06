#pragma once
#include <array>
#include <raylib.h>

struct Grid {
  public:
	static constexpr int WIDTH = 13;
	static constexpr int HEIGHT = 10;
	static constexpr int CELL_SIZE = 66;
	// static constexpr int CELL_SIZE = 80;

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
		short textureNumber = 1;
		short rotation = 0;
		int hint = 0;
		int val = 0;
		bool defeted = false;
		CellState state = CellState::Hidden;
		SpecialFunction specialFunction = SpecialFunction::None;
	};

	std::array<std::array<Cell, WIDTH>, HEIGHT> cells;
};
