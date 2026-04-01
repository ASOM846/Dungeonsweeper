#include "gridGenerator.hpp"
#include "../gameMode.hpp"
#include "../utils/randomUtils.hpp"
#include "grid.hpp"
#include <raylib.h>

void GridGenerator::Init(Grid &grid, PlayerStats &playerStats,
						 GameMode gameMode, GridConfig config) {
	(void)gameMode;

	// Base fill
	for (size_t y = 0; y < grid.GetHeight(); ++y) {
		for (size_t x = 0; x < grid.GetWidth(); ++x) {
			auto &c = grid.cells[y][x];
			c.hint = 0;
			c.defeted = false;
			c.state = GridGenerator::CellState::Hidden;
			c.specialFunction = Grid::SpecialFunction::None;
			c.textureNumber = GetFlorTextureNumber();
			c.rotation = GetRandomValue(0, 3);
			c.flagVal = 0;

			int seed = GetRandomValue(1, 20);
			if (seed <= 15) {
				if (playerStats.currentLevel < 3) {
					c.val = PickW(kMonsterW);
				} else {
					c.val = PickW(kMonsterW2);
				}
			} else {
				c.val = 0;
			}
		}
	}

	PlaceSpecialFunction(grid, Grid::SpecialFunction::Starting, 1, true);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::UncoverEnemiesVal1, 1,
						 false);

	InitOgre(grid);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::MysteryMonster,
						 config.numberOfMysteryEnemies, 1);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::Necromancer,
						 config.numberOfNecromancers, 0);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::Heal,
						 config.numberOfHearts, 0);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::Mana, config.numberOfMana,
						 0);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::ChestKey,
						 config.numberOfKeys, 0);

	PlaceSpecialFunction(grid, Grid::SpecialFunction::Chest,
						 config.numberOfChests, 0);

	if (gameMode == GameMode::Classic) {
		PlaceSpecialFunction(grid, Grid::SpecialFunction::Wizzard, 1, 0);
	} else {
		PlaceSpecialFunction(grid, Grid::SpecialFunction::Ladder, 1, 0);
	}

	if (grid.UpperGrid != nullptr) {
		PlaceSpecialFunction(grid, Grid::SpecialFunction::GoUpGrid, 1, 0);
	}

	// for (size_t y = 0; y < grid.GetHeight(); ++y) {
	// 	for (size_t x = 0; x < grid.GetWidth(); ++x) {
	// 		grid.cells[y][x].state = Grid::CellState::Revealed;
	// 	}
	// }
}

Grid::GridType GridGenerator::GetGridTypeForGameMode(const GameMode &gm,
													 const Grid &grid) {
	(void)grid;
	switch (gm) {
	case GameMode::Classic:
		return Grid::GridType::Classic;
	case GameMode::Endless:
		return Grid::GridType::Endless;
	default:
		return Grid::GridType::Classic;
	}
}

void GridGenerator::PlaceSpecialFunction(Grid &grid,
										 Grid::SpecialFunction funct,
										 int count = 1,
										 bool shouldBeInCenter = true) {
	for (int i = 0; i < count; ++i) {
		for (int attempts = 0; attempts < 500; ++attempts) {
			size_t x;
			size_t y;

			if (shouldBeInCenter) {
				x = util::GetRandom(0, grid.GetWidth() - 4);
				y = util::GetRandom(0, grid.GetHeight() - 4);

				x += 2;
				y += 2;
			} else {
				x = util::GetRandom(0, grid.GetWidth() - 1);
				y = util::GetRandom(0, grid.GetHeight() - 1);
			}

			if (grid.cells[y][x].specialFunction !=
				Grid::SpecialFunction::None) {
				continue;
			}

			grid.cells[y][x].specialFunction = funct;
			grid.cells[y][x].val = 0;

			if (funct == Grid::SpecialFunction::GoUpGrid) {
				grid.cells[y][x].state = Grid::CellState::Revealed;
			}

			if (funct == Grid::SpecialFunction::Necromancer) {
				grid.cells[y][x].val = 6;
			}

			if (funct == Grid::SpecialFunction::Starting ||
				funct == Grid::SpecialFunction::Wizzard) {
				if (funct == Grid::SpecialFunction::Starting) {
					grid.cells[y][x].val = 0;
				} else {
					grid.cells[y][x].val = 13;
				}
				grid.cells[y][x].state = Grid::CellState::Revealed;
			}

			break;
		}
	}
}

void GridGenerator::InitOgre(Grid &grid) {
	static constexpr std::array<std::pair<int, int>, 8> kRelativePositions = {
		std::pair<int, int>{-1, 0},	 std::pair<int, int>{1, 0},
		std::pair<int, int>{0, -1},	 std::pair<int, int>{0, 1},
		std::pair<int, int>{-1, -1}, std::pair<int, int>{-1, 1},
		std::pair<int, int>{1, -1},	 std::pair<int, int>{1, 1},
	};

	for (int attempts = 0; attempts < 500; ++attempts) {
		const size_t x =
			static_cast<size_t>(GetRandomValue(1, grid.GetWidth() - 2));

		const size_t y =
			static_cast<size_t>(GetRandomValue(1, grid.GetHeight() - 2));

		auto isFree = [&](size_t cx, size_t cy) {
			return grid.cells[cy][cx].specialFunction ==
				   Grid::SpecialFunction::None;
		};

		if (!isFree(x, y)) {
			continue;
		}

		bool canPlace = true;
		for (const auto &[dx, dy] : kRelativePositions) {
			if (!isFree(x + dx, y + dy)) {
				canPlace = false;
				break;
			}
		}

		if (!canPlace) {
			continue;
		}

		grid.cells[y][x].specialFunction = Grid::SpecialFunction::OgreBig;
		grid.cells[y][x].val = 8;

		for (const auto &[dx, dy] : kRelativePositions) {
			auto &neighbor = grid.cells[y + dy][x + dx];
			neighbor.specialFunction = Grid::SpecialFunction::OgreSmall;
			neighbor.val = 5;
		}
		return;
	}
}

int GridGenerator::GetFlorTextureNumber() {
	int seed = GetRandomValue(1, 14);
	if (seed <= 6)
		return 1;

	return seed - 6;
}

GridGenerator::SpecialFunction GridGenerator::GetSpecialFunction() {
	static const std::array<std::pair<GridGenerator::SpecialFunction, int>, 2>
		kSpecials = {std::make_pair(GridGenerator::SpecialFunction::Heal, 14),
					 std::make_pair(GridGenerator::SpecialFunction::Mana, 10)};
	int total = 0;
	for (const auto &e : kSpecials)
		total += e.second;
	int r = GetRandomValue(1, total), acc = 0;
	for (const auto &e : kSpecials) {
		acc += e.second;
		if (r <= acc)
			return e.first;
	}
	return kSpecials.back().first;
}
