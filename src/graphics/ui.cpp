#include "ui.hpp"
#include "../game.hpp"
#include "button.hpp"
#include <algorithm>

namespace {
void DrawWrappedText(const Font &font, const std::string &text,
					 Rectangle bounds, int fontSize, float spacing,
					 Color tint) {
	if (text.empty() || bounds.width <= 0.0f || bounds.height <= 0.0f) {
		return;
	}

	const float lineHeight = static_cast<float>(fontSize) + spacing;
	float cursorY = bounds.y;

	size_t start = 0;
	while (start < text.size() &&
		   cursorY + lineHeight <= bounds.y + bounds.height) {
		// Skip leading spaces/newlines
		while (start < text.size() &&
			   (text[start] == ' ' || text[start] == '\n' ||
				text[start] == '\r' || text[start] == '\t')) {
			if (text[start] == '\n') {
				cursorY += lineHeight;
				if (cursorY + lineHeight > bounds.y + bounds.height) {
					return;
				}
			}
			++start;
		}
		if (start >= text.size()) {
			break;
		}

		size_t end = start;
		size_t lastBreak = start;
		bool foundAny = false;

		// Find the longest substring that fits in bounds.width
		while (end < text.size() && text[end] != '\n' && text[end] != '\r') {
			if (text[end] == ' ') {
				lastBreak = end;
			}
			std::string candidate = text.substr(start, end - start + 1);
			Vector2 size = MeasureTextEx(font, candidate.c_str(),
										 static_cast<float>(fontSize), spacing);
			if (size.x > bounds.width) {
				break;
			}
			foundAny = true;
			++end;
		}

		size_t lineEnd = end;
		if (!foundAny) {
			// Force at least one character to render
			lineEnd = std::min(start + 1, text.size());
		} else if (lineEnd < text.size() && text[lineEnd] != '\n' &&
				   lastBreak > start) {
			// Prefer breaking at last space when we overflowed
			lineEnd = lastBreak;
		}

		std::string line = text.substr(start, lineEnd - start);
		// Trim trailing spaces
		while (!line.empty() && line.back() == ' ') {
			line.pop_back();
		}

		DrawTextEx(font, line.c_str(), Vector2{bounds.x, cursorY},
				   static_cast<float>(fontSize), spacing, tint);
		cursorY += lineHeight;

		// Advance start
		start = lineEnd;
		while (start < text.size() && text[start] == ' ') {
			++start;
		}
		if (start < text.size() &&
			(text[start] == '\n' || text[start] == '\r')) {
			++start;
		}
	}
}

void DrawTriColorTriangleIndicator(Rectangle bounds, bool redOn, bool greenOn,
								   bool goldOn) {
	if (bounds.width <= 1.0f || bounds.height <= 1.0f) {
		return;
	}

	// Big triangle vertices (A top, B bottom-left, C bottom-right)
	Vector2 a{bounds.x + bounds.width * 0.5f, bounds.y};
	Vector2 b{bounds.x, bounds.y + bounds.height};
	Vector2 c{bounds.x + bounds.width, bounds.y + bounds.height};

	// Midpoints (used to form a "triforce" layout: 3 triangles + empty center)
	Vector2 ab{(a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f};
	Vector2 ac{(a.x + c.x) * 0.5f, (a.y + c.y) * 0.5f};
	Vector2 bc{(b.x + c.x) * 0.5f, (b.y + c.y) * 0.5f};

	const Color off = Color{50, 44, 38, 255};
	const Color colRed = redOn ? Color{185, 70, 55, 255} : off;
	const Color colGreen = greenOn ? Color{70, 160, 95, 255} : off;
	const Color colGold = goldOn ? Color{210, 175, 70, 255} : off;
	const Color line = Color{120, 96, 72, 255};

	// Top = gold, bottom-left = red, bottom-right = green
	DrawTriangle(a, ab, ac, colGold);
	DrawTriangle(ab, b, bc, colRed);
	DrawTriangle(ac, bc, c, colGreen);

	// Outline the three sub-triangles (keeps the "hole" visible)
	DrawTriangleLines(a, ab, ac, line);
	DrawTriangleLines(ab, b, bc, line);
	DrawTriangleLines(ac, bc, c, line);
	DrawTriangleLines(a, b, c, line);
}
} // namespace

void UI::RenderUi(const PlayerStats &playerStats, const GameState &gameState) {
	const int barWidth = UI_BAR_WIDTH;
	const int barHeight = GetScreenHeight();

	const int screenW = GetScreenWidth();
	Vector2 position = {static_cast<float>(screenW - barWidth), 0.0f};

	if (IsKeyPressed(KEY_TAB)) {
		TriggerMessageBox(
			"Use WASD or arrow keys to move, left click to reveal a cell, "
			"right click to flag/unflag a cell. Reach the ladder to go to the "
			"next level. Good luck!");
	}
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
	const int maxHeartSlots =
		playerStats.maxHp + (playerStats.drawHalfHp ? 1 : 0);
	const int rows =
		std::max(1, (maxHeartSlots + heartsPerRow - 1) / heartsPerRow);

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

	// Optional half-heart (drawn as left half of the full-heart texture)
	if (playerStats.drawHalfHp) {
		const int i = playerStats.maxHp;
		const int row = i / heartsPerRow;
		const int col = i % heartsPerRow;
		const float baseX =
			static_cast<float>(cardX) + paddingX + col * (scaledW + spacing);
		const float baseY =
			static_cast<float>(cardY) + paddingY + row * (scaledH + rowSpacing);

		Rectangle src{0.0f, 0.0f, heartTex.width * 0.5f,
					  static_cast<float>(heartTex.height)};
		Rectangle dst{baseX, baseY, scaledW * 0.5f, scaledH};
		Vector2 origin{0.0f, 0.0f};

		const Color tint = playerStats.hpHalf ? WHITE : GRAY;
		DrawTexturePro(heartTex, src, dst, origin, 0.0f, tint);
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

	const float triPadTop = 18.0f;
	const float triW = std::min(120.0f, static_cast<float>(cardW) - 20.0f);
	const float triH = triW * 0.8660254f;
	const float triX =
		static_cast<float>(cardX) + (static_cast<float>(cardW) - triW) * 0.5f;
	const float triY = static_cast<float>(evoY + evoH) + triPadTop;
	DrawTriColorTriangleIndicator(
		Rectangle{triX, triY, triW, triH}, playerStats.hasRedSword,
		playerStats.hasGreenSword, playerStats.hasGoldSword);

	// Coins card
	const int coinsCardH = 60;
	const int coinsCardY = static_cast<int>(evoY + evoH + triH + 48);
	DrawRectangleRounded(
		Rectangle{static_cast<float>(cardX), static_cast<float>(coinsCardY),
				  static_cast<float>(cardW), static_cast<float>(coinsCardH)},
		0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(
		Rectangle{static_cast<float>(cardX), static_cast<float>(coinsCardY),
				  static_cast<float>(cardW), static_cast<float>(coinsCardH)},
		2.0f, Color{90, 72, 54, 255});

	// Coin icon and count
	const Texture2D &coinTex = textureManager->get(TextureId::Coin);
	const float coinScale = 1.8f;
	const int coinIconSize = static_cast<int>(coinTex.width * coinScale);
	const int coinIconX = cardX + 18;
	const int coinIconY = coinsCardY + (coinsCardH - coinIconSize) / 2;
	DrawTextureEx(
		coinTex,
		Vector2{static_cast<float>(coinIconX), static_cast<float>(coinIconY)},
		0.0f, coinScale, WHITE);

	const int coinFontSize = 28;
	const int coinTextX = coinIconX + coinIconSize + 18;
	const int coinTextY = coinsCardY + (coinsCardH - coinFontSize) / 2;
	DrawText(TextFormat("%d", playerStats.coins), coinTextX, coinTextY,
			 coinFontSize, Color{230, 210, 80, 255});

	// Message box overlay (draw last so it stays on top)
	RenderMessageBox();
}

void UI::RenderLose(const PlayerStats &playerStats) {
	(void)playerStats;
	RenderMessageBox();
}

void UI::TriggerMessageBox(const char *message) {
	if (message == nullptr) {
		return;
	}
	messageBoxText = message;
	messageBoxOpen = true;
}

void UI::CloseMessageBox() {
	messageBoxOpen = false;
	messageBoxText.clear();
}

void UI::RenderMessageBox() {
	if (!messageBoxOpen) {
		return;
	}

	const int screenW = GetScreenWidth();
	const int screenH = GetScreenHeight();

	// Dim background
	DrawRectangle(0, 0, screenW, screenH, Color{0, 0, 0, 150});

	// Dialog
	const float maxW = 560.0f;
	const float boxW = std::min(maxW, static_cast<float>(screenW) - 48.0f);
	const float boxH = 220.0f;
	const float boxX = (static_cast<float>(screenW) - boxW) * 0.5f;
	const float boxY = (static_cast<float>(screenH) - boxH) * 0.5f;
	Rectangle box{boxX, boxY, boxW, boxH};

	DrawRectangleRounded(box, 0.2f, 8, Color{34, 30, 26, 255});
	DrawRectangleLinesEx(box, 2.0f, Color{120, 96, 72, 255});

	// Message text (wrapped)
	const int fontSize = 20;
	const float textPadX = 18.0f;
	const float textTop = 18.0f;
	const float buttonAreaH = 70.0f;
	Rectangle textRect{boxX + textPadX, boxY + textTop, boxW - textPadX * 2.0f,
					   boxH - textTop - buttonAreaH};
	DrawWrappedText(GetFontDefault(), messageBoxText, textRect, fontSize, 1.0f,
					Color{220, 200, 170, 255});

	// OK button
	NewButton okBtn;
	const float okW = 140.0f;
	const float okH = 44.0f;
	okBtn = NewButton(0.0f, 0.0f, okW, okH, "OK");
	okBtn.SetColors(Color{34, 30, 26, 255}, Color{120, 96, 72, 255},
					Color{220, 200, 170, 255});
	const float okX = boxX + (boxW - okW) * 0.5f;
	const float okY = boxY + boxH - okH - 16.0f;
	okBtn.SetPosition(okX, okY);
	okBtn.Update();
	okBtn.Draw();

	if (okBtn.IsClicked()) {
		CloseMessageBox();
	}
}
