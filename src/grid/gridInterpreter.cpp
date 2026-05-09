#include "gridInterpreter.hpp"
#include "../entity/playerStats.hpp"
#include "grid.hpp"
#include "gridGenerator.hpp"
#include <raylib.h>

void GridInterpreter::Update(Grid *&grid, PlayerStats &playerStats, UI &ui,
							 InputManager &inputManager) {
	const int framesSpeed = 3;
	for (size_t y = 0; y < grid->GetHeight(); ++y) {
		for (size_t x = 0; x < grid->GetWidth(); ++x) {
			Grid::Cell &cell = grid->cells[y][x];

			if (cell.state == CellState::Revealing) {
				cell.framesCounter++;

				if (cell.framesCounter >= framesSpeed) {
					cell.framesCounter = 0;
					cell.animationFrame++;

					if (cell.animationFrame > 7) {
						cell.state = CellState::Revealed;
						cell.animationFrame = 0;
						cell.framesCounter = 0;
					}
				}
			}
		}
	}

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

		playerStats.isSelectionPopup = true;
	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !inputManager.IsLocked()) {
		inputManager.LockFor();

		if (grid->cells[y][x].state != CellState::Hinting) {
			playerStats.curretTurn++;
			playerStats.wasGridClicked = true;
		}

		if (grid->cells[y][x].state != CellState::Revealed &&
			grid->cells[y][x].state != CellState::Hinting) {
			playerStats.IncreaseTime();
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

	if (playerStats.hp < 0 || playerStats.gameWon) {
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
		cell.specialFunction == SpecialFunction::ChestKey ||
		cell.specialFunction == Grid::SpecialFunction::UncoverEnemiesVal1) {
		cell.state = CellState::Revealing;
		return;
	}

	OnRevealedClick(x, y, grid, playerStats, ui);
}

void GridInterpreter::OnRevealedClick(int x, int y, Grid *&grid,
									  PlayerStats &playerStats, UI &ui) {
	auto &cell = grid->cells[y][x];

	playerStats.addScore(cell.val);

	switch (cell.specialFunction) {
	case Grid::SpecialFunction::Starting:
		cell.defeted = true;
		OnStartingClick(x, y, *grid);
		break;

	case Grid::SpecialFunction::Wizzard: {
		playerStats.gameWon = true;
	}

	case Grid::SpecialFunction::Necromancer: {
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
											1, 0);
		return;
	}

	case Grid::SpecialFunction::UncoverEnemiesVal1:
		for (size_t y = 0; y < grid->GetHeight(); ++y) {
			for (size_t x = 0; x < grid->GetWidth(); ++x) {
				if (grid->cells[y][x].val == 1 &&
					grid->cells[y][x].state != Grid::CellState::Hinting &&
					grid->cells[y][x].state != Grid::CellState::pointsNotTaken)
					grid->cells[y][x].state = Grid::CellState::Revealing;
			}
		}
		break;

	case Grid::SpecialFunction::ChestKey:
		cell.defeted = true;
		playerStats.keys++;
		cell.state = CellState::Hinting;
		break;

	case Grid::SpecialFunction::Chest: {
		if (playerStats.keys <= 0) {
			ui.TriggerMessageBox("You need a key to open this chest!");
			cell.state = CellState::Revealed;
			return;
		}

		cell.defeted = true;
		playerStats.keys--;
		playerStats.isChoosePending = true;
		playerStats.currentPointsToEvo += GetRandomValue(8, 14);
		playerStats.coins += GetRandomValue(20, 60);
		cell.state = CellState::Hinting;
		return;
	}

	case Grid::SpecialFunction::GoUpGrid: {
		if (grid->UpperGrid != nullptr) {
			grid = grid->UpperGrid;
		}
		return;
	}

		cell.state = CellState::Hinting;

	case Grid::SpecialFunction::Heal:
		playerStats.HealToFull();
		break;

	case Grid::SpecialFunction::Mana:
		playerStats.currentPointsToEvo += 4;
		break;

	case Grid::SpecialFunction::Ladder:
		playerStats.currentLevel++;
		playerStats.shoudlNewLevelStart = true;
		break;
	}

	if (cell.val <= 0) {
		cell.state = CellState::Hinting;
		return;
	}

	playerStats.hp -= cell.val;
	cell.state = CellState::pointsNotTaken;
	cell.defeted = false;
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
		if (cell.state != CellState::Hidden) {
			return;
		}

		if (cell.specialFunction == SpecialFunction::None && cell.val <= 0) {
			cell.state = CellState::Hinting;
		} else {
			cell.state = CellState::Revealing;
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
