#pragma once

#include "../modules/particles.hpp"
#include "../textureManager.hpp"
#include <raylib.h>
#include <vector>

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
	struct bcgCell {
		short textureNumber;
		short rotation;
	};

	ParticleSystem fire1;
	ParticleSystem fire2;

	float DungeonLength;
	float SweeperLength;

	std::vector<bcgCell> bcgTextures;
};
