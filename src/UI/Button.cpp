#include "../../include/game/UI/Button.hpp"
#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Game.hpp"

Button::Button() : UIElement(), _mouseHandler(*_game.pMouseHandler)
{
}

bool Button::init(std::string textureName, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)(), bool enabled)
{
    UIElement::init(textureName, relativePos, relativeDims, enabled);

    _actionFunc = actionFunc;
}

void Button::update()
{
    UIElement::update();

    if(_mouseHandler.mouseWithinRect(_drawRect) && _mouseHandler.isButtonJustPressed(MOUSE_LEFT))
    {
        _actionFunc();
    }
}