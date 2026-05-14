#include "windowManager.hpp"
#include "../graphics/startAnim.hpp"
#include <raylib.h>

void WindowManager::Init() {
	width = 1280;
	height = width / 16 * 10;

	// #if defined(__ANDROID__)
	// 	width = height = 0;
	// #endif

	InitWindow(width, height, "Dungeonsweeper");
	SetTargetFPS(60);

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);
	// SetWindowState(FLAG_FULLSCREEN_MODE);

	HideCursor();

	currentMode = AppMode::Menu;

	game.Init();
	menu.Init(game.GetTextureManager());
	background.Init();
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
			background.Update();
			menu.Update();
			if (menu.GetCurrentState() == MenuState::ClassicGameShoudlStart) {
				currentConfig.difficulty = menu.GetSelectedDifficulty();
				currentConfig.mode = menu.GetSelectedMode();
				SwitchMode(AppMode::SelectingGameType);
			}
			break;
		case AppMode::SelectingGameType:
			background.Update();
			gameTypeMenu.Update();
			if (gameTypeMenu.IsPicked()) {
				currentConfig.type = gameTypeMenu.GetSelectedType();
				game.RunGame(currentConfig);
				SwitchMode(AppMode::Game);
			}
			if (gameTypeMenu.ShoudlReturnToMenu()) {
				SwitchMode(AppMode::Menu);
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
			background.Render(game.GetTextureManager());
			menu.Render(game.GetTextureManager());
			break;
		case AppMode::SelectingGameType:
			background.Render(game.GetTextureManager());
			gameTypeMenu.Render();
			break;
		case AppMode::Game:
			game.Render();
			break;
		case AppMode::Settings:
			break;
		}
	}
	mouseManager.Render(game.GetTextureManager());

	DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 40);
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
	case AppMode::SelectingGameType:
		currentMode = AppMode::SelectingGameType;
		gameTypeMenu.Reset();
		gameTypeMenu.Init(menu.GetSelectedDifficulty());
		break;
	case AppMode::Game:
		currentMode = AppMode::Game;
		game.Reset();
		break;
	case AppMode::Settings:
		break;
	}
}
