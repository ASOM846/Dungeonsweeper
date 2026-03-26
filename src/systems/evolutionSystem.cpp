#include "evolutionSystem.hpp"
#include <iostream>

void EvolutionSystem::Update(PlayerStats &player) {

	std::cout << player.evolutions << std::endl;

	if (player.currentPointsToEvo < player.pointsToEvo) {
		return;
	}
	if (!IsKeyPressed(KEY_E)) {
		return;
	}

	player.currentPointsToEvo -= player.pointsToEvo;
	player.evolutionLevel++;

	player.maxHp = 5 + (player.evolutionLevel / 2);
	player.drawHalfHp = (player.evolutionLevel % 2) == 1;

	if (player.evolutionLevel == 0) {
		player.pointsToEvo = 4;
	} else if (player.evolutionLevel == 1) {
		player.pointsToEvo = 5;
	} else {
		player.pointsToEvo = 2 * player.evolutionLevel + 3;
	}

	player.evolutions++;

	player.HealToFull();
}
