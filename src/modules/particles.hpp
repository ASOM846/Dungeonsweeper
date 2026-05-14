#pragma once
#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

class ParticleSystem {
  public:
	enum ParticleType {
		WATER = 0,
		SMOKE,
		FIRE
	};

  private:
	struct Particle {
		ParticleType type;
		Vector2 position;
		Vector2 velocity;
		float size; // Zastąpiono 'radius' i 'Rectangle' pojedynczym rozmiarem
		Color color;
		float lifeTime;
		bool alive;
	};

	std::vector<Particle> particles;

	Vector2 emitterPosition;
	ParticleType currentType = WATER;

	int emissionRate = 1; // particles per frame
	int maxParticles = 3000;

  public:
	ParticleSystem(Vector2 pos = {-100, -100}, int maxCount = 3000) {
		emitterPosition = pos;
		maxParticles = maxCount;
		particles.reserve(maxParticles);
	}

	void SetPosition(Vector2 pos) { emitterPosition = pos; }

	void SetType(ParticleType type) { currentType = type; }

	void SetEmissionRate(int rate) { emissionRate = rate; }

	void Emit() {
		if ((int)particles.size() >= maxParticles)
			return;

		Particle p;
		p.type = currentType;
		p.position = emitterPosition;
		p.lifeTime = 0.0f;
		p.alive = true;

		float speed = (float)(rand() % 10) / 5.0f;

		switch (currentType) {
		case WATER:
			p.size = 10.0f; // Wcześniej radius = 5
			p.color = BLUE;
			break;

		case SMOKE:
			p.size = 14.0f; // Wcześniej radius = 7
			p.color = GRAY;
			break;

		case FIRE:
			p.size = 20.0f; // Wcześniej radius = 10
			p.color = YELLOW;
			speed *= 0.2f;
			break;
		}

		float angle = (float)(rand() % 360) * DEG2RAD;

		p.velocity = {speed * cosf(angle), speed * sinf(angle)};

		particles.push_back(p);
	}

	// =========================
	// UPDATE
	// =========================
	void Update(int screenWidth, int screenHeight) {
		// emit
		if (emissionRate < 0) {
			if (rand() % (-emissionRate) == 0)
				Emit();
		} else {
			for (int i = 0; i < emissionRate; i++)
				Emit();
		}

		// update particles
		for (auto &p : particles) {
			if (!p.alive)
				continue;

			p.lifeTime += 1.0f / 60.0f;

			switch (p.type) {
			case WATER:
				p.position.x += p.velocity.x;
				p.velocity.y += 0.2f;
				p.position.y += p.velocity.y;
				break;

			case SMOKE:
				p.position.x += p.velocity.x;
				p.velocity.y -= 0.05f;
				p.position.y += p.velocity.y;
				p.size += 1.0f; // Zwiększanie rozmiaru
				p.color.a -= 4;

				if (p.color.a < 10)
					p.alive = false;
				break;

			case FIRE:
				p.position.x += p.velocity.x + cosf(p.lifeTime * 10.0f);
				p.velocity.y -= 0.05f;
				p.position.y += p.velocity.y;
				p.size -= 0.3f; // Zmniejszanie kwadratu
				p.color.g -= 3;

				if (p.size <= 0.2f)
					p.alive = false;
				break;
			}

			// out of screen
			if (p.position.x < -20 || p.position.x > screenWidth + 20 ||
				p.position.y < -20 || p.position.y > screenHeight + 20) {
				p.alive = false;
			}
		}

		// cleanup
		particles.erase(std::remove_if(particles.begin(), particles.end(),
									   [](Particle &p) { return !p.alive; }),
						particles.end());
	}

	void Draw() {
		for (auto &p : particles) {
			if (!p.alive)
				continue;

			// Rysujemy kwadrat. Odejmujemy połowę rozmiaru od X i Y,
			// aby pozycja (p.position) była idealnie na środku kwadratu,
			// tak jak to działa w DrawCircleV.
			Vector2 topLeft = {p.position.x - p.size / 2.0f,
							   p.position.y - p.size / 2.0f};
			Vector2 rectSize = {p.size, p.size};

			DrawRectangleV(topLeft, rectSize, p.color);
		}
	}
};
