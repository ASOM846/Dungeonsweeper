#include "playerStats.hpp"
#include "passiveItem.hpp"

bool PlayerStats::PushBackPassiveItems(std::unique_ptr<PassiveItem> item) {
	if (passiveItems.size() < static_cast<size_t>(inventorySize)) {
		passiveItems.push_back(std::move(item));
		return true;
	}

	return false;
}
