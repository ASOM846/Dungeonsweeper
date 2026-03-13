#pragma once
#include "../grid/grid.hpp"
#include "item.hpp"
#include <raylib.h>
#include <vector>

struct PlayerStats {
	int score = 0;
	int hp = 5;
	bool hpHalf = false;
	int maxHp = 5;
	bool drawHalfHp = false;
	int pointsToEvo = 4;
	int currentPointsToEvo = 0;
	int keys = 0;

	int coins = 0;

	bool hasRedSword = true;
	bool hasGreenSword = 1;
	bool hasGoldSword = 1;

	int currentLevel = 1;
	bool shoudlNewLevelStart = false;
	int evolutionLevel = 0;

	bool isInputLocked = false;

	// inventory
	int inventorySize = 3;
	std::vector<Item> inventory;

	Item *selectedItem;

	Grid::Cell *selectedCell;

	float GetHp() const { return hp + (hpHalf ? 0.5f : 0.0f); }
	float GetMaxHp() const { return maxHp + (drawHalfHp ? 0.5f : 0.0f); }
	bool IsFullHp() const { return hp == maxHp && hpHalf == drawHalfHp; }
	void HealToFull() {
		hp = maxHp;
		hpHalf = drawHalfHp;
	}
};
