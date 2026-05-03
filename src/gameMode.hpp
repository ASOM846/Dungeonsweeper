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

enum class GameType {
	Normal,
	Challenge
};

struct GameConfig {
	Difficulty difficulty;
	GameType type;
};

struct GameTime {
	float starting;
	float onClick;
	float cap;
};

inline GameTime GetGameTime(Difficulty diff) {
	switch (diff) {
	case Difficulty::Easy:
		return {180.0F, 8.0F, 300.0F};
	case Difficulty::Medium:
		return {120.0F, 6.0F, 240.0F};
	case Difficulty::Hard:
		return {90.0F, 5.0F, 210.0F};
	default:
		return {180.0F, 8.0F, 300.0F};
	}
}

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
		return {5, 6, 3, 3, 0, 0};
	case Difficulty::Medium:
		return {4, 5, 3, 3, 1, 1};
	case Difficulty::Hard:
		return {3, 4, 2, 2, 3, 2};
	default:
		return {5, 6, 2, 2, 1, 0};
	}
}
