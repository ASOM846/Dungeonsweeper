#pragma once
#include "../grid/grid.hpp"
#include <memory.h>
#include <memory>
#include <raylib.h>
#include <vector>

class PassiveItem;

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

	int evolutions = 0;
	int curretTurn = 0;
	bool wasGridClicked = 0;

	bool hasRedSword = true;
	bool hasGreenSword = 1;
	bool hasGoldSword = 1;

	int currentLevel = 1;
	bool shoudlNewLevelStart = false;
	int evolutionLevel = 0;

	bool isInputLocked = false;

	// passiveInventory
	int inventorySize = 3;
	std::vector<std::unique_ptr<PassiveItem>> passiveItems;

	bool PushBackPassiveItems(std::unique_ptr<PassiveItem> item);

	bool isChoosePending = false;
	bool isSelectionPopup = false;

	float GetHp() const { return hp + (hpHalf ? 0.5f : 0.0f); }
	float GetMaxHp() const { return maxHp + (drawHalfHp ? 0.5f : 0.0f); }
	bool IsFullHp() const { return hp == maxHp && hpHalf == drawHalfHp; }

	void Heal(int val_) {
		hp += val_;
		if (hp > maxHp)
			hp = maxHp;
	}

	void HealToFull() {
		hp = maxHp;
		hpHalf = drawHalfHp;
	}

	void EveryFrameReset() { wasGridClicked = false; }
};
