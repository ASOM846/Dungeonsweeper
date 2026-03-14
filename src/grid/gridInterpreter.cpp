#include "gridInterpreter.hpp"
#include "../entity/passiveItem.hpp"
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
	int x = static_cast<int>((mousePos.x - offset.x) / size);
	int y = static_cast<int>((mousePos.y - offset.y) / size);

	if (x < 0 || y < 0 || x >= static_cast<int>(grid->GetWidth()) ||
		y >= static_cast<int>(grid->GetHeight())) {
		return;
	}

	playerStats.selectedCell = &grid->cells[y][x];

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
		cell.specialFunction == SpecialFunction::ChestKey ||
		cell.specialFunction == SpecialFunction::SwordRed ||
		cell.specialFunction == SpecialFunction::SwordGreen ||
		cell.specialFunction == SpecialFunction::SwordGold ||
		cell.specialFunction == SpecialFunction::MiniDungeonEntry) {
		cell.state = CellState::Revealed;
		return;
	}

	OnRevealedClick(x, y, grid, playerStats, ui);
}

void GridInterpreter::OnRevealedClick(int x, int y, Grid *&grid,
									  PlayerStats &playerStats, UI &ui) {
	auto &cell = grid->cells[y][x];

	if (cell.specialFunction == Grid::SpecialFunction::ItemCell) {
		OnShopCellClick(x, y, *grid, playerStats);
		return;
	}

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

	if (cell.specialFunction == SpecialFunction::MiniDungeonEntry) {
		if (cell.LowerGrid == nullptr) {
			cell.LowerGrid = new Grid(7, 5, *&grid);
			GridGenerator generator;
			generator.InitShirene(*cell.LowerGrid);
		}

		if (!playerStats.hasGoldSword || !playerStats.hasGreenSword ||
			!playerStats.hasRedSword) {
			ui.TriggerMessageBox(
				"You shall not enter the Hole before picking up every sword");
			return;
		} else {
			grid = cell.LowerGrid;
			return;
		}
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

	if (cell.specialFunction == SpecialFunction::SwordRed) {
		playerStats.hasRedSword = true;
		playerStats.currentPointsToEvo += 3;
		return;
	}

	if (cell.specialFunction == SpecialFunction::SwordGreen) {
		playerStats.hasGreenSword = true;
		playerStats.currentPointsToEvo += 3;
		return;
	}

	if (cell.specialFunction == SpecialFunction::SwordGold) {
		playerStats.hasGoldSword = true;
		playerStats.currentPointsToEvo += 3;
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

void GridInterpreter::OnShopCellClick(int x, int y, Grid &grid,
									  PlayerStats &playerStats) {

	auto &cell = grid.cells[y][x];

	if (playerStats.coins >= cell.item.price)
		playerStats.coins -= cell.item.price;
	else
		return;

	playerStats.passiveItems.push_back(std::make_unique<Regen>());

	cell.item.type = Item::ItemType::None;
	cell.defeted = true;
	cell.state = Grid::CellState::Hinting;
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

void GridInterpreter::ApplyItem(int x, int y, Grid &grid,
								PlayerStats &playerStats) {
	if (!playerStats.selectedItem)
		return;
	switch (playerStats.selectedItem->type) {
	case Item::ItemType::Uncover2x2:
		Apply2x2(x, y, grid);
		break;
	}
}

void GridInterpreter::Apply2x2(int x, int y, Grid &grid) {
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

	for (int dx = 0; dx < 2; ++dx) {
		for (int dy = 0; dy < 2; ++dy) {
			revealIfValid(x + dx, y + dy);
		}
	}
}
