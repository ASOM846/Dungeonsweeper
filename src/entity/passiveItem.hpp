#pragma once
#include "../grid/grid.hpp"
#include "playerStats.hpp"

class PassiveItem {
  public: // lub protected:
	enum class PassiveType {
		Regen
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
		Rate = Common;
		turnsToActivate = 5;
		turnsCounter = 0;
	}

	PassiveType GetType() const override { return PassiveType::Regen; }
	void ApplyEffect(Grid &grid, PlayerStats &playerStats) override {
		playerStats.Heal(2);
	}
};

class PassiveItemManager {
  public:
	void Update(Grid &grid, PlayerStats &playerStats) {
		for (auto &itemPtr : playerStats.passiveItems) {
			if (itemPtr) {
				itemPtr->turnsCounter++;
				if (itemPtr->turnsCounter >= itemPtr->turnsToActivate) {
					itemPtr->ApplyEffect(grid, playerStats);
					itemPtr->turnsCounter = 0;
				}
			}
		}
	}
};
