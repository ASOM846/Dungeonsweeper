#pragma once
#include <raylib.h>
#include <vector>

struct Grid {
  public:
	int GetWidth() { return width; }
	int GetHeight() { return height; }

	static constexpr int CELL_SIZE = 66;

	static constexpr int NUMBER_OF_HEARTS = 5;
	static constexpr int NUMBER_OF_MANA = 6;
	static constexpr int NUMBER_OF_KEYS = 2;
	static constexpr int NUMBER_OF_CHESTS = 1;
	static constexpr int NUMBER_OF_NECROMANCERS = 1;

	enum class CellState {
		Hidden,
		Revealed,
		pointsNotTaken,
		Hinting,
	};

	enum class SpecialFunction {
		None,
		Starting,
		Wizzard,
		SwordRed,
		SwordGreen,
		SwordGold,
		OgreBig,
		OgreSmall,
		Chest,
		ChestKey,
		Heal,
		Mana,
		Ladder,
		Necromancer,
	};

	struct Cell {
		short textureNumber = 1;
		short rotation = 0;
		int hint = 0;
		int val = 0;
		bool defeted = false;
		bool flagged = false;
		CellState state = CellState::Hidden;
		SpecialFunction specialFunction = SpecialFunction::None;
	};

	std::vector<std::vector<Cell>> cells;

	Grid(int w, int h) : width(w), height(h), cells(h, std::vector<Cell>(w)) {}

  private:
	int width = 13;
	int height = 10;
};
