#include <raylib.h>

namespace {

struct LogoAnim {
	int state = 0;
	int framesCounter = 0;
	int lettersCount = 0;
	float alpha = 0.0f;
	bool animFinished = false;
	float fadeGame = 0.0f;
};

LogoAnim logoAnim;

void UpdateAnim() {
	switch (logoAnim.state) {
	case 0:
		logoAnim.alpha += 0.02f;
		if (logoAnim.alpha >= 1.0f) {
			logoAnim.alpha = 1.0f;
			logoAnim.state = 1;
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
			logoAnim.framesCounter = 0;
		}
		break;
	case 3:
		logoAnim.framesCounter++;
		if (logoAnim.framesCounter > 60) {
			logoAnim.state = 4;
		}
		break;
	case 4:
		logoAnim.fadeGame += 0.04f;
		if (logoAnim.fadeGame >= 1.0f) {
			logoAnim.fadeGame = 1.0f;
			logoAnim.state = 5;
			logoAnim.animFinished = true;
		}
		break;
	case 5:
		break;
	}
}

void RenderAnim() {
	int screenW = GetScreenWidth();
	int screenH = GetScreenHeight();
	int centerX = screenW / 2;
	int centerY = screenH / 2;

	if (logoAnim.state < 3) {
		DrawRectangle(centerX - 112, centerY - 112, 224, 224,
					  Fade(RAYWHITE, logoAnim.alpha));
		if (logoAnim.state >= 1) {
			DrawText(TextSubtext("raylib", 0, logoAnim.lettersCount),
					 centerX - 44, centerY + 48, 50,
					 Fade(BLACK, logoAnim.alpha));
		}
	}

	if (logoAnim.state == 3) {
		DrawRectangle(0, 0, screenW, screenH, BLACK);
	}

	if (logoAnim.state >= 4) {
		DrawRectangle(0, 0, screenW, screenH,
					  Fade(BLACK, 1.0f - logoAnim.fadeGame));
	}
}

bool IsAnimFinished() {
	return logoAnim.animFinished;
}

} // namespace
