#include "../../include/game/UI/TextButton.hpp"
#include "../../include/game/Input/MouseHandler.hpp"
#include "../../include/game/Game.hpp"

TextButton::TextButton() : TextElement(), _mouseHandler(*_game.pMouseHandler)
{
}

bool TextButton::init(std::string textureName, std::string buttonText, std::string fontName, SDL_Color textColor, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)(), bool enabled)
{
    TextElement::init(textureName, buttonText, fontName, textColor, relativePos, relativeDims, false, true, enabled);

    _actionFunc = actionFunc;
}

void TextButton::update()
{
    TextElement::update();

    if(_mouseHandler.mouseWithinRect(_drawRect) && _mouseHandler.isButtonJustPressed(MOUSE_LEFT))
    {
        _actionFunc();
    }
}