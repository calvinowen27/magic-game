#include "../include/game/Components.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Registry.hpp"

Component::Component()
{
    pGame = Game::getInstance();
    pRegistry = pGame->pRegistry;
}

TransformComponent::TransformComponent(Vector2 pos, Vector2 dims) : Component()
{
    _pos = pos;
    _dims = dims;
    _pxDims = (Vector2Int)(dims * pGame->ppm);
    _pxPos = pGame->worldToPixel(pos) - Vector2Int(0, _pxDims.y);

    pRegistry->addTransform(this);
}

void TransformComponent::update(float time)
{
    _pxDims = (Vector2Int)(_dims * pGame->ppm);
    _pxPos = pGame->worldToPixel(_pos) - Vector2Int(0, _pxDims.y);
}