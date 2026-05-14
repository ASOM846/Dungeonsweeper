#include "card.hpp"

SideCard::SideCard() {}

SideCard::SideCard(float x, float y, float width, float height,
				   const std::string &title, const std::string &subtitle,
				   const std::string &desc1, const std::string &desc2,
				   const std::string &desc3, const std::string &buttonText,
				   bool locked)
	: title(title), subtitle(subtitle), desc1(desc1), desc2(desc2),
	  desc3(desc3), buttonText(buttonText), isLocked(locked) {
	rect = {x, y, width, height};
}

void SideCard::Update() {
	Vector2 mousePos = GetMousePosition();

	isHovered = CheckCollisionPointRec(mousePos, rect);

	isClicked = isHovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

void SideCard::Render() {
	Rectangle bg = rect;

	if (isHovered) {
		bg.x -= 4;
		bg.y -= 4;
		bg.width += 8;
		bg.height += 8;
	}

	Color bgColor = isHovered ? hoverBg : Color{30, 25, 30, 220};

	Color borderColor = isHovered ? hoverBorder : baseBorder;

	DrawRectangleRec(bg, bgColor);

	DrawRectangleLinesEx(bg, 3.0f, borderColor);

	Rectangle innerRect = {bg.x + 8, bg.y + 8, bg.width - 16, bg.height - 16};

	DrawRectangleLinesEx(innerRect, 1.0f, ColorAlpha(borderColor, 0.3f));

	int titleFontSize = 40;

	DrawText(title.c_str(),
			 bg.x + (bg.width - MeasureText(title.c_str(), titleFontSize)) / 2,
			 bg.y + 30, titleFontSize, borderColor);

	int subFontSize = 26;

	DrawText(subtitle.c_str(),
			 bg.x + (bg.width - MeasureText(subtitle.c_str(), subFontSize)) / 2,
			 bg.y + 75, subFontSize, WHITE);

	DrawLine(bg.x + 30, bg.y + 120, bg.x + bg.width - 30, bg.y + 120,
			 Color{255, 255, 255, 50});

	int descFont = 20;

	DrawText(desc1.c_str(),
			 bg.x + (bg.width - MeasureText(desc1.c_str(), descFont)) / 2,
			 bg.y + 150, descFont, LIGHTGRAY);

	DrawText(desc2.c_str(),
			 bg.x + (bg.width - MeasureText(desc2.c_str(), descFont)) / 2,
			 bg.y + 180, descFont, LIGHTGRAY);

	DrawText(desc3.c_str(),
			 bg.x + (bg.width - MeasureText(desc3.c_str(), descFont)) / 2,
			 bg.y + 220, descFont, isLocked ? LIGHTGRAY : borderColor);

	Rectangle playBtn = {bg.x + 20, bg.y + bg.height - 60, bg.width - 40, 40};

	DrawRectangleRec(playBtn,
					 isHovered ? borderColor : ColorAlpha(borderColor, 0.4f));

	DrawText(buttonText.c_str(),
			 playBtn.x +
				 (playBtn.width - MeasureText(buttonText.c_str(), 20)) / 2,
			 playBtn.y + 10, 20, isLocked && !isHovered ? LIGHTGRAY : BLACK);
}

void SideCard::Reset() {
	isHovered = false;
}

void SideCard::SetPosition(float x, float y) {
	rect.x = x;
	rect.y = y;
}

void SideCard::SetSize(float width, float height) {
	rect.width = width;
	rect.height = height;
}

void SideCard::SetColors(Color base, Color hover, Color bg) {
	baseBorder = base;
	hoverBorder = hover;
	hoverBg = bg;
}

bool SideCard::IsClicked() const {
	return isClicked;
}

bool SideCard::IsHovered() const {
	return isHovered;
}
