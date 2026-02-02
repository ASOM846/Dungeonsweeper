#include "gridManager.hpp"

void GridManager::Init() {
    gridGenerator.Init(grid);
}

void GridManager::Update(int &hp, int &maxHp, int &pointsToEvo) {
    gridGenerator.Update(grid, hp, maxHp, pointsToEvo);
}

void GridManager::Render(const TextureManager *textureManager, int hp, int pointsToEvo, int maxHp) {
    gridRender.RenderGrid(textureManager, grid);
}