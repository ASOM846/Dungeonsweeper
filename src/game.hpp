#pragma once
#include "grid/gridManager.hpp"
#include "textureManager.hpp"
#include "graphics/ui.hpp"
#include <raylib.h>

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();

	void Run();

	void Update();
	void Draw();

  private:
	int hp = 4;
	int maxHp = 4;
	int pointsToEvo = 0;

	GridManager gridManager;
	TextureManager textureManager;
	UI ui;
};