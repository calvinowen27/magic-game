#ifndef UI_GROUP_INCLUDE
#define UI_GROUP_INCLUDE

#include "UIElement.hpp"
#include "Button.hpp"
#include "TextElement.hpp"
#include "TextButton.hpp"
#include "../Spells/SpellManager.hpp"

#include "../../SDL2/SDL.h"

#include <set>
#include <memory>
#include <queue>

class Game;
class SpellManager;

class UIGroup
{
protected:
    Game &game;

    std::set<std::shared_ptr<UIElement>> uiElements;
    bool enabled = true;

public:
    UIGroup();
    virtual void init();
    virtual void update();
    virtual void draw(SDL_Renderer *pRenderer);
    virtual void setEnabled(bool val);
    virtual bool toggleEnabled();
    inline bool isEnabled() { return enabled; }

    template <typename T>
    std::shared_ptr<T> newUIElement()
    {
        if (!std::is_base_of<UIElement, T>::value)
        {
            throw new std::invalid_argument("UIGroup::newUIElement<Type>() : Type must be derived from UIElement class.");
        }

        std::shared_ptr<T> element = std::make_shared<T>();
        uiElements.emplace(element);
        return element;
    }
};

class SpellUIGroup : public UIGroup
{
private:
    SpellManager &_spellManager;

    std::shared_ptr<Button> _pRadialSpellButton;
    std::shared_ptr<Button> _pProjectileSpellButton;
    std::shared_ptr<Button> _pTrailSpellButton;

public:
    SpellUIGroup();
    void init() override;
    bool toggleEnabled() override;

    static void radialButton();
    static void projectileButton();
    static void trailButton();
};

class DebugUIGroup : public UIGroup
{
private:
    std::shared_ptr<TextElement> _pFPScounter;
    std::shared_ptr<TextElement> _pUPScounter;
    std::shared_ptr<TextElement> _pPosDisplay;
    std::shared_ptr<TextElement> _pVelDisplay;
    std::shared_ptr<TextElement> _pAccDisplay;
    std::shared_ptr<TextElement> _pPPMDisplay;

public:
    DebugUIGroup();
    void init() override;
    void update() override;
};

class PauseUIGroup : public UIGroup
{
private:
    std::shared_ptr<TextButton> _pResumeButton;
    std::shared_ptr<TextButton> _pQuitButton;

public:
    PauseUIGroup();
    void init() override;
};

#endif