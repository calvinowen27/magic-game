#ifndef BUTTON_INCLUDE
#define BUTTON_INCLUDE

#include "UIElement.hpp"

class MouseHandler;

class Button : public UIElement
{
    private:
    MouseHandler &_mouseHandler;
    void (*_actionFunc)() = nullptr;

public:
    Button();
    bool init(std::string textureName, Vector2 relativePos, Vector2 relativeDims, void (*actionFunc)(), bool enabled = true);
    void update() override;
};

#endif