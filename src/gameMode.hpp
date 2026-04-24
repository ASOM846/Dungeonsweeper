#pragma once

enum class GameMode {
	Classic,
	Endless,
};

enum class Difficulty {
	Easy,
	Medium,
	Hard
};

struct GridConfig {
	int numberOfHearts;
	int numberOfMana;
	int numberOfKeys;
	int numberOfChests;
	int numberOfNecromancers;
	int numberOfMysteryEnemies;
};

inline GridConfig GetGridConfig(Difficulty diff) {
	switch (diff) {
	case Difficulty::Easy:
		return {6, 6, 3, 3, 0, 0};
	case Difficulty::Medium:
		return {5, 6, 3, 3, 1, 1};
	case Difficulty::Hard:
		return {3, 4, 2, 2, 3, 2};
	default:
		return {5, 6, 2, 2, 1, 0};
	}
}
