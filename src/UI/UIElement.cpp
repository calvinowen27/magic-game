#include "../../include/game/UI/UIElement.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"

UIElement::UIElement() : _game(*Game::getInstance()), _contentManager(*_game.pContentManager), _uiManager(*_game.pUIManager)
{
}

bool UIElement::init(std::string textureName, Vector2 relativePos, Vector2 relativeDims, bool enabled)
{
    this->enabled = enabled;
    _pTexture = _contentManager.getTexture(textureName);

    _relativePos = relativePos;
    _relativeDims = relativeDims;
    _pxPos = (Vector2Int)(_relativePos * Vector2(_game.winWidth, _game.winHeight));
    _pxDims = (Vector2Int)(_relativeDims * Vector2(_game.winWidth, _game.winHeight));

    _drawRect = SDL_Rect{_pxPos.x, _pxPos.y, _pxDims.x, _pxDims.y};

    return true;
}

void UIElement::update()
{
    _pxPos = (Vector2Int)(_relativePos * Vector2(_game.winWidth, _game.winHeight));
    _pxDims = (Vector2Int)(_relativeDims * Vector2(_game.winWidth, _game.winHeight));

    _drawRect = SDL_Rect{_pxPos.x, _pxPos.y, _pxDims.x, _pxDims.y};
}

void UIElement::draw(SDL_Renderer *pRenderer)
{
    SDL_RenderCopy(pRenderer, _pTexture, NULL, &_drawRect);
}
