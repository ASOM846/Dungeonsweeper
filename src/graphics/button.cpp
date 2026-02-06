#include "button.hpp"

Color Green = Color{25, 25, 112, 255};		 // Midnight Blue
Color HoverColor = Color{72, 61, 139, 255};	 // Dark Slate Blue
Color PressedColor = Color{47, 79, 79, 255}; // Dark Slate Gray

Button::Button()
	: x(0), y(0), width(100), height(50), label("Button"), color(Green),
	  isToggled(false) {}

Button::Button(float x, float y, float width, float height,
			   const std::string &label)
	: x(x), y(y), width(width), height(height), label(label), color(Green),
	  isToggled(false) {}

void Button::Draw() const {
	DrawRectangle(x, y, width, height, color);

	int textWidth = MeasureText(label.c_str(), 20);
	int textX = x + (width - textWidth) / 2;
	int textY = y + (height - 20) / 2;

	DrawText(label.c_str(), textX, textY, 20, WHITE);
}

bool Button::IsClicked() const {
	return CheckCollisionPointRec(GetMousePosition(), {x, y, width, height}) &&
		   IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Button::IsHovered() const {
	return CheckCollisionPointRec(GetMousePosition(), {x, y, width, height});
}

void Button::SetLabel(const std::string &newLabel) {
	label = newLabel;
}

void Button::Update() {
	if (CheckCollisionPointRec(GetMousePosition(), {x, y, width, height})) {
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			color = PressedColor;
		} else {
			color = HoverColor;
		}
	} else {
		color = Green;
	}
}

void Button::Toggle() {
	size_t pos = label.find(" : OFF");
	if (pos != std::string::npos) {
		label = label.substr(0, pos);
	}
	pos = label.find(" : ON");
	if (pos != std::string::npos) {
		label = label.substr(0, pos);
	}

	isToggled = !isToggled;
	if (isToggled) {
		SetLabel(label + " : OFF");
	} else {
		SetLabel(label + " : ON");
	}
}

void Button::SetPosition(float newX, float newY) {
	x = newX;
	y = newY;
}

float Button::GetWidth() const {
	return width;
}

float Button::GetHeight() const {
	return height;
}

float Button::GetX() const { // Implementacja metody GetX
	return x;
}

float Button::GetY() const { // Implementacja metody GetY
	return y;
}

// NewButton
NewButton::NewButton()
	: x(0), y(0), width(100), height(50), label("Button"),
	  textColor(Color{220, 200, 170, 255}), // label color
	  isToggled(false), fillColor(Color{34, 30, 26, 255}),
	  borderColor(Color{120, 96, 72, 255}),
	  hoverFillColor(Color{40, 34, 28, 255}),
	  hoverBorderColor(Color{160, 130, 100, 255}),
	  pressedFillColor(Color{22, 20, 18, 255}),
	  pressedBorderColor(Color{140, 116, 90, 255}), roundness(0.22f),
	  segments(8), fontSize(20), hovered(false), pressed(false) {}

NewButton::NewButton(float xPos, float yPos, float btnWidth, float btnHeight,
					 const std::string &btnLabel)
	: x(xPos), y(yPos), width(btnWidth), height(btnHeight), label(btnLabel),
	  textColor(Color{220, 200, 170, 255}), isToggled(false),
	  fillColor(Color{34, 30, 26, 255}), borderColor(Color{120, 96, 72, 255}),
	  hoverFillColor(Color{40, 34, 28, 255}),
	  hoverBorderColor(Color{160, 130, 100, 255}),
	  pressedFillColor(Color{22, 20, 18, 255}),
	  pressedBorderColor(Color{140, 116, 90, 255}), roundness(0.22f),
	  segments(8), fontSize(20), hovered(false), pressed(false) {}

void NewButton::SetColors(Color newFill, Color newBorder, Color newText) {
	fillColor = newFill;
	borderColor = newBorder;
	textColor = newText;

	// sensowne domyślne warianty dla hover/press (lekko jaśniej / ciemniej)
	hoverFillColor = Color{static_cast<unsigned char>(
						  (int)newFill.r + 8 > 255 ? 255 : (int)newFill.r + 8),
					  static_cast<unsigned char>(
						  (int)newFill.g + 8 > 255 ? 255 : (int)newFill.g + 8),
					  static_cast<unsigned char>(
						  (int)newFill.b + 8 > 255 ? 255 : (int)newFill.b + 8),
					  newFill.a};
	hoverBorderColor = newBorder;
	pressedFillColor = Color{static_cast<unsigned char>((int)newFill.r / 2),
						 static_cast<unsigned char>((int)newFill.g / 2),
						 static_cast<unsigned char>((int)newFill.b / 2), newFill.a};
	pressedBorderColor = newBorder;
}

void NewButton::Draw() const {
	Color currentFillColor = fillColor;
	Color currentBorderColor = borderColor;
	Color currentTextColor = textColor;

	if (pressed) {
		currentFillColor = pressedFillColor;
		currentBorderColor = pressedBorderColor;
		auto subClamp = [](unsigned char v, unsigned char sub) -> unsigned char {
			return (v > sub) ? static_cast<unsigned char>(v - sub)
						 : static_cast<unsigned char>(0);
		};
		currentTextColor = Color{subClamp(textColor.r, 10), subClamp(textColor.g, 10),
							  subClamp(textColor.b, 10), textColor.a};
	} else if (hovered) {
		currentFillColor = hoverFillColor;
		currentBorderColor = hoverBorderColor;
	}

	Rectangle tileRect{x, y, width, height};

	// cień pod przyciskiem (lekki "lift")
	Rectangle shadowRect{x, y + 3.0f, width, height};
	DrawRectangleRounded(shadowRect, roundness, segments, Color{0, 0, 0, 70});

	// główne tło
	DrawRectangleRounded(tileRect, roundness, segments, currentFillColor);

	// subtelny highlight u góry (pseudo-gradient jak w UI)
	float highlightH = height * 0.38f;
	if (highlightH < 6.0f) {
		highlightH = 6.0f;
	}
	Rectangle highlightRect{x + 2.0f, y + 2.0f, width - 4.0f, highlightH};
	DrawRectangleRounded(highlightRect, roundness, segments,
							  Color{255, 255, 255, static_cast<unsigned char>(hovered ? 22 : 14)});

	// obrys
	DrawRectangleRoundedLines(tileRect, roundness, segments, currentBorderColor);

	// tekst
	int textWidth = MeasureText(label.c_str(), fontSize);
	int textX = static_cast<int>(x + (width - textWidth) / 2);
	int textY = static_cast<int>(y + (height - fontSize) / 2);
	DrawText(label.c_str(), textX, textY, fontSize, currentTextColor);
}

bool NewButton::IsClicked() const {
	return CheckCollisionPointRec(GetMousePosition(), {x, y, width, height}) &&
		   IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool NewButton::IsHovered() const {
	return CheckCollisionPointRec(GetMousePosition(), {x, y, width, height});
}

void NewButton::SetLabel(const std::string &newLabel) {
	label = newLabel;
}

void NewButton::Update() {
	hovered = CheckCollisionPointRec(GetMousePosition(), {x, y, width, height});
	pressed = hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
}

void NewButton::Toggle() {
	size_t pos = label.find(" : OFF");
	if (pos != std::string::npos) {
		label = label.substr(0, pos);
	}
	pos = label.find(" : ON");
	if (pos != std::string::npos) {
		label = label.substr(0, pos);
	}

	isToggled = !isToggled;
	if (isToggled) {
		SetLabel(label + " : OFF");
	} else {
		SetLabel(label + " : ON");
	}
}

void NewButton::SetPosition(float newX, float newY) {
	x = newX;
	y = newY;
}

float NewButton::GetWidth() const {
	return width;
}

float NewButton::GetHeight() const {
	return height;
}

float NewButton::GetX() const {
	return x;
}

float NewButton::GetY() const {
	return y;
}
