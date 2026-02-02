#pragma once
#include "../textureManager.hpp"
#include <raylib.h>

class UI {
    public:
    UI() = default;
    ~UI() = default;

    void Init(TextureManager *tm) {
        textureManager = tm;
    }
    
    void RenderUi(const int hp, const int pointsToEvo, const int maxHp);

    const int GetBarWidth() const { return UI_BAR_WIDTH; }
    constexpr static int UI_BAR_WIDTH = 200;

    private:
    TextureManager *textureManager;
};