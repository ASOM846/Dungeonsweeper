#include "game.hpp"

void Game::Init() {
	InitWindow(800, 600, "My Game");
	SetTargetFPS(60);
	//SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_VSYNC_HINT);
    textureManager.loadAll();
	gridManager.Init();
}

void Game::Run() {
	while (!WindowShouldClose()) {
		Update();
		Draw();
	}
    textureManager.unloadAll();
	CloseWindow();
}

void Game::Update() {
	gridManager.Update();
}

void Game::Draw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	gridManager.Render(&textureManager);

	EndDrawing();
}