#pragma once
#include <memory.h>
#include <memory>
#include <raylib.h>
#include <vector>

class PassiveItem;

struct PlayerStats {
	int score = 0;

	void addScore(int val) { score += val; }

	int hp = 5;
	bool hpHalf = false;
	int maxHp = 5;
	bool drawHalfHp = false;
	int pointsToEvo = 4;
	int currentPointsToEvo = 0;
	int keys = 0;

	int coins = 0;

	bool gameWon = 0;

	int evolutions = 0;
	int curretTurn = 0;
	bool wasGridClicked = 0;

	float timer = 0.0F;

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

	float GetHp() const {
		return static_cast<float>(hp) + (hpHalf ? 0.5f : 0.0f);
	}

	float GetMaxHp() const {
		return static_cast<float>(maxHp) + (drawHalfHp ? 0.5f : 0.0f);
	}

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

	void Update() {
		if (IsKeyPressed(KEY_E)) {
			TryEvolve();
		}

		wasGridClicked = false;
	}

	void TryEvolve() {
		if (currentPointsToEvo < pointsToEvo) {
			return;
		}

		currentPointsToEvo -= pointsToEvo;
		evolutionLevel++;

		maxHp = 5 + (evolutionLevel / 2);
		drawHalfHp = (evolutionLevel % 2) == 1;

		if (evolutionLevel == 0) {
			pointsToEvo = 4;
		} else if (evolutionLevel == 1) {
			pointsToEvo = 5;
		} else {
			pointsToEvo = 2 * evolutionLevel + 3;
		}

		evolutions++;

		HealToFull();
	}
};
