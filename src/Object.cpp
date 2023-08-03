#include "../include/game/Object.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ObjectManager.hpp"
#include "../include/game/ContentManager.hpp"

#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_image.h"

Object::Object(std::string objType, Vector2 pos, Vector2 dims, bool doCollisions)
{
    pGame = Game::getInstance();
    pObjectManager = ObjectManager::getInstance();
    pContentManager = pGame->pContentManager;

    type = objType;

    _pTexture = pContentManager->getTextureFromType(type);

    pTransform = new TransformComponent(pos, dims);

    // _pos = pos;
    // _dims = dims;
    // _pxDims = (Vector2Int)(dims * _pGame->ppm);
    // _pxPos = _pGame->worldToPixel(pos) - Vector2Int(0, _pxDims.y);
    colBox = pObjectManager->getColBox(type);
    _doCollisions = doCollisions;

    _spriteRect = SDL_Rect{pTransform->pxPos().x, pTransform->pxPos().y, pTransform->pxDims().x, pTransform->pxDims().y};

    pObjectManager->addObject(this);
}

Object::~Object()
{
    SDL_DestroyTexture(_pTexture);
}

void Object::draw(SDL_Renderer *pRenderer)
{
    if (!pGame->objOnScreen(*this))
        return;

    _spriteRect = SDL_Rect{pTransform->pxPos().x, pTransform->pxPos().y, pTransform->pxDims().x, pTransform->pxDims().y};

    SDL_Point axis{0, _spriteRect.h};
    if (_flipped)
        axis.x = _spriteRect.w;

    SDL_RenderCopyEx(pRenderer, _pTexture, NULL, &_spriteRect, _textureAngle, &axis, velocity.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Object::update(float time)
{
    // _pxPos = _pGame->worldToPixel(_pos) - Vector2Int(0, _pxDims.y);
    // _pxDims = (Vector2Int)(_dims * _pGame->ppm);

    if (SDL_fabsf(velocity.x) < 0.0625)
        velocity.x = 0;
    if (SDL_fabsf(velocity.y) < 0.0625)
        velocity.y = 0;

    Vector2 deltaPos = velocity * time;

    handleCollisions(deltaPos);
}

void Object::handleCollisions(Vector2 deltaPos)
{
    Vector2 nextPos = pTransform->pos() + deltaPos;

    colLeftX = pTransform->pos().x + (colBox.start().x * pTransform->dims().x);
    colRightX = pTransform->pos().x + (colBox.end().x * pTransform->dims().x);
    colBottomY = pTransform->pos().y + (colBox.start().y * pTransform->dims().y);
    colTopY = pTransform->pos().y + (colBox.end().y * pTransform->dims().y);

    if (isStatic)
        return;

    for (Object *other : pObjectManager->getObjs())
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
        if (_borderEnabled[0] && other->_borderEnabled[1])
        {
            if (colLeftX >= other->colRightX && colLeftX + deltaPos.x < other->colRightX)
                nextPos.x = other->colRightX - (colBox.start().x * pTransform->dims().x);
        }

        // this right and other left
        if (_borderEnabled[1] && other->_borderEnabled[0])
        {
            if (colRightX <= other->colLeftX && colRightX + deltaPos.x > other->colLeftX)
                nextPos.x = other->colLeftX - (colBox.end().x * pTransform->dims().x);
        }

        // this bottom and other top
        if (_borderEnabled[2] && other->_borderEnabled[3])
        {
            if (colBottomY >= other->colTopY && colBottomY + deltaPos.y < other->colTopY)
                nextPos.y = other->colTopY - (colBox.start().y * pTransform->dims().y);
        }

        // this top and other bottom
        if (_borderEnabled[3] && other->_borderEnabled[2])
        {
            if (colTopY <= other->colBottomY && colTopY + deltaPos.y > other->colBottomY)
                nextPos.y = other->colBottomY - (colBox.end().y * pTransform->dims().y);
        }
    }

    pTransform->pos() = nextPos;
}