#pragma once
#include <raylib.h>
#include <vector>

struct Grid {
  public:
	size_t GetWidth() { return width; }
	size_t GetHeight() { return height; }

	static constexpr int CELL_SIZE = 66;

	static constexpr int NUMBER_OF_HEARTS = 5;
	static constexpr int NUMBER_OF_MANA = 6;
	static constexpr int NUMBER_OF_KEYS = 2;
	static constexpr int NUMBER_OF_CHESTS = 2;
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
		GoUpGrid,
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
		Grid *LowerGrid = nullptr;
	};

	enum class GridType {
		Classic,
		Endless,
		Shop,
		Shirene,
	};

	std::vector<std::vector<Cell>> cells;

	GridType GetType() { return type; }
	void SetType(const GridType gt) { type = gt; }
	Grid(size_t w, size_t h, Grid *grid, GridType gridType = GridType::Classic)
		: cells(h, std::vector<Cell>(w)), UpperGrid(grid), type(gridType),
		  width(w), height(h) {}

	Cell &GetCell(Vector2 position) {
		return cells[static_cast<size_t>(position.y)]
					[static_cast<size_t>(position.x)];
	}

	Grid *UpperGrid = nullptr;

  private:
	GridType type;

	size_t width;
	size_t height;
};
