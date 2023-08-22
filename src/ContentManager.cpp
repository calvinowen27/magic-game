#include "../include/game/ContentManager.hpp"
#include "../include/game/Game.hpp"
#include <filesystem>

ContentManager::ContentManager() : _game(*Game::getInstance())
{
    // _game = Game::getInstance();
}

ContentManager::~ContentManager()
{
    destroyContent();
}

void ContentManager::loadContent()
{
    loadTextures();
    loadFonts();
}

void ContentManager::loadTextures()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    _textures.insert({"", nullptr});

    SDL_Surface *pSurface;
    SDL_Texture *pTexture;

    for(const auto& dirEntry : recursive_directory_iterator(TEXTURE_PATH))
    {
        pSurface = IMG_Load(dirEntry.path().c_str());
        if(pSurface == NULL)
        {
            std::cerr << "ContentManager loadTextures(): Failed to load image to surface: " << IMG_GetError() << std::endl;
            return;
        }

        pTexture = SDL_CreateTextureFromSurface(_game.pRenderer, pSurface);
        if(pTexture == NULL)
        {
            std::cerr << "ContentManager loadTextures(): Failed to load texture from surface: " << IMG_GetError() << std::endl;
            return;
        }

        _textures.insert({dirEntry.path().filename().string(), pTexture});

        SDL_FreeSurface(pSurface);
    }

    _typeTextures["Wall"] = _textures["wall.png"];
    _typeTextures["Player"] = _textures["player.png"];
    _typeTextures["Enemy"] = _textures["enemy.png"];
    _typeTextures["Grass"] = _textures["grass.png"];
    _typeTextures["Spell"] = _textures["ice_spell.png"];
    _typeTextures["red_bar"] = _textures["red_bar.png"];
    _typeTextures["green_bar"] = _textures["green_bar.png"];
}

void ContentManager::loadFonts()
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    std::string currPath;
    TTF_Font *font;

    for(const auto& dirEntry : recursive_directory_iterator(FONT_PATH))
    {
        currPath = dirEntry.path().filename().string();

        font = TTF_OpenFont(dirEntry.path().c_str(), 51);
        if(font == NULL)
        {
            std::cerr << "ContentManager loadFonts(): Failed to open font: " << TTF_GetError() << std::endl;
            return;
        }

        _fonts.insert({currPath, font});
    }
}

void ContentManager::destroyContent()
{
    destroyTextures();
    closeFonts();
}

void ContentManager::destroyTextures()
{
    std::map<std::string, SDL_Texture *>::iterator it;
    for(it = _textures.begin(); it != _textures.end(); it++)
    {
        SDL_DestroyTexture(it->second);
    }
}

void ContentManager::closeFonts()
{
    std::map<std::string, TTF_Font *>::iterator it;
    for(it = _fonts.begin(); it != _fonts.end(); it++)
    {
        TTF_CloseFont(it->second);
    }
}

SDL_Texture *ContentManager::getTexture(std::string textureName)
{
    return _textures[textureName];
}

SDL_Texture *ContentManager::getTextureFromType(std::string type)
{
    return _typeTextures[type];
}

TTF_Font* ContentManager::getFont(std::string fontName)
{
    return _fonts[fontName];
}