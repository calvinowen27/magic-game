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
#include "UIGroup.hpp"

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

    std::set<std::shared_ptr<UIGroup>> _uiGroups;

    std::shared_ptr<SpellUIGroup> _pSpellUI;
    std::shared_ptr<DebugUIGroup> _pDebugUI;

public:
    UIManager();
    static UIManager *getInstance();
    void init();
    void update();
    void draw(SDL_Renderer *pRenderer);

    template <typename T>
    std::shared_ptr<T> newUIGroup()
    {
        if (!std::is_base_of<UIGroup, T>::value)
        {
            throw new std::invalid_argument("UIManager::newUIGroup<Type>() : Type must be derived from UIGroup class.");
        }

        std::shared_ptr<T> group = std::make_shared<T>();
        _uiGroups.emplace(group);
        return group;
    }

    inline std::shared_ptr<SpellUIGroup> getSpellUI() { return _pSpellUI; }
    inline std::shared_ptr<DebugUIGroup> getDebugUI() { return _pDebugUI; }
};

#endif