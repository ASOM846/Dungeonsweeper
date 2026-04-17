#include <raylib.h>

namespace {
struct LogoAnim {
	int state = 0;
	int framesCounter = 0;
	int lettersCount = 0;
	float alpha = 0.0f;
	bool animFinished = true;
};

LogoAnim logoAnim;

void UpdateAnim() {
	if (logoAnim.animFinished)
		return;

	switch (logoAnim.state) {
	case 0:
		logoAnim.alpha += 0.02f;
		if (logoAnim.alpha >= 1.0f) {
			logoAnim.alpha = 1.0f;
			logoAnim.state = 1;
			logoAnim.framesCounter = 0;
		}
		break;
	case 1:
		logoAnim.framesCounter++;
		if (logoAnim.framesCounter >= 12) {
			logoAnim.lettersCount++;
			logoAnim.framesCounter = 0;
		}
		if (logoAnim.lettersCount >= 6) {
			logoAnim.state = 2;
		}
		break;
	case 2:
		logoAnim.alpha -= 0.02f;
		if (logoAnim.alpha <= 0.0f) {
			logoAnim.alpha = 0.0f;
			logoAnim.state = 3;
			logoAnim.animFinished = true;
		}
		break;
	}
}

void RenderAnim() {
	if (logoAnim.animFinished)
		return;

	int centerX = GetScreenWidth() / 2;
	int centerY = GetScreenHeight() / 2;

	DrawRectangle(centerX - 112, centerY - 112, 224, 224,
				  Fade(RAYWHITE, logoAnim.alpha));

	if (logoAnim.state >= 1) {
		DrawText(TextSubtext("raylib", 0, logoAnim.lettersCount), centerX - 44,
				 centerY + 48, 50, Fade(BLACK, logoAnim.alpha));
	}
}

bool IsAnimFinished() {
	return logoAnim.animFinished;
}
} // namespace
