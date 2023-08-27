#ifndef UI_MANAGER_INCLUDE
#define UI_MANAGER_INCLUDE

#define WHITE \
    SDL_Color { 255, 255, 255, 255 }
#define BLACK \
    SDL_Color { 0, 0, 0, 255 }
#define GRAY \
    SDL_Color { 127, 127, 127, 255 }
#define RED \
    SDL_Color { 255, 0, 0, 255 }
#define GREEN \
    SDL_Color { 0, 255, 0, 255 }
#define BLUE \
    SDL_Color { 0, 0, 255, 255 }

#include "UIElement.hpp"
#include "TextElement.hpp"
#include "TextButton.hpp"
#include "../Spells/SpellManager.hpp"

#include <vector>
#include <set>
#include <memory>
#include <type_traits>

class Player;

class UIManager
{
private:
    static UIManager *_pInstance;

    Game &_game;

    std::set<std::shared_ptr<UIElement>> _uiElements;

    std::set<std::shared_ptr<UIElement>> _spellUI;
    bool _spellUIEnabled = false;

    std::shared_ptr<TextElement> _pFPScounter;
    std::shared_ptr<TextElement> _pUPScounter;
    std::shared_ptr<TextElement> _pPosDisplay;
    std::shared_ptr<TextElement> _pVelDisplay;
    std::shared_ptr<TextElement> _pAccDisplay;
    std::shared_ptr<TextElement> _pPPMDisplay;

public:
    UIManager();
    static UIManager *getInstance();
    void init();
    void update();
    void draw(SDL_Renderer *pRenderer);

    inline bool toggleSpellUI()
    {
        _spellUIEnabled = !_spellUIEnabled;

        for(auto el : _spellUI)
            el->setEnabled(_spellUIEnabled);

        if(_spellUIEnabled)
        _game.pSpellManager->killCurrSpell();

        return _spellUIEnabled;
    }

    inline bool isSpellUIEnabled() { return _spellUIEnabled; }

    template <typename T>
    std::shared_ptr<T> newUIElement()
    {
        if (!std::is_base_of<UIElement, T>::value)
        {
            throw new std::invalid_argument("UIManager::newUIElement<Type>() : Type must be derived from UIElement class.");
        }

        std::shared_ptr<T> element = std::make_shared<T>();
        _uiElements.emplace(element);
        return element;
    }
};

#endif