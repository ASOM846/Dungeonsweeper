#pragma once

#include "../game.hpp"
#include "../gameMode.hpp"
#include "../graphics/gameTypeMenu.hpp"
#include "../graphics/menu.hpp"
#include "../graphics/menuBackground.hpp"
#include <raylib.h>

enum class AppMode {
	Menu,
	SelectingGameType,
	Game,
	Settings,
};

class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();
	void Run();

  private:
	void Update();
	void Render();

	void SwitchMode(AppMode newMode);

	Game game;
	Menu menu;
	GameTypeMenu gameTypeMenu;
	MenuBackground background;

	AppMode currentMode;

	GameConfig currentConfig;

	int width;
	int height;
};
