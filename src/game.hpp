#pragma once
#include <raylib.h>
#include "grid/gridManager.hpp"
#include "textureManager.hpp"

class Game {
    public:
    Game() = default;
    ~Game() = default;

    void Init();

    void Run();

    void Update();
    void Draw();

private:
    GridManager gridManager;
    TextureManager textureManager;
};