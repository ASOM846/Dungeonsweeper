#include "gridGenerator.hpp"

void GridGenerator::Init(Grid &grid) {
	for (size_t y = 0; y < Grid::HEIGHT; ++y) {
		for (size_t x = 0; x < Grid::WIDTH; ++x) {
			auto &c = grid.cells[y][x];
			c.hint = 0;
			c.defeted = false;
			c.state = GridGenerator::CellState::Hidden;
			c.specialFunction = Grid::SpecialFunction::None;

			int seed = GetRandomValue(1, 20);
			if (seed <= 15) {
				c.val = PickW(kMonsterW);
			} else if (seed <= 17) {
				c.specialFunction = GetSpecialFunction();
			} else {
				c.val = 0;
			}
		}
	}

	// for (size_t y = 0; y < Grid::HEIGHT; ++y) {
	// 	for (size_t x = 0; x < Grid::WIDTH; ++x) {
	// 		grid.cells[y][x].state = GridGenerator::CellState::Revealed;
	// 	}
	// }

	int randomX = GetRandomValue(0, Grid::WIDTH - 1);
	int randomY = GetRandomValue(0, Grid::HEIGHT - 1);
	grid.cells[randomY][randomX].val = 0;
	grid.cells[randomY][randomX].state = GridGenerator::CellState::Starting;
}

GridGenerator::SpecialFunction GridGenerator::GetSpecialFunction() {
	static const std::array<std::pair<GridGenerator::SpecialFunction, int>, 2>
		kSpecials = {std::make_pair(GridGenerator::SpecialFunction::Heal, 10),
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
