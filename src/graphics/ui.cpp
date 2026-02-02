#include "ui.hpp"
#include <algorithm>

void UI::RenderUi(const int hp, const int pointsToEvo, const int maxHp) {
	const int barWidth = UI_BAR_WIDTH;
	const int barHeight = GetScreenHeight();

	const int screenW = GetScreenWidth();
	Vector2 position = {static_cast<float>(screenW - barWidth), 0.0f};

	// Panel background
	DrawRectangleGradientV(static_cast<int>(position.x), 0, barWidth, barHeight,
						   Color{28, 24, 20, 255}, Color{18, 16, 14, 255});
	DrawRectangleLinesEx(Rectangle{position.x, 0, static_cast<float>(barWidth), static_cast<float>(barHeight)},
							 2.0f, Color{70, 58, 48, 255});

	// Header
	const int headerH = 90;
	DrawRectangle(static_cast<int>(position.x) + 14, 16, barWidth - 28, headerH - 20,
				  Color{40, 34, 28, 255});
	DrawRectangleLinesEx(Rectangle{position.x + 14.0f, 16.0f, static_cast<float>(barWidth - 28), static_cast<float>(headerH - 20)},
							 2.0f, Color{120, 96, 72, 255});
	DrawText("DUNGEON", static_cast<int>(position.x) + 26, 28, 26, Color{220, 200, 170, 255});
	DrawText("ROGUE", static_cast<int>(position.x) + 26, 56, 18, Color{170, 140, 110, 255});

	const int cardW = barWidth - 28;
	const int cardX = static_cast<int>(position.x) + 14;

	// Avatar card (above HP)
	const int avatarSize = 96;
	const int avatarPad = 12;
	const int avatarCardH = avatarSize + avatarPad * 2 + 24;
	const int avatarY = headerH + 12;
	DrawRectangleRounded(Rectangle{static_cast<float>(cardX), static_cast<float>(avatarY), static_cast<float>(cardW), static_cast<float>(avatarCardH)},
						 0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(Rectangle{static_cast<float>(cardX), static_cast<float>(avatarY), static_cast<float>(cardW), static_cast<float>(avatarCardH)},
							 2.0f, Color{90, 72, 54, 255});
	const int avatarX = cardX + (cardW - avatarSize) / 2;
	const int avatarFrameY = avatarY + 24;
	DrawRectangle(avatarX, avatarFrameY, avatarSize, avatarSize, Color{22, 20, 18, 255});
	DrawRectangleLines(avatarX, avatarFrameY, avatarSize, avatarSize, Color{120, 96, 72, 255});
	DrawText("PLACE", avatarX + 10, avatarFrameY + 34, 16, Color{130, 110, 90, 255});
	DrawText("HOLDER", avatarX + 6, avatarFrameY + 54, 16, Color{130, 110, 90, 255});

	// Hearts card
	const int cardH = 140;
	const int cardY = avatarY + avatarCardH + 16;
	DrawRectangleRounded(Rectangle{static_cast<float>(cardX), static_cast<float>(cardY), static_cast<float>(cardW), static_cast<float>(cardH)},
						 0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(Rectangle{static_cast<float>(cardX), static_cast<float>(cardY), static_cast<float>(cardW), static_cast<float>(cardH)},
							 2.0f, Color{90, 72, 54, 255});
	DrawText("HP", cardX + 12, cardY + 10, 18, Color{200, 170, 140, 255});

	const Texture2D &heartTex = textureManager->get(TextureId::HeartFull);
	const float paddingX = 12.0f;
	const float paddingY = 36.0f;
	const float bottomPadding = 10.0f;
	const float spacing = 6.0f;
	const int heartsPerRow = 5;
	const float rowSpacing = 10.0f;
	const int rows = std::max(1, (maxHp + heartsPerRow - 1) / heartsPerRow);

	const float availableW = static_cast<float>(cardW) - paddingX * 2.0f;
	const float availableH = static_cast<float>(cardH) - paddingY - bottomPadding;
	const float baseScale = 28.0f / static_cast<float>(heartTex.width);
	const float scaleX = (availableW - (heartsPerRow - 1) * spacing) /
							 (heartsPerRow * static_cast<float>(heartTex.width));
	const float scaleY = (availableH - (rows - 1) * rowSpacing) /
							 (rows * static_cast<float>(heartTex.height));
	const float heartScale = std::max(0.1f, std::min(baseScale, std::min(scaleX, scaleY)));
	const float scaledW = heartTex.width * heartScale;
	const float scaledH = heartTex.height * heartScale;

	for (int i = 0; i < maxHp; ++i) {
		const int row = i / heartsPerRow;
		const int col = i % heartsPerRow;
		const float baseX = static_cast<float>(cardX) + paddingX + col * (scaledW + spacing);
		const float baseY = static_cast<float>(cardY) + paddingY + row * (scaledH + rowSpacing);
		Vector2 pos = {baseX, baseY};

		DrawTextureEx(heartTex, pos, 0.0f, heartScale, (i < hp) ? WHITE : GRAY);
	}

	// Evolution bar
	const int evoY = avatarY + avatarCardH + cardH + 32;
	const int evoH = 50;
	DrawRectangleRounded(Rectangle{static_cast<float>(cardX), static_cast<float>(evoY), static_cast<float>(cardW), static_cast<float>(evoH)},
						 0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(Rectangle{static_cast<float>(cardX), static_cast<float>(evoY), static_cast<float>(cardW), static_cast<float>(evoH)},
							 2.0f, Color{90, 72, 54, 255});

	const float progress = (maxHp > 0) ? std::min(1.0f, static_cast<float>(pointsToEvo) / static_cast<float>(maxHp)) : 0.0f;
	const int barX = cardX + 10;
	const int barY = evoY + 22;
	const int barW = cardW - 20;
	const int barH = 14;
	DrawRectangle(barX, barY, barW, barH, Color{22, 20, 18, 255});
	DrawRectangle(barX, barY, static_cast<int>(barW * progress), barH, Color{140, 180, 120, 255});
	DrawRectangleLines(barX, barY, barW, barH, Color{100, 88, 70, 255});
	DrawText("EVOLUTION", cardX + 12, evoY + 6, 16, Color{200, 170, 140, 255});

	// Footer hints
	const int footerY = evoY + evoH + 18;
	DrawText("Loot. Survive. Ascend.", cardX + 8, footerY, 14, Color{130, 110, 90, 255});
}