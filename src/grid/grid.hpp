#pragma once
#include <raylib.h>
#include <array>

struct Grid {
  public:
	static constexpr int WIDTH = 13;
	static constexpr int HEIGHT = 10;
	static constexpr int CELL_SIZE = 56;

	enum class CellState {
        //main states
		Hidden,			// not giving info
		Revealed,		// showing cell hp(value)
		pointsNotTaken, // showing cell val and different colour
		Hinting,		// showing hint value

        //special state
		Starting,
        HpUp,
	};

	struct Cell {
		int hint = 0;
		int val = 0; // base hp and texture determiner
		bool defeted = false;
		CellState state = CellState::Hidden;
	};

	std::array<std::array<Cell, WIDTH>, HEIGHT> cells;
};