#include "gridGenerator.hpp"

void GridGenerator::Init(Grid &grid) {
	for (size_t y = 0; y < Grid::HEIGHT; ++y) {
		for (size_t x = 0; x < Grid::WIDTH; ++x) {
			auto &c = grid.cells[y][x];
			c.hint = 0;
			c.defeted = false;
			c.state = GridGenerator::CellState::Hidden;
			c.val = (GetRandomValue(1, 2) == 1) ? PickW(kMonsterW)
												: 0; // 1/2 na potworka
		}
	}

	int randomX = GetRandomValue(0, Grid::WIDTH - 1);
	int randomY = GetRandomValue(0, Grid::HEIGHT - 1);
	grid.cells[randomY][randomX].val = 0;
	grid.cells[randomY][randomX].state = GridGenerator::CellState::Starting;
}

void GridGenerator::Update(Grid &grid, int &hp, int &maxHp, int &pointsToEvo) {
	Vector2 mousePos = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 offset = gUtils::GetOffset(grid);
		int size = Grid::CELL_SIZE;

		int x = static_cast<int>((mousePos.x - offset.x) / size);
		int y = static_cast<int>((mousePos.y - offset.y) / size);

		if (x < 0 || y < 0 || x >= Grid::WIDTH || y >= Grid::HEIGHT) {
			return;
		}

		switch (grid.cells[y][x].state) {
		case CellState::Hidden:
			OnHidenClick(x, y, grid, hp);
			break;
		case CellState::Revealed:
			OnRevealedClick(x, y, grid, hp);
			break;
		case CellState::pointsNotTaken:
			OnPointsNotTakenClick(x, y, grid, pointsToEvo);
			break;
		case CellState::Hinting:
			OnHintingClick(x, y, grid);
			break;
		case CellState::Starting:
			OnStartingClick(x, y, grid);
			break;
		default:
			break;
		}
	}

	if (IsKeyPressed(KEY_E) || IsKeyPressed(KEY_SPACE)) {
		if (pointsToEvo >= maxHp) {
			maxHp += 1;
			pointsToEvo = 0;
			hp = maxHp;
		}
	}

	if (hp < 0) {
		for (size_t y = 0; y < Grid::HEIGHT; ++y) {
			for (size_t x = 0; x < Grid::WIDTH; ++x) {
				if (grid.cells[y][x].state != CellState::Hinting)
					grid.cells[y][x].state = CellState::Revealed;
			}
		}
	}

	RecalculateHints(grid);
}

void GridGenerator::RecalculateHints(Grid &grid) {
	for (size_t y = 0; y < Grid::HEIGHT; ++y) {
		for (size_t x = 0; x < Grid::WIDTH; ++x) {
			grid.cells[y][x].hint = gUtils::GetNeighboursSum(
				static_cast<int>(x), static_cast<int>(y), grid.cells);
		}
	}
}

void GridGenerator::OnHidenClick(int x, int y, Grid &grid, int &hp) {
	if (grid.cells[y][x].val > 0) {
		hp -= grid.cells[y][x].val;
		grid.cells[y][x].state = CellState::pointsNotTaken;
	} else {
		grid.cells[y][x].state = CellState::Hinting;
	}
}

void GridGenerator::OnRevealedClick(int x, int y, Grid &grid, int &hp) {
	hp -= grid.cells[y][x].val;
	grid.cells[y][x].state = CellState::pointsNotTaken;
}

void GridGenerator::OnPointsNotTakenClick(int x, int y, Grid &grid,
										  int &pointsToEvo) {
	pointsToEvo += grid.cells[y][x].val;
	grid.cells[y][x].state = CellState::Hinting;
	grid.cells[y][x].defeted = true;
}

void GridGenerator::OnHintingClick(int x, int y, Grid &grid) {

}
void GridGenerator::OnStartingClick(int x, int y, Grid &grid) {
	grid.cells[y][x].val = 0;
	UncoverStartingCellNeighbors(x, y, grid.cells);
	grid.cells[y][x].state = CellState::Revealed;
}

void GridGenerator::UncoverStartingCellNeighbors(
	int x, int y,
	std::array<std::array<Grid::Cell, Grid::WIDTH>, Grid::HEIGHT> &cells) {
	auto revealIfValid = [&](int rx, int ry) {
		if (rx < 0 || ry < 0 || rx >= static_cast<int>(Grid::WIDTH) ||
			ry >= static_cast<int>(Grid::HEIGHT)) {
			return;
		}
		cells[ry][rx].state = CellState::Revealed;
	};

	for (int dy = -1; dy <= 1; ++dy) {
		for (int dx = -1; dx <= 1; ++dx) {
			if (dx == 0 && dy == 0) {
				continue;
			}
			revealIfValid(x + dx, y + dy);
		}
	}

	revealIfValid(x, y - 2);
	revealIfValid(x, y + 2);
	revealIfValid(x - 2, y);
	revealIfValid(x + 2, y);
}
