#include "gridInterpreter.hpp"
#include "../entity/playerStats.hpp"
#include "grid.hpp"
#include "gridGenerator.hpp"
#include <raylib.h>

void GridInterpreter::Update(Grid *&grid, PlayerStats &playerStats, UI &ui,
							 InputManager &inputManager) {
	if (inputManager.IsLocked())
		return;

	if (this->ui == nullptr)
		this->ui = &ui;

	RecalculateHints(*grid);

	Vector2 mousePos = GetMousePosition();
	Vector2 offset = gUtils::GetOffset(*grid);
	int size = Grid::CELL_SIZE;

	int x{static_cast<int>((mousePos.x - offset.x) / static_cast<float>(size))};
	int y{static_cast<int>((mousePos.y - offset.y) / static_cast<float>(size))};

	if (x < 0 || y < 0 || x >= static_cast<int>(grid->GetWidth()) ||
		y >= static_cast<int>(grid->GetHeight())) {
		return;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !inputManager.IsLocked()) {
		inputManager.LockFor();
		if (grid->cells[y][x].state == CellState::Revealed ||
			grid->cells[y][x].state == CellState::pointsNotTaken) {
			return;
		}

		grid->cells[y][x].flagged = !grid->cells[y][x].flagged;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !inputManager.IsLocked()) {
		inputManager.LockFor();

		if (grid->cells[y][x].state != CellState::Hinting) {
			playerStats.curretTurn++;
			playerStats.wasGridClicked = true;
		}

		switch (grid->cells[y][x].state) {
		case CellState::Hidden:
			OnHidenClick(x, y, grid, playerStats, inputManager, ui);
			break;
		case CellState::Revealed:
			OnRevealedClick(x, y, grid, playerStats, ui);
			break;
		case CellState::pointsNotTaken:
			OnPointsNotTakenClick(x, y, *grid, playerStats.currentPointsToEvo);
			break;
		case CellState::Hinting:
			OnHintingClick(x, y, *grid);
			break;
		default:
			break;
		}
	}

	if (playerStats.hp < 0) {
		for (size_t y = 0; y < grid->GetHeight(); ++y) {
			for (size_t x = 0; x < grid->GetWidth(); ++x) {
				if (grid->cells[y][x].state != CellState::Hinting)
					grid->cells[y][x].state = CellState::Revealed;
			}
		}
	}
}

void GridInterpreter::OnHidenClick(int x, int y, Grid *&grid,
								   PlayerStats &playerStats,
								   InputManager &inputManager, UI &ui) {
	auto &cell = grid->cells[y][x];
	if (cell.specialFunction == SpecialFunction::Ladder ||
		cell.specialFunction == SpecialFunction::Mana ||
		cell.specialFunction == SpecialFunction::Heal ||
		cell.specialFunction == SpecialFunction::Chest ||
		cell.specialFunction == SpecialFunction::ChestKey) {
		cell.state = CellState::Revealed;
		return;
	}

	OnRevealedClick(x, y, grid, playerStats, ui);
}

void GridInterpreter::OnRevealedClick(int x, int y, Grid *&grid,
									  PlayerStats &playerStats, UI &ui) {
	auto &cell = grid->cells[y][x];

	if (cell.specialFunction == SpecialFunction::Starting) {
		cell.defeted = true;
		OnStartingClick(x, y, *grid);
		return;
	}

	if (cell.specialFunction == SpecialFunction::Necromancer) {
		const int roll = GetRandomValue(0, 2);
		if (roll == 0) {
			playerStats.hp -= cell.val;
			playerStats.currentPointsToEvo += 3;
			cell.defeted = true;
			cell.state = CellState::pointsNotTaken;
			return;
		}

		cell.defeted = true;
		cell.state = CellState::Hinting;
		cell.specialFunction = SpecialFunction::None;
		cell.val = 0;
		GridGenerator::PlaceSpecialFunction(*grid, SpecialFunction::Necromancer,
											1);
		return;
	}

	if (cell.specialFunction == SpecialFunction::ChestKey) {
		cell.defeted = true;
		playerStats.keys++;
		cell.state = CellState::Hinting;
		return;
	}

	if (cell.specialFunction == SpecialFunction::Chest) {
		if (playerStats.keys <= 0) {
			ui.TriggerMessageBox("You need a key to open this chest!");
			cell.state = CellState::Revealed;
			return;
		}

		cell.defeted = true;
		playerStats.keys--;
		playerStats.currentPointsToEvo += 3;
		playerStats.coins += GetRandomValue(20, 60);
		cell.state = CellState::Hinting;
		return;
	}

	if (cell.specialFunction == SpecialFunction::GoUpGrid) {
		if (grid->UpperGrid != nullptr) {
			DrawText("GRIDSWAPPED_________________", 10, 10, 40, GREEN);
			grid = grid->UpperGrid;
		}
		return;
	}

	cell.defeted = true;
	cell.state = CellState::Hinting;

	if (cell.specialFunction == SpecialFunction::Heal) {
		playerStats.HealToFull();
		return;
	}

	if (cell.specialFunction == SpecialFunction::Mana) {
		playerStats.currentPointsToEvo += 3;
		return;
	}

	if (cell.specialFunction == SpecialFunction::Ladder) {
		playerStats.currentLevel++;
		playerStats.shoudlNewLevelStart = true;
		return;
	}

	if (cell.val <= 0) {
		cell.state = CellState::Hinting;
		return;
	}

	playerStats.hp -= cell.val;
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
	UncoverStartingCellNeighbors(x, y, grid);
	grid.cells[y][x].state = CellState::Hinting;
}

void GridInterpreter::OnHealthClick(int x, int y, Grid &grid, int &hp,
									int &maxHp) {
	(void)x;
	(void)y;
	(void)grid;
	hp = maxHp;
}

void GridInterpreter::UncoverStartingCellNeighbors(int x, int y, Grid &grid) {

	std::vector<std::vector<Grid::Cell>> &cells = grid.cells;

	auto revealIfValid = [&](int rx, int ry) {
		if (rx < 0 || ry < 0 || rx >= static_cast<int>(grid.GetWidth()) ||
			ry >= static_cast<int>(grid.GetHeight())) {
			return;
		}

		auto &cell = cells[ry][rx];
		if (cell.specialFunction == SpecialFunction::None && cell.val <= 0) {
			cell.state = CellState::Hinting;
		} else {
			cell.state = CellState::Revealed;
		}
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
