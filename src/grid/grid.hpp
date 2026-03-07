#pragma once
#include "../entity/item.hpp"
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
		MiniDungeonEntry,
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
		ItemCell,
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
		Item item;
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
	Grid(int w, int h, Grid *grid, GridType gridType = GridType::Classic)
		: width(w), height(h), UpperGrid(grid), type(gridType),
		  cells(h, std::vector<Cell>(w)) {}

	Cell &GetCell(Vector2 position) { return cells[position.y][position.x]; }

	Grid *UpperGrid = nullptr;

  private:
	GridType type;

	int width;
	int height;
};
