#include "windowManager.hpp"
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
static void WebMainLoop(void *arg) {
	static_cast<WindowManager *>(arg)->UpdateAndRender();
}
#endif

void WindowManager::Init() {
	// InitWindow(1920, 1080, "Dungeonsweeper");
	InitWindow(1080, 720, "Dungeonsweeper");
	SetTargetFPS(60);

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);
	// SetWindowState(FLAG_FULLSCREEN_MODE);

	currentMode = AppMode::Menu;

	menu.Init();
	game.Init();
}

void WindowManager::Run() {
#if defined(PLATFORM_WEB)
	// On web, emscripten_set_main_loop_arg takes over the event loop.
	// The browser manages resource cleanup; CloseWindow() is not called here.
	emscripten_set_main_loop_arg(WebMainLoop, this, 0, 1);
#else
	while (!WindowShouldClose()) {
		UpdateAndRender();
	}

	CloseWindow();
#endif
}

void WindowManager::UpdateAndRender() {
	Update();
	Render();
}

void WindowManager::Update() {
	switch (currentMode) {
	case AppMode::Menu:
		menu.Update();
		if (menu.GetCurrentState() == MenuState::ClassicGameShoudlStart) {
			game.InitClassicGame();
			SwitchMode(AppMode::Game);
		}
		if (menu.GetCurrentState() == MenuState::EndlessGameShouldStart) {
			game.InitEndlessGame();
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
	ClearBackground(BLACK);

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
