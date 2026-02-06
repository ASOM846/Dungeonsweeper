#pragma once

#include "../grid/grid.hpp"
#include "../textureManager.hpp"
#include "button.hpp"
#include <raylib.h>

class Menu {
  public:
	Menu() : IsStartGame(false) {}
	~Menu() = default;

	void Init();
	void Update();
	void Render(TextureManager &textureManager);

	bool IsStartGamePressed() const;

  private:
	void UpdateButtonsPosition();
	void RenderBackground(const TextureManager &TextureManager) const;

  private:
	NewButton startGameButton;
	bool IsStartGame;

	const int bigButtonHeight = 52;
	const int bigButtonWidth = 260;
};
