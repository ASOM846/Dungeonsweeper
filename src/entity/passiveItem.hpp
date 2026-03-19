#pragma once
#include "../grid/grid.hpp"
#include "playerStats.hpp"
#include <raylib.h>

class PassiveItem {
  public: // lub protected:
	enum class PassiveType {
		Regen,
		PointsToEvo,
		UncoverRandom,
	};

	enum SpawnRate {
		Common,
		Medium,
		Rare,
		UltraRare,
	};

	PassiveItem() {}
	virtual ~PassiveItem() {}

	PassiveType Type;
	SpawnRate Rate;
	int turnsToActivate;
	int turnsCounter;

	virtual PassiveType GetType() const = 0;
	virtual void ApplyEffect(Grid &grid, PlayerStats &playerStats) = 0;
};

class Regen : public PassiveItem {
  public:
	Regen() {
		Rate = Medium;
		turnsToActivate = 10;
		turnsCounter = 0;
	}

	PassiveType GetType() const override { return PassiveType::Regen; }
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		playerStats.Heal(2);
	}
};

class PointsToEvo : public PassiveItem {
  public:
	PointsToEvo() {
		Rate = Medium;
		turnsToActivate = 10;
		turnsCounter = 0;
	}

	PassiveType GetType() const override { return PassiveType::PointsToEvo; }
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		playerStats.currentPointsToEvo += 4;
	}
};

class UncoverRandomRare : public PassiveItem {
  public:
	UncoverRandomRare() {
		Rate = Rare;
		turnsToActivate = 8;
		turnsCounter = 0;
	}

	PassiveType GetType() const override { return PassiveType::UncoverRandom; }
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		int attempts = 500;

		for (int i = 0; i < attempts; ++i) {
			int x = GetRandomValue(0, grid.GetWidth() - 1);
			int y = GetRandomValue(0, grid.GetHeight() - 1);

			if (grid.cells[y][x].state == Grid::CellState::Hidden) {
				grid.cells[y][x].state = Grid::CellState::Revealed;
				break;
			}
		}
	}
};

class PassiveItemManager {
  public:
	void Update(Grid &grid, PlayerStats &playerStats) {
		for (auto &itemPtr : playerStats.passiveItems) {
			if (itemPtr) {
				if (playerStats.wasGridClicked)
					itemPtr->turnsCounter++;
				if (itemPtr->turnsCounter >= itemPtr->turnsToActivate) {
					itemPtr->ApplyEffect(grid, playerStats);
					itemPtr->turnsCounter = 0;
				}
			}
		}
	}
};
