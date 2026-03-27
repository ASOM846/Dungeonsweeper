#pragma once
#include <raylib.h>
#include <vector>

struct Grid {
  public:
	[[nodiscard]] size_t GetWidth() const { return width; }
	[[nodiscard]] size_t GetHeight() const { return height; }

	static constexpr int CELL_SIZE = 66;

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
		OgreBig,
		OgreSmall,
		Chest,
		ChestKey,
		Heal,
		Mana,
		Ladder,
		GoUpGrid,
		Necromancer,
		MysteryCell,
		MysteryMonster,
	};

	struct Cell {
		short textureNumber = 1;
		short rotation = 0;
		int hint = 0;
		int val = 0;
		bool defeted = false;
		int flagVal = 0;
		CellState state = CellState::Hidden;
		SpecialFunction specialFunction = SpecialFunction::None;
		Grid *LowerGrid = nullptr;
		short animationFrame = 0;
	};

	enum class GridType {
		Classic,
		Endless,
		Shop,
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
