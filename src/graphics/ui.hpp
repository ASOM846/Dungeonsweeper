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

	void RenderUi(const PlayerStats &playerStats, const GameState &gameState);
	void RenderLose(const PlayerStats &playerStats);

	void TriggerMessageBox(const char *message);
	void CloseMessageBox();

	bool IsMessageBoxOpen() const { return messageBoxOpen; }
	const int GetBarWidth() const { return UI_BAR_WIDTH; }
	constexpr static int UI_BAR_WIDTH = 200;

  private:
	void RenderMessageBox();

	TextureManager *textureManager;
	bool messageBoxOpen = false;
	std::string messageBoxText;
};
