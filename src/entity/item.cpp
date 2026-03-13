#include "item.hpp"
#include "../grid/grid.hpp"
#include "playerStats.hpp"

void Item::ApplyEffect(Grid &grid, PlayerStats &playerStats) {
	switch (type) {
	case ItemType::HpUp:
		playerStats.HealToFull();
		break;
	case ItemType::EvolutionUp:
		playerStats.pointsToEvo += 6;
		break;
	}
}
