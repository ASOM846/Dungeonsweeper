#pragma once
#include <raylib.h>
#include <stdexcept>
#include <unordered_map>

enum class TextureId {
	Enemy1,
	Enemy2,
	Enemy3,
	Enemy4,
	Enemy5,
	Enemy6,
	Enemy7,
	Enemy8,
	Enemy9,
	Enemy10,
	Enemy11,
	BeginButton,
	Player,
	Coin,
	HeartFull,
	Floor1,
};

class TextureManager {
  public:
	TextureManager() = default;
	~TextureManager() { unloadAll(); }

	void loadAll() {
		load(TextureId::Enemy1, "assets/enemy1.png");
		load(TextureId::Enemy2, "assets/enemy2.png");
		load(TextureId::Enemy3, "assets/enemy3.png");
		load(TextureId::Enemy4, "assets/enemy4.png");
		load(TextureId::Enemy5, "assets/enemy5.png");
		load(TextureId::Enemy6, "assets/enemy6.png");
		load(TextureId::Enemy7, "assets/enemy7.png");
		load(TextureId::Enemy8, "assets/enemy8.png");
		load(TextureId::Enemy9, "assets/enemy9.png");
		load(TextureId::Enemy10, "assets/enemy10.png");
		load(TextureId::Enemy11, "assets/enemy11.png");
		load(TextureId::BeginButton, "assets/buttonBegin.png");
		load(TextureId::Player, "assets/player.png");
		load(TextureId::Coin, "assets/coin.png");
		load(TextureId::HeartFull, "assets/heartFull.png");
		load(TextureId::Floor1, "assets/floor1.png");
	}

	const Texture2D &get(TextureId id) const {
		auto it = textures.find(id);
		if (it == textures.end())
			throw std::runtime_error("Brak tekstury");
		return it->second;
	}

	void unloadAll() {
		for (auto &kv : textures) {
			UnloadTexture(kv.second);
		}
		textures.clear();
	}

  private:
	void load(TextureId id, const char *path) {
		Texture2D tex = LoadTexture(path);
		if (tex.id == 0)
			throw std::runtime_error(std::string("Nie udało się wczytać: ") +
									 path);
		textures.emplace(id, tex);
	}

	std::unordered_map<TextureId, Texture2D> textures;
};