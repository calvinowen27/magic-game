#include "../include/game/Button.hpp"
#include "../include/game/MouseHandler.hpp"
#include "../include/game/Game.hpp"

Button::Button(std::string textureName, std::string buttonText, std::string fontName, SDL_Color textColor, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)()) : 
TextElement(textureName, buttonText, fontName, textColor, relativePos, relativeDims, false, true)
{
    _pMouseHandler = _game.pMouseHandler;
    _actionFunc = actionFunc;
}

void Button::update()
{
    TextElement::update();

    // std::cout << _pMouseHandler->mouseWithinRect(_drawRect) << " " << _pMouseHandler->isButtonPressed(MOUSE_LEFT) << std::endl;

    if(_pMouseHandler->mouseWithinRect(_drawRect) && _pMouseHandler->isButtonJustPressed(MOUSE_LEFT))
    {
        _actionFunc();
    }
}