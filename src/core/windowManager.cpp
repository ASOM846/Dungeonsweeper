#include "windowManager.hpp"
void WindowManager::Init() {
	InitWindow(1280, 720, "My Application");
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
			SwitchMode(AppMode::Game);
		}
		break;
	case AppMode::Game:
		game.Update();
		if (game.ShouldReturnToMenu()) {
			SwitchMode(AppMode::Menu);
		}
		break;
	}
}

void WindowManager::Render() {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	switch (currentMode) {
		void Run();
	case AppMode::Menu:
		menu.Render(game.GetTextureManager());
		break;
	case AppMode::Game:
		game.Render();
		break;
	}

	EndDrawing();
}

void WindowManager::SwitchMode(AppMode newMode) {
	if (newMode == currentMode)
		return;

	switch (newMode) {
	case AppMode::Menu:
		currentMode = AppMode::Menu;
		menu.Reset();
		break;
	case AppMode::Game:
		currentMode = AppMode::Game;
		game.Reset();
		break;
	}
}
