#include "gridManager.hpp"

void GridManager::Init() {
    gridGenerator.Init(grid);
}

void GridManager::Update() {
    gridGenerator.Update(grid, hp, maxHp, pointsToEvo);
}

void GridManager::Render(const TextureManager *textureManager) {
    gridRender.RenderGrid(textureManager, grid);
    gridRender.RenderUi(hp, pointsToEvo, maxHp);
}
