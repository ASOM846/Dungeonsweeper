#pragma once
#include "../entity/playerStats.hpp"
#include <raylib.h>

class EvolutionSystem {
  public:
	EvolutionSystem() = default;
	~EvolutionSystem() = default;

	void Update(PlayerStats &player);

  private:
};
