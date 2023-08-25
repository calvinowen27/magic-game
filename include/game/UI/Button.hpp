#ifndef BUTTON_INCLUDE
#define BUTTON_INCLUDE

#include "TextElement.hpp"

class MouseHandler;

class Button : public TextElement
{
private:
    MouseHandler &_mouseHandler;
    void (*_actionFunc)();

public:
    Button();
    bool init(std::string textureName, std::string buttonText, std::string fontName, SDL_Color textColor, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)());
    void update() override;
};

#endif