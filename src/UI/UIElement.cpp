#include "../../include/game/UI/UIElement.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/ContentManager.hpp"

UIElement::UIElement(std::string textureName, Vector2 relativePos, Vector2 relativeDims) : _game(*Game::getInstance()), _contentManager(*_game.pContentManager)
{
    _pUIManager = UIManager::getInstance();
    // _pGame = Game::getInstance();
    // _pContentManager = _pGame->pContentManager;

    _pTexture = _contentManager.getTexture(textureName);

    _relativePos = relativePos;
    _relativeDims = relativeDims;
    _pxPos = (Vector2Int)(_relativePos * Vector2(_game.winWidth, _game.winHeight));
    _pxDims = (Vector2Int)(_relativeDims * Vector2(_game.winWidth, _game.winHeight));

    _drawRect = SDL_Rect{_pxPos.x, _pxPos.y, _pxDims.x, _pxDims.y};

    _pUIManager->addElement(this);
}

UIElement::~UIElement()
{
    SDL_DestroyTexture(_pTexture);
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