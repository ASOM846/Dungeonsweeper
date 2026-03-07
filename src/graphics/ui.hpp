#pragma once
#include "../entity/playerStats.hpp"
#include "../textureManager.hpp"
#include <raylib.h>
#include <raymath.h>
#include <string>

enum class GameState;

class UI {
  public:
	UI() = default;
	~UI() = default;

	void Init(TextureManager *tm) { textureManager = tm; }

	void UpdateInventoryClick(PlayerStats &playerStats);
	void RenderUi(const PlayerStats &playerStats, const GameState &gameState);
	void RenderLose(const PlayerStats &playerStats);

	void TriggerMessageBox(const char *message);
	void CloseMessageBox();

	bool IsMessageBoxOpen() const { return messageBoxOpen; }
	const int GetBarWidth() const { return UI_BAR_WIDTH; }
	constexpr static int UI_BAR_WIDTH = 200;

  private:
	int gridCols = 3;
	int gridRows = 3;
	int totalSlots = 9;

	float slotSize = 42.0f;
	float slotGap = 8.0f;
	float gridW = gridCols * slotSize + (gridCols - 1) * slotGap;
	float gridH = gridRows * slotSize + (gridRows - 1) * slotGap;

	float gridX;

	float gridY;

	void RenderMessageBox();

	void RenderInventory(const PlayerStats &playerStats);

	TextureManager *textureManager;
	bool messageBoxOpen = false;
	std::string messageBoxText;
};
