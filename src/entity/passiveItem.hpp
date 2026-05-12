#pragma once
#include "../grid/grid.hpp"
#include "playerStats.hpp"
#include <raylib.h>
#include <string>

class PassiveItem {
  public:
	enum class PassiveType {
		Regen,
		PointsToEvo,
		UncoverRandom,
		AddChestKey,
	};

	enum SpawnRate {
		Common,
		Medium,
		Rare,
		UltraRare,
	};

	PassiveItem() {}
	virtual ~PassiveItem() {}

	Color color;
	std::string desc;
	std::string shortDesc;

	PassiveType Type;
	SpawnRate Rate;
	int turnsToActivate;
	int turnsCounter;

	[[nodiscard]] virtual PassiveType GetType() const = 0;
	virtual void ApplyEffect(Grid &grid, PlayerStats &playerStats) = 0;
};

/*
 pomysly na pasywki:
co x tur zabiera 1 hp i dodaje punkty ewolucji
*/

class Regen : public PassiveItem {
  public:
	Regen() {
		Rate = Common;
		turnsToActivate = 8;
		turnsCounter = 0;

		color = RED;
		desc = "+4 HP";
		shortDesc = "+4 HP";
	}

	[[nodiscard]] PassiveType GetType() const override {
		return PassiveType::Regen;
	}
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		(void)grid;
		playerStats.Heal(4);
	}
};

class PointsToEvo : public PassiveItem {
  public:
	PointsToEvo() {
		Rate = Common;
		turnsToActivate = 8;
		turnsCounter = 0;

		color = GOLD;
		desc = "+4 EVO";
		shortDesc = "+4 EVO";
	}

	[[nodiscard]] PassiveType GetType() const override {
		return PassiveType::PointsToEvo;
	}
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		(void)grid;
		playerStats.currentPointsToEvo += 4;
	}
};

class UncoverRandomRare : public PassiveItem {
  public:
	UncoverRandomRare() {
		Rate = Common;
		turnsToActivate = 8;
		turnsCounter = 0;

		color = BLUE;
		desc = "uncover random";
		shortDesc = "show random";
	}

	[[nodiscard]] PassiveType GetType() const override {
		return PassiveType::UncoverRandom;
	}
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		(void)playerStats;
		int attempts = 500;

		for (int i = 0; i < attempts; ++i) {
			size_t x = static_cast<size_t>(
				GetRandomValue(0, static_cast<int>(grid.GetWidth() - 1)));
			size_t y = static_cast<size_t>(
				GetRandomValue(0, static_cast<int>(grid.GetHeight() - 1)));

			if (grid.cells[y][x].state == Grid::CellState::Hidden) {
				grid.cells[y][x].state = Grid::CellState::Revealed;
				break;
			}
		}
	}
};

class AddChestKey : public PassiveItem {
  public:
	AddChestKey() {
		Rate = Common;
		turnsToActivate = 25;
		turnsCounter = 0;

		color = MAGENTA;
		desc = "+1 key";
		shortDesc = desc;
	}

	[[nodiscard]] PassiveType GetType() const override {
		return PassiveType::AddChestKey;
	}

	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		(void)grid;

		playerStats.keys++;
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
