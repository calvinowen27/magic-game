#include "../../include/game/Animation/Animation.hpp"
#include "../../include/game/Animation/AnimationManager.hpp"
#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/ECS/Entity.hpp"
#include "../../include/game/ObjectManager.hpp"
#include "../../include/game/ContentManager.hpp"
#include "../../include/game/Game.hpp"

Animation::Animation(AnimationManager *animationManager) : _game(*Game::getInstance()), _animationManager(*animationManager), _objectManager(*ObjectManager::getInstance()), _contentManager(*_game.pContentManager)
{
}

void Animation::init(EntityType entityType, std::string name, float duration, int frameCount, int animationIdx, bool loops)
{
    _entityType = entityType;
    _name = name;
    _duration = duration;
    _frameCount = frameCount;
    _frameTime = _duration / _frameCount;
    _animationIdx = animationIdx;
    _loops = loops;
    _pTexture = _contentManager.getTextureFromType(_entityType);

    Vector2Int spriteDims = _objectManager.getSpriteDims(entityType);
    Vector2Int textureDims;
    SDL_QueryTexture(_pTexture, NULL, NULL, &textureDims.x, &textureDims.y);

    int frameX = 0;
    int frameY = _animationIdx * spriteDims.y;

    for (int i = 0; i < _frameCount; i++)
    {
        _rects.push_back(SDL_Rect{ frameX, frameY, spriteDims.x, spriteDims.y });
        frameX += spriteDims.x;
        if (frameX >= textureDims.x)
        {
            frameX = 0;
            frameY += spriteDims.y;
        }
    }
}