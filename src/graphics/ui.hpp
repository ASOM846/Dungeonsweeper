#pragma once
#include "../entity/playerStats.hpp"
#include "../textureManager.hpp"
#include <raylib.h>
#include <raymath.h>

class UI {
  public:
	UI() = default;
	~UI() = default;

	void Init(TextureManager *tm) { textureManager = tm; }

	void RenderUi(const PlayerStats &playerStats) const;

	const int GetBarWidth() const { return UI_BAR_WIDTH; }
	constexpr static int UI_BAR_WIDTH = 200;

  private:
	TextureManager *textureManager;
};
