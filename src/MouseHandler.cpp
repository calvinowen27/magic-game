#include "../include/game/MouseHandler.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/Spells/Spell.hpp"
#include "../include/game/Player.hpp"
#include "../include/game/UI/UIManager.hpp"
#include "../include/game/Spells/SpellManager.hpp"

MouseHandler::MouseHandler() : _game(*Game::getInstance())
{
}

void MouseHandler::update()
{
    _prevButtonState = _currButtonState;
    _pxPrevMousePos = _pxMousePos;
    _currButtonState = SDL_GetMouseState(&_pxMousePos.x, &_pxMousePos.y);

    _prevMousePos = _mousePos;
    _mousePos = _game.pixelToWorld(_pxMousePos);
}

void MouseHandler::onMouseButtonDown(int button)
{

}

void MouseHandler::onMouseButtonUp(int button)
{
    if(button == SDL_BUTTON_LEFT)
    {
        if(!_game.pUIManager->isSpellUIEnabled() && _game.pSpellManager->getCurrSpell())
            _game.pSpellManager->getCurrSpell()->cast(_game.pPlayer->getPos() + (_game.pPlayer->getDims() / 2));
    }
}

bool MouseHandler::isButtonPressed(int button)
{
    return _currButtonState & SDL_BUTTON(button);
}

bool MouseHandler::wasButtonPressed(int button)
{
    return _prevButtonState & SDL_BUTTON(button);
}

bool MouseHandler::isButtonJustPressed(int button)
{
    return isButtonPressed(button) && !wasButtonPressed(button);
}

bool MouseHandler::isButtonJustReleased(int button)
{
    return !isButtonPressed(button) && wasButtonPressed(button);
}

void MouseHandler::mouseWheel(int wheelY)
{
    if(wheelY > 0)
        _game.zoomIn();

    if(wheelY < 0)
        _game.zoomOut();
}

bool MouseHandler::mouseWithinRect(SDL_Rect rect)
{
    return !(_pxMousePos.x < rect.x || _pxMousePos.x > rect.x + rect.w || _pxMousePos.y < rect.y || _pxMousePos.y > rect.y + rect.h);
}