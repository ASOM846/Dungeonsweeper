#include "evolutionSystem.hpp"

void EvolutionSystem::Update(PlayerStats &player) {
	if (player.pointsToEvo > 0 &&
		player.currentPointsToEvo >= player.pointsToEvo && IsKeyDown(KEY_E)) {
		if (player.maxHp < 10) {
			player.pointsToEvo += 1;
		}
		if (player.maxHp >= 10) {
			player.pointsToEvo += 2;
		}
		player.currentPointsToEvo -= player.maxHp;
		player.maxHp += 1;
		player.hp = player.maxHp;
	}
}
