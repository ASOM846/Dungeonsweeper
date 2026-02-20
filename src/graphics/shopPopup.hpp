#pragma once
#include "../grid/grid.hpp"
#include "raylib.h"
#include <algorithm>
#include <raymath.h>
#include <string>

struct ShopPopupInfo {
	Texture2D icon;
	std::string name;
	int price;
	std::string description;
	bool valid = false;
};

void HandlePopup(const Grid &grid, Vector2 offset) {}

void DrawShopPopup(const ShopPopupInfo &info, Vector2 cursor,
				   const Texture2D &tex) {
	if (!info.valid)
		return;
	const float pad = 12.0f;
	const float iconSize = 38.0f;
	const int fontSize = 22;
	const int descFont = 16;
	std::string priceStr = std::to_string(info.price);

	DrawText("DRAWED POPUP", 10, 10, 20, RED);
	// Estimate panel size
	int nameW = MeasureText(info.name.c_str(), fontSize);
	int priceW = MeasureText(priceStr.c_str(), fontSize);
	int descW = MeasureText(info.description.c_str(), descFont);
	float w = pad * 2 +
			  std::max(std::max(iconSize + 10 + nameW, priceW + iconSize + 10),
					   static_cast<float>(descW));
	float h = pad * 2 + iconSize + 10 + fontSize + 8 + descFont;

	// Position panel near cursor, clamp to screen
	Vector2 pos = cursor + Vector2{24, 24};
	if (pos.x + w > GetScreenWidth())
		pos.x = GetScreenWidth() - w - 8;
	if (pos.y + h > GetScreenHeight())
		pos.y = GetScreenHeight() - h - 8;

	DrawRectangleRounded({pos.x, pos.y, w, h}, 0.18f, 6,
						 Color{34, 30, 26, 240});
	DrawRectangleLinesEx({pos.x, pos.y, w, h}, 2.0f, Color{210, 175, 70, 255});

	// Icon and name
	DrawTextureEx(info.icon, {pos.x + pad, pos.y + pad}, 0,
				  iconSize / info.icon.width, WHITE);
	DrawText(info.name.c_str(), pos.x + pad + iconSize + 10, pos.y + pad + 6,
			 fontSize, Color{220, 200, 170, 255});

	// Price with coin
	Texture2D coin = tex;
	float coinScale = fontSize * 0.9f / coin.height;
	DrawTextureEx(coin, {pos.x + pad, pos.y + pad + iconSize + 8}, 0, coinScale,
				  WHITE);
	DrawText(priceStr.c_str(), pos.x + pad + coin.width * coinScale + 8,
			 pos.y + pad + iconSize + 8, fontSize, Color{230, 210, 80, 255});

	// Description
	DrawText(info.description.c_str(), pos.x + pad,
			 pos.y + pad + iconSize + 8 + fontSize + 8, descFont,
			 Color{180, 170, 140, 255});
}
