#pragma once

#include <raylib.h>
#include <string>

class SideCard {
  public:
	SideCard();
	SideCard(float x, float y, float width, float height,
			 const std::string &title, const std::string &subtitle,
			 const std::string &desc1, const std::string &desc2,
			 const std::string &desc3, const std::string &buttonText,
			 bool locked);

	void Update();
	void Render();

	void SetPosition(float x, float y);
	void SetSize(float width, float height);

	void SetColors(Color base, Color hover, Color bg);

	[[nodiscard]] bool IsClicked() const;
	[[nodiscard]] bool IsHovered() const;

  private:
	Rectangle rect{};

	bool isHovered = false;
	bool isClicked = false;

	std::string title;
	std::string subtitle;

	std::string desc1;
	std::string desc2;
	std::string desc3;

	std::string buttonText;

	bool isLocked = false;

	Color baseBorder{};
	Color hoverBorder{};
	Color hoverBg{};
};
