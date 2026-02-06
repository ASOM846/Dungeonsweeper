#include "windowManager.hpp"
void WindowManager::Init() {
	InitWindow(800, 600, "My Application");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);

	currentMode = AppMode::Menu;

	menu.Init();
	game.Init();
}

void WindowManager::Run() {
	while (!WindowShouldClose()) {
		Update();
		Render();
	}

	CloseWindow();
}

void WindowManager::Update() {
	switch (currentMode) {
	case AppMode::Menu:
		menu.Update();
		if (menu.IsStartGamePressed()) {
			currentMode = AppMode::Game;
		}
		break;
	case AppMode::Game:
		game.Update();
		break;
	}
}

void WindowManager::Render() {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	switch (currentMode) {
	case AppMode::Menu:
		menu.Render(game.GetTextureManager());
		break;
	case AppMode::Game:
		game.Render();
		break;
	}

	EndDrawing();
}
