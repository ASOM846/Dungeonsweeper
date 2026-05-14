#pragma once

#include "../entity/passiveItem.hpp"
#include "../entity/playerStats.hpp"
#include <memory>
#include <vector>

class ItemChooser {
  public:
	ItemChooser();
	~ItemChooser();

	void Init();
	void Update(PlayerStats &playerStats);
	void Render(PlayerStats &playerStats);

  private:
	using ItemFactory = std::unique_ptr<PassiveItem> (*)();
	using FactoryFn = std::unique_ptr<PassiveItem> (*)();

	static std::unique_ptr<PassiveItem> CreateRegen();
	static std::unique_ptr<PassiveItem> CreatePointsToEvo();
	static std::unique_ptr<PassiveItem> CreateUncoverRandomRare();
	static std::unique_ptr<PassiveItem> CreateAddKey();

	[[nodiscard]] int SpawnRateToWeight(PassiveItem::SpawnRate rate) const;
	void BuildWeightedPool();
	void RollChoices();

	const int numberOfChooses{3};

	int replaceCandidate = -1;
	bool selectingReplacement = false;

	std::vector<ItemFactory> allFactories;
	std::vector<ItemFactory> weightedPool;
	std::vector<std::unique_ptr<PassiveItem>> availableItems;
};
