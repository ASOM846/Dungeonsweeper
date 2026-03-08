#pragma once
#include "raylib.h"
#include <algorithm>
#include <raymath.h>
#include <string>

struct ShopPopupInfo {
	std::string name;
	int val; // uniwersalne pole na hp, exp, gold itd.
	std::string description;
	bool valid = false;
};

inline void DrawShopPopup(const ShopPopupInfo &info, Vector2 cursor) {
	if (!info.valid)
		return;

	const float pad = 10.0f;
	const float headerH = 28.0f;
	const int titleFont = 18;
	const int valueFont = 20;
	const int descFont = 13;
	const float minW = 160.0f;
	const float maxW = 260.0f;

	std::string value = std::to_string(info.val);

	int titleW = MeasureText(info.name.c_str(), titleFont);
	int valueW = MeasureText(value.c_str(), valueFont);
	int descW = MeasureText(info.description.c_str(), descFont);

	float w =
		std::max(minW, std::min(maxW, pad * 2 + static_cast<float>(std::max(
													{titleW, valueW, descW}))));
	float h = headerH + pad + 24.0f + descFont + pad;

	Vector2 pos = cursor + Vector2{12, 12};
	if (pos.x + w > GetScreenWidth())
		pos.x = GetScreenWidth() - w - 4;
	if (pos.y + h > GetScreenHeight())
		pos.y = GetScreenHeight() - h - 4;
	pos.x = std::max(4.0f, pos.x);
	pos.y = std::max(4.0f, pos.y);

	Rectangle panel = {pos.x, pos.y, w, h};
	Rectangle header = {pos.x, pos.y, w, headerH};

	// Minimalistyczne tło i cienka ramka
	DrawRectangleRounded(panel, 0.13f, 6, Color{32, 30, 28, 235});
	DrawRectangleRoundedLinesEx(panel, 0.13f, 6, 1.2f,
								Color{180, 160, 90, 180});

	// Tytuł (wyśrodkowany)
	int titleX = pos.x + (w - titleW) / 2;
	DrawText(info.name.c_str(), titleX, pos.y + 6, titleFont,
			 Color{220, 210, 180, 255});

	// Wartość (wyśrodkowana, pod tytułem)
	int valueX = pos.x + (w - valueW) / 2;
	DrawText(value.c_str(), valueX, pos.y + headerH + 2, valueFont,
			 Color{200, 180, 120, 255});

	// Opis (wyśrodkowany, pod wartością)
	int descX = pos.x + (w - descW) / 2;
	DrawText(info.description.c_str(), descX, pos.y + headerH + 20, descFont,
			 Color{170, 160, 140, 255});
}
