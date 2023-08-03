#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions)
{
    _pGame = Game::getInstance();
    _pObjectManager = ObjectManager::getInstance();
    _pContentManager = _pGame->pContentManager;

    _type = objType;

    _pTexture = _pContentManager->getTextureFromType(_type);

    _pTransform = new TransformComponent(pos, dims);

    // _pos = pos;
    // _dims = dims;
    // _pxDims = (Vector2Int)(dims * _pGame->ppm);
    // _pxPos = _pGame->worldToPixel(pos) - Vector2Int(0, _pxDims.y);
    _colBox = _pObjectManager->getColBox(_type);
    _doCollisions = doCollisions;

    _spriteRect = SDL_Rect{_pTransform->pxPos().x, _pTransform->pxPos().y, _pTransform->pxDims().x, _pTransform->pxDims().y};

    _pObjectManager->addObject(this);
}

Object::~Object()
{
    SDL_DestroyTexture(_pTexture);
}

void Object::draw(SDL_Renderer *pRenderer)
{
    if (!_pGame->objOnScreen(*this))
        return;

    _spriteRect = SDL_Rect{_pTransform->pxPos().x, _pTransform->pxPos().y, _pTransform->pxDims().x, _pTransform->pxDims().y};

    SDL_Point axis{0, _spriteRect.h};
    if (_flipped)
        axis.x = _spriteRect.w;

    SDL_RenderCopyEx(pRenderer, _pTexture, NULL, &_spriteRect, _textureAngle, &axis, _velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Object::update(float time)
{
    // _pxPos = _pGame->worldToPixel(_pos) - Vector2Int(0, _pxDims.y);
    // _pxDims = (Vector2Int)(_dims * _pGame->ppm);

    if (SDL_fabsf(_velocity.x) < 0.0625)
        _velocity.x = 0;
    if (SDL_fabsf(_velocity.y) < 0.0625)
        _velocity.y = 0;

    Vector2 deltaPos = _velocity * time;

    handleCollisions(deltaPos);
}

void Object::handleCollisions(Vector2 deltaPos)
{
    Vector2 nextPos = _pTransform->pos() + deltaPos;

    colLeftX = _pTransform->pos().x + (_colBox.start().x * _pTransform->dims().x);
    colRightX = _pTransform->pos().x + (_colBox.end().x * _pTransform->dims().x);
    colBottomY = _pTransform->pos().y + (_colBox.start().y * _pTransform->dims().y);
    colTopY = _pTransform->pos().y + (_colBox.end().y * _pTransform->dims().y);

    if (_isStatic)
        return;

    for (Object *other : _pObjectManager->getObjs())
    {
        // skip if self
        if (other == this)
            continue;

        if (!other->_doCollisions)
            continue;

        // skip if not touching
        if (colLeftX + deltaPos.x >= other->colRightX || colRightX + deltaPos.x <= other->colLeftX ||
            colBottomY + deltaPos.y >= other->colTopY || colTopY + deltaPos.y <= other->colBottomY)
            continue;

        // this left and other right
        if (borderEnabled[0] && other->borderEnabled[1])
        {
            if (colLeftX >= other->colRightX && colLeftX + deltaPos.x < other->colRightX)
                nextPos.x = other->colRightX - (_colBox.start().x * _pTransform->dims().x);
        }

        // this right and other left
        if (borderEnabled[1] && other->borderEnabled[0])
        {
            if (colRightX <= other->colLeftX && colRightX + deltaPos.x > other->colLeftX)
                nextPos.x = other->colLeftX - (_colBox.end().x * _pTransform->dims().x);
        }

        // this bottom and other top
        if (borderEnabled[2] && other->borderEnabled[3])
        {
            if (colBottomY >= other->colTopY && colBottomY + deltaPos.y < other->colTopY)
                nextPos.y = other->colTopY - (_colBox.start().y * _pTransform->dims().y);
        }

        // this top and other bottom
        if (borderEnabled[3] && other->borderEnabled[2])
        {
            if (colTopY <= other->colBottomY && colTopY + deltaPos.y > other->colBottomY)
                nextPos.y = other->colBottomY - (_colBox.end().y * _pTransform->dims().y);
        }
    }

    _pTransform->pos() = nextPos;
}