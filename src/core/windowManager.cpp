#include "windowManager.hpp"
#include "../graphics/startAnim.hpp"
#include <raylib.h>

void WindowManager::Init() {
	// InitWindow(1920, 1080, "Dungeonsweeper");

	width = 1280;
	height = width / 16 * 10;

#if defined(__ANDROID__)
	width = height = 0;
#endif

	InitWindow(width, height, "Dungeonsweeper");
	SetTargetFPS(60);

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);
	// SetWindowState(FLAG_FULLSCREEN_MODE);

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
	if (!IsAnimFinished()) {
		UpdateAnim();

	} else {
		switch (currentMode) {
		case AppMode::Menu:
			menu.Update();
			if (menu.GetCurrentState() == MenuState::ClassicGameShoudlStart) {
				game.RunGame(menu.GetSelectedDifficulty());
				SwitchMode(AppMode::Game);
			}
			break;
		case AppMode::Game:
			game.Update();
			if (game.ShouldReturnToMenu()) {
				SwitchMode(AppMode::Menu);
			}
			break;
		case AppMode::Settings:
			break;
		}
	}
}

void WindowManager::Render() {
	BeginDrawing();
	ClearBackground(BLACK);

	if (!IsAnimFinished()) {
		RenderAnim();
	} else {
		switch (currentMode) {
			void Run();
		case AppMode::Menu:
			menu.Render(game.GetTextureManager());
			break;
		case AppMode::Game:
			game.Render();

#if defined(__ANDROID__)
			DrawText("ANDROID TEST", 10, 10, 50, RED);
#endif

			break;
		case AppMode::Settings:
			break;
		}
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
	case AppMode::Settings:
		break;
	}
}
