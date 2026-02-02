#include "game.hpp"

void Game::Init() {
	InitWindow(800, 600, "My Game");
	SetTargetFPS(60);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_VSYNC_HINT);
    textureManager.loadAll();
	gridManager.Init();
	ui.Init(&textureManager);
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
	gridManager.Update(hp, maxHp, pointsToEvo);
}

void Game::Draw() {
	BeginDrawing();
	ClearBackground(RAYWHITE);

	gridManager.Render(&textureManager, hp, pointsToEvo, maxHp);
	ui.RenderUi(hp, pointsToEvo, maxHp);

	EndDrawing();
}