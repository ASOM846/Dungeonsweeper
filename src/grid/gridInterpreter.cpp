#include "gridInterpreter.hpp"

void GridInterpreter::Update(Grid &grid, PlayerStats &playerStats) {
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
			OnHidenClick(x, y, grid, playerStats.hp);
			break;
		case CellState::Revealed:
			OnRevealedClick(x, y, grid, playerStats.hp, playerStats.maxHp,
							playerStats.currentPointsToEvo);
			break;
		case CellState::pointsNotTaken:
			OnPointsNotTakenClick(x, y, grid, playerStats.currentPointsToEvo);
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

	if (playerStats.hp < 0) {
		for (size_t y = 0; y < Grid::HEIGHT; ++y) {
			for (size_t x = 0; x < Grid::WIDTH; ++x) {
				if (grid.cells[y][x].state != CellState::Hinting)
					grid.cells[y][x].state = CellState::Revealed;
			}
		}
	}

	RecalculateHints(grid);
}

void GridInterpreter::OnHidenClick(int x, int y, Grid &grid, int &hp) {
	auto &cell = grid.cells[y][x];
	if (cell.specialFunction == SpecialFunction::Heal) {
		cell.state = CellState::Revealed;
		return;
	}

	if (cell.val > 0) {
		hp -= grid.cells[y][x].val;
		grid.cells[y][x].state = CellState::pointsNotTaken;
	} else {
		grid.cells[y][x].state = CellState::Hinting;
	}
}

void GridInterpreter::OnRevealedClick(int x, int y, Grid &grid, int &hp,
									  int &maxHp, int &currentPointsToEvo) {
	auto &cell = grid.cells[y][x];

	cell.defeted = true;
	cell.state = CellState::Hinting;
	if (cell.specialFunction == SpecialFunction::Heal) {
		hp = maxHp;
		return;
	}

	if (cell.specialFunction == SpecialFunction::Mana) {
		currentPointsToEvo += 3;
		return;
	}

	hp -= cell.val;
	cell.state = CellState::pointsNotTaken;
}

void GridInterpreter::OnPointsNotTakenClick(int x, int y, Grid &grid,
											int &currentPointsToEvo) {
	currentPointsToEvo += grid.cells[y][x].val;
	grid.cells[y][x].state = CellState::Hinting;
	grid.cells[y][x].defeted = true;
}

void GridInterpreter::OnHintingClick(int x, int y, Grid &grid) {
	(void)x;
	(void)y;
	(void)grid;
}

void GridInterpreter::RecalculateHints(Grid &grid) {}

void GridInterpreter::OnStartingClick(int x, int y, Grid &grid) {
	grid.cells[y][x].val = 0;
	UncoverStartingCellNeighbors(x, y, grid.cells);
	grid.cells[y][x].state = CellState::Revealed;
}

void GridInterpreter::OnHealthClick(int x, int y, Grid &grid, int &hp,
									int &maxHp) {
	(void)x;
	(void)y;
	(void)grid;
	hp = maxHp;
}

void GridInterpreter::UncoverStartingCellNeighbors(
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
