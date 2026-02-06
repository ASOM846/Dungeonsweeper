#include "ui.hpp"
#include <algorithm>

void UI::RenderUi(const PlayerStats &playerStats, const bool isLose) const {
	const int barWidth = UI_BAR_WIDTH;
	const int barHeight = GetScreenHeight();

	const int screenW = GetScreenWidth();
	Vector2 position = {static_cast<float>(screenW - barWidth), 0.0f};

	// Panel background
	DrawRectangleGradientV(static_cast<int>(position.x), 0, barWidth, barHeight,
						   Color{28, 24, 20, 255}, Color{18, 16, 14, 255});
	DrawRectangleLinesEx(Rectangle{position.x, 0, static_cast<float>(barWidth),
								   static_cast<float>(barHeight)},
						 2.0f, Color{70, 58, 48, 255});

	// Header
	const int headerH = 90;
	DrawRectangle(static_cast<int>(position.x) + 14, 16, barWidth - 28,
				  headerH - 20, Color{40, 34, 28, 255});
	DrawRectangleLinesEx(Rectangle{position.x + 14.0f, 16.0f,
								   static_cast<float>(barWidth - 28),
								   static_cast<float>(headerH - 20)},
						 2.0f, Color{120, 96, 72, 255});
	DrawText("DUNGEON", static_cast<int>(position.x) + 26, 28, 26,
			 Color{220, 200, 170, 255});
	DrawText("SWEEPER", static_cast<int>(position.x) + 26, 56, 18,
			 Color{170, 140, 110, 255});

	const int cardW = barWidth - 28;
	const int cardX = static_cast<int>(position.x) + 14;

	// Avatar card (above HP)
	const int avatarSize = 96;
	const int avatarPad = 12;
	const int avatarCardH = avatarSize + avatarPad * 2 + 24;
	const int avatarY = headerH + 12;

	DrawRectangleRounded(
		Rectangle{static_cast<float>(cardX), static_cast<float>(avatarY),
				  static_cast<float>(cardW), static_cast<float>(avatarCardH)},
		0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(
		Rectangle{static_cast<float>(cardX), static_cast<float>(avatarY),
				  static_cast<float>(cardW), static_cast<float>(avatarCardH)},
		2.0f, Color{90, 72, 54, 255});
	const int avatarX = cardX + (cardW - avatarSize) / 2;
	const int avatarFrameY = avatarY + 24;

	const float scale = 3.0f;
	Texture2D playerTex = textureManager->get(TextureId::Player);

	// Wycentrowanie tekstury gracza w kwadracie ramki (avatarFrameY)
	const int frameX = avatarX;
	const int frameY = avatarFrameY;
	const int frameW = avatarSize;
	const int frameH = avatarSize;

	float texW = playerTex.width * scale;
	float texH = playerTex.height * scale;
	float texX = frameX + (frameW - texW) * 0.5f;
	float texY = frameY + (frameH - texH) * 0.5f;

	DrawRectangle(frameX, frameY, frameW, frameH, Color{22, 20, 18, 255});
	DrawRectangleLines(frameX, frameY, frameW, frameH, Color{120, 96, 72, 255});
	DrawTextureEx(playerTex, (Vector2){texX, texY}, 0.0f, scale, WHITE);

	// Hearts card
	const int cardH = 140;
	const int cardY = avatarY + avatarCardH + 16;
	DrawRectangleRounded(
		Rectangle{static_cast<float>(cardX), static_cast<float>(cardY),
				  static_cast<float>(cardW), static_cast<float>(cardH)},
		0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(
		Rectangle{static_cast<float>(cardX), static_cast<float>(cardY),
				  static_cast<float>(cardW), static_cast<float>(cardH)},
		2.0f, Color{90, 72, 54, 255});
	DrawText("HP", cardX + 12, cardY + 10, 18, Color{200, 170, 140, 255});

	const Texture2D &heartTex = textureManager->get(TextureId::HeartFull);
	const float paddingX = 12.0f;
	const float paddingY = 36.0f;
	const float bottomPadding = 10.0f;
	const float spacing = 6.0f;
	const int heartsPerRow = 5;
	const float rowSpacing = 10.0f;
	const int rows =
		std::max(1, (playerStats.maxHp + heartsPerRow - 1) / heartsPerRow);

	const float availableW = static_cast<float>(cardW) - paddingX * 2.0f;
	const float availableH =
		static_cast<float>(cardH) - paddingY - bottomPadding;
	const float baseScale = 28.0f / static_cast<float>(heartTex.width);
	const float scaleX = (availableW - (heartsPerRow - 1) * spacing) /
						 (heartsPerRow * static_cast<float>(heartTex.width));
	const float scaleY = (availableH - (rows - 1) * rowSpacing) /
						 (rows * static_cast<float>(heartTex.height));
	const float heartScale =
		std::max(0.1f, std::min(baseScale, std::min(scaleX, scaleY)));
	const float scaledW = heartTex.width * heartScale;
	const float scaledH = heartTex.height * heartScale;

	for (int i = 0; i < playerStats.maxHp; ++i) {
		const int row = i / heartsPerRow;
		const int col = i % heartsPerRow;
		const float baseX =
			static_cast<float>(cardX) + paddingX + col * (scaledW + spacing);
		const float baseY =
			static_cast<float>(cardY) + paddingY + row * (scaledH + rowSpacing);
		Vector2 pos = {baseX, baseY};

		DrawTextureEx(heartTex, pos, 0.0f, heartScale,
					  (i < playerStats.hp) ? WHITE : GRAY);
	}

	// Evolution bar
	const int evoY = avatarY + avatarCardH + cardH + 32;
	const int evoH = 50;
	DrawRectangleRounded(
		Rectangle{static_cast<float>(cardX), static_cast<float>(evoY),
				  static_cast<float>(cardW), static_cast<float>(evoH)},
		0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(
		Rectangle{static_cast<float>(cardX), static_cast<float>(evoY),
				  static_cast<float>(cardW), static_cast<float>(evoH)},
		2.0f, Color{90, 72, 54, 255});

	const float progress =
		(playerStats.pointsToEvo > 0)
			? std::min(1.0f,
					   static_cast<float>(playerStats.currentPointsToEvo) /
						   static_cast<float>(playerStats.pointsToEvo))
			: 0.0f;
	const int barX = cardX + 10;
	const int barY = evoY + 22;
	const int barW = cardW - 20;
	const int barH = 14;
	DrawRectangle(barX, barY, barW, barH, Color{22, 20, 18, 255});
	DrawRectangle(barX, barY, static_cast<int>(barW * progress), barH,
				  Color{140, 180, 120, 255});
	DrawRectangleLines(barX, barY, barW, barH, Color{100, 88, 70, 255});
	if (playerStats.pointsToEvo > 0) {
		const int labelFontSize = 16;
		const int labelPadX = 12;
		const int labelY = evoY + 6;

		DrawText("Evolution", cardX + labelPadX, labelY, labelFontSize,
				 Color{200, 170, 140, 255});

		const char *ratioText = TextFormat(
			"%d/%d", playerStats.currentPointsToEvo, playerStats.pointsToEvo);
		const int ratioW = MeasureText(ratioText, labelFontSize);
		const int ratioX = cardX + cardW - labelPadX - ratioW;
		DrawText(ratioText, ratioX, labelY, labelFontSize,
				 Color{200, 170, 140, 255});
	}

	const int evoY2 = avatarY + avatarCardH + cardH + evoH + 48;
	DrawRectangleRounded(
		Rectangle{static_cast<float>(cardX), static_cast<float>(evoY2),
				  static_cast<float>(cardW), static_cast<float>(avatarFrameY)},
		0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(
		Rectangle{static_cast<float>(cardX), static_cast<float>(evoY2),
				  static_cast<float>(cardW), static_cast<float>(avatarFrameY)},
		2.0f, Color{90, 72, 54, 255});

	// Footer hints
	const int footerY = evoY + evoH + 32;

	std::string hintText = "Playing...";
	if (isLose) {
		hintText = "You lose! Press anywhere to restart.";
	}
	DrawText(hintText.c_str(), cardX + 12, footerY, 16,
			 Color{200, 170, 140, 255});
}
void RenderLose(const PlayerStats &playerStats) {}
