#pragma once

#include "../game.hpp"
#include "../graphics/menu.hpp"
#include <raylib.h>

enum class AppMode {
	Menu,
	Game,
	Settings,
};

class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();
	void Run();
	void UpdateAndRender();

  private:
	void Update();
	void Render();

	void SwitchMode(AppMode newMode);

	Game game;
	Menu menu;

	AppMode currentMode;
};
