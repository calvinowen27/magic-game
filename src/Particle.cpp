#include "../include/game/Particle.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ContentManager.hpp"

Particle::Particle(std::string type, Vector2 pos, Vector2 dims, float lifeDur)
{
    _pGame = Game::getInstance();
    _pContentManager = _pGame->pContentManager;
    _pos = pos;
    _pxPos = _pGame->worldToPixel(_pos);
    _dims = dims;
    _pxDims = (Vector2Int)(_dims * _pGame->ppm);

    _type = type;

    _pTexture = _pContentManager->getTextureFromType(_type);
}

Particle::~Particle()
{
    SDL_DestroyTexture(_pTexture);
}

void Particle::draw(SDL_Renderer *pRenderer)
{
    _spriteRect = SDL_Rect{_pxPos.x, _pxPos.y, _pxDims.x, _pxDims.y};

    SDL_RenderCopyEx(pRenderer, _pTexture, NULL, &_spriteRect, _textureAngle, NULL, _flipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void Particle::update(float time)
{
    _pxPos = _pGame->worldToPixel(_pos) - Vector2Int(0, _pxDims.y);
    _pxDims = (Vector2Int)(_dims * _pGame->ppm);
}