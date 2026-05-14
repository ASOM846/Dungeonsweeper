#pragma once

#include "../modules/particles.hpp"
#include "../textureManager.hpp"
#include <raylib.h>

class MenuBackground {
  public:
	MenuBackground() = default;
	~MenuBackground() = default;

	void UpdatePositions();

	void Update();
	void Render(const TextureManager &textureManager);

	void Init();

	void RenderBcg(const TextureManager &textureManager);

  private:
	ParticleSystem fire1;
	ParticleSystem fire2;

	float DungeonLength;
	float SweeperLength;
};
