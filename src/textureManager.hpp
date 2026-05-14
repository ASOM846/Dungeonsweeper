#pragma once
#include <iostream>
#include <raylib.h>
#include <stdexcept>
#include <string>
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
	Explosion0,
	Explosion1,
	Explosion2,
	Explosion3,
	Explosion4,
	Explosion5,
	Explosion6,
	Explosion7,
	Scratch0,
	Scratch1,
	Scratch2,
	Scratch3,
	Scratch4,
	Scratch5,
	Scratch6,
	Scratch7,
	OgreBig,
	OgreSmall,
	BeginButton,
	Necromancer,
	Player,
	Bomb,
	Coin,
	HeartFull,
	Skull,
	BlueWizzard,
	Key,
	Chest,
	SwordRed,
	SwordGreen,
	SwordGold,
	Hole,
	MysteryEnemy,
	FlaskGreen,
	Ladder,
	Floor1,
	Floor2,
	Floor3,
	Floor4,
	Floor5,
	Floor6,
	Floor7,
	Floor8,
};

class TextureManager {
  public:
	TextureManager() = default;
	~TextureManager() { unloadAll(); }

	void loadAll() {
		loadFont();

		load(TextureId::Enemy1, "assets/enemy1.png");
		load(TextureId::Enemy2, "assets/enemy2.png");
		load(TextureId::Enemy3, "assets/enemy3.png");
		load(TextureId::Enemy4, "assets/enemy4.png");
		load(TextureId::Enemy5, "assets/enemy5.png");
		load(TextureId::Enemy6, "assets/enemy6.png");
		load(TextureId::Enemy7, "assets/enemy7.png");
		load(TextureId::Enemy8, "assets/enemy8.png");
		load(TextureId::Enemy9, "assets/enemy9.png");
		load(TextureId::Explosion0, "assets/explosion000.png");
		load(TextureId::Explosion1, "assets/explosion001.png");
		load(TextureId::Explosion2, "assets/explosion002.png");
		load(TextureId::Explosion3, "assets/explosion003.png");
		load(TextureId::Explosion4, "assets/explosion004.png");
		load(TextureId::Explosion5, "assets/explosion005.png");
		load(TextureId::Explosion6, "assets/explosion006.png");
		load(TextureId::Explosion7, "assets/explosion007.png");
		load(TextureId::Scratch0, "assets/CS001.png");
		load(TextureId::Scratch1, "assets/CS002.png");
		load(TextureId::Scratch2, "assets/CS003.png");
		load(TextureId::Scratch3, "assets/CS004.png");
		load(TextureId::Scratch4, "assets/CS005.png");
		load(TextureId::Scratch5, "assets/CS006.png");
		load(TextureId::Scratch6, "assets/CS007.png");
		load(TextureId::Scratch7, "assets/CS008.png");
		load(TextureId::Enemy10, "assets/enemy10.png");
		load(TextureId::Enemy11, "assets/enemy11.png");
		load(TextureId::OgreBig, "assets/ogreBig.png");
		load(TextureId::OgreSmall, "assets/ogreSmall.png");
		load(TextureId::Necromancer, "assets/necromancer.png");
		load(TextureId::BeginButton, "assets/buttonBegin.png");
		load(TextureId::Player, "assets/player.png");
		load(TextureId::Bomb, "assets/bomb.png");
		load(TextureId::Coin, "assets/coin.png");
		load(TextureId::HeartFull, "assets/heartFull.png");
		load(TextureId::Skull, "assets/skull.png");
		load(TextureId::BlueWizzard, "assets/blueWizzard.png");
		load(TextureId::Key, "assets/key1.png");
		load(TextureId::Chest, "assets/chest1.png");
		load(TextureId::Hole, "assets/hole.png");
		load(TextureId::MysteryEnemy, "assets/mysteryEnemy.png");
		load(TextureId::FlaskGreen, "assets/flask_big_green.png");
		load(TextureId::Ladder, "assets/ladder.png");
		load(TextureId::SwordRed, "assets/swordRed.png");
		load(TextureId::SwordGreen, "assets/swordGreen.png");
		load(TextureId::SwordGold, "assets/swordGolden.png");
		load(TextureId::Floor1, "assets/floor1.png");
		load(TextureId::Floor2, "assets/floor2.png");
		load(TextureId::Floor3, "assets/floor3.png");
		load(TextureId::Floor4, "assets/floor4.png");
		load(TextureId::Floor5, "assets/floor5.png");
		load(TextureId::Floor6, "assets/floor6.png");
		load(TextureId::Floor7, "assets/floor7.png");
		load(TextureId::Floor8, "assets/floor8.png");
	}

	const Texture2D &get(TextureId id) const {
		auto it = textures.find(id);
		if (it == textures.end())
			throw std::runtime_error("Brak tekstury");
		return it->second;
	}

	const Font &getDefaultFont() const { return defaultFont; }

	void unloadAll() {
		for (auto &kv : textures) {
			UnloadTexture(kv.second);
		}
		textures.clear();

		UnloadFont(defaultFont);
	}

  private:
	void load(TextureId id, const char *path) {
		Texture2D tex = LoadTexture(path);
		if (tex.id == 0)
			throw std::runtime_error(std::string("Nie udało się wczytać: ") +
									 path);
		textures.emplace(id, tex);
	}

	void loadFont() {
		defaultFont = LoadFontEx("assets/SuperPixel.ttf", 60, 0, 0);

		if (!IsFontValid(defaultFont)) {
			std::cerr << "ERROR: Font loading failed using defaultFont.\n";

			defaultFont = GetFontDefault();
		}
	}

	std::unordered_map<TextureId, Texture2D> textures;
	Font defaultFont;
};
