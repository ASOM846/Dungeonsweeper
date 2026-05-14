#include "../textureManager.hpp"
#include <raylib.h>

class MouseManager {
  public:
	MouseManager() = default;
	~MouseManager() = default;

	static void Render(const TextureManager &tm) {
		Texture2D tex = tm.get(TextureId::SwordGold);
		Vector2 mousePos = GetMousePosition();
		float scale = 2.0f;
		float rotation = -45.0f;

		// 1. Prostokąt źródłowy (cała tekstura)
		Rectangle source = {0.0f, 0.0f, static_cast<float>(tex.width),
							static_cast<float>(tex.height)};

		// 2. Prostokąt docelowy
		// Uwaga: dest.x i dest.y to pozycja myszki. Szerokość i wysokość są
		// pomnożone przez skalę.
		Rectangle dest = {mousePos.x, mousePos.y, tex.width * scale,
						  tex.height * scale};

		// ==========================================
		// 3. PUNKT ZAKOTWICZENIA (ORIGIN) - KLUCZOWE
		// ==========================================
		// Musisz podać, w którym miejscu na teksturze znajduje się czubek
		// miecza. Jeśli Twój miecz na oryginalnym obrazku (przed obrotem) jest
		// skierowany:
		// - Prosto w górę: tipX = tex.width / 2.0f, tipY = 0.0f
		// - W prawy górny róg: tipX = tex.width, tipY = 0.0f
		// - W lewy górny róg: tipX = 0.0f, tipY = 0.0f

		float tipX =
			tex.width / 2.0f; // ZMIEŃ TO w zależności od Twojego obrazka
		float tipY = 0.0f;	  // ZMIEŃ TO w zależności od Twojego obrazka

		// Origin musi uwzględniać skalę!
		Vector2 origin = {tipX * scale, tipY * scale};

		// Rysujemy!
		DrawTexturePro(tex, source, dest, origin, rotation, RAYWHITE);
	}
};
