#ifndef CONTENT_MANAGER_INCLUDE
#define CONTENT_MANAGER_INCLUDE

#include "../SDL2/SDL.h"
#include "../SDL2/SDL_image.h"
#include "../SDL2/SDL_ttf.h"
#include <map>
#include <string>

class Game;

class ContentManager
{
    private:
        const std::string TEXTURE_PATH = "./content/textures";
        const std::string FONT_PATH = "./content/fonts";

        Game *_pGame;
        std::map<std::string, SDL_Texture *> _textures;
        std::map<std::string, SDL_Texture *> _typeTextures;
        std::map<std::string, TTF_Font *> _fonts;

    public:
        void loadContent();
        void loadTextures();
        void loadFonts();
        void destroyContent();
        void destroyTextures();
        void closeFonts();
        SDL_Texture* getTexture(std::string textureName);
        SDL_Texture *getTextureFromType(std::string type);
        TTF_Font* getFont(std::string fontName);
        ContentManager();
        ~ContentManager();
};

#endif
