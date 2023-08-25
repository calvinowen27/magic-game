#ifndef UIELEMENT_INCLUDE
#define UIELEMENT_INCLUDE

#include <string>

#include "../../SDL2/SDL.h"
#include "../../SDL2/SDL_image.h"
#include "../../SDL2/SDL_ttf.h"

#include "../Vector2.hpp"

class Game;
class UIManager;
class ContentManager;

class UIElement
{
    protected:
        Game &_game;
        UIManager &_uiManager;
        ContentManager &_contentManager;
        
        Vector2 _relativePos;
        Vector2 _relativeDims;
        Vector2Int _pxPos;
        Vector2Int _pxDims;

        SDL_Texture *_pTexture;
        SDL_Rect _drawRect;

    public:
        UIElement();
        ~UIElement();
        bool init(std::string textureName, Vector2 relativePos, Vector2 relativeDims);
        virtual void draw(SDL_Renderer* pRenderer);
        virtual void update();
};

#endif