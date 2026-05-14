#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <raylib.h>
#include <string>

class Button {
  public:
	Button();
	Button(float x, float y, float width, float height,
		   const std::string &label);
	void Draw() const;
	bool IsClicked() const;
	bool IsHovered() const;
	void SetLabel(const std::string &newLabel);
	void Update();
	void Toggle();
	void SetPosition(float newX, float newY);
	float GetWidth() const;
	float GetHeight() const;
	float GetX() const;
	float GetY() const;

  private:
	float x;
	float y;
	float width;
	float height;
	std::string label;
	Color color;
	bool isToggled;
};

class NewButton {
  public:
	NewButton();
	NewButton(float xPos, float yPos, float btnWidth, float btnHeight,
			  const std::string &btnLabel, const Font &font);
	void SetColors(Color newFill, Color newBorder, Color newText);
	void Draw() const;
	bool IsClicked() const;
	bool IsHovered() const;
	void SetLabel(const std::string &newLabel);
	void Update();
	void Toggle();
	void SetPosition(float newX, float newY);
	void SetSize(float newWidth, float newHeight);
	float GetWidth() const;
	float GetHeight() const;
	float GetX() const;
	float GetY() const;

  private:
	float x;
	float y;
	float width;
	float height;
	std::string label;
	Color textColor;
	bool isToggled;

	Color fillColor;
	Color borderColor;
	Color hoverFillColor;
	Color hoverBorderColor;
	Color pressedFillColor;
	Color pressedBorderColor;
	float roundness;
	int segments;
	int fontSize;
	bool hovered;
	bool pressed;
	Font font;
};

#endif // BUTTON_HPP
