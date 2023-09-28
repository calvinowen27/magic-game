#include "../../include/game/UI/UIGroup.hpp"
#include "../../include/game/Spells/SpellManager.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/UI/UIManager.hpp"
#include "../../include/game/Player.hpp"

/* UI GROUP */
UIGroup::UIGroup() : game(*Game::getInstance())
{
}

void UIGroup::init()
{
}

void UIGroup::update()
{
    for (auto el : uiElements)
        el->update();
}

void UIGroup::draw(SDL_Renderer *pRenderer)
{
    for (auto el : uiElements)
        el->draw(pRenderer);
}

void UIGroup::setEnabled(bool val)
{
    enabled = val;

    for (auto el : uiElements)
        el->setEnabled(val);
}

bool UIGroup::toggleEnabled()
{
    setEnabled(!enabled);
    return enabled;
}

/* SPELL UI GROUP */
SpellUIGroup::SpellUIGroup() : UIGroup(), _spellManager(*game.pSpellManager)
{
}

void SpellUIGroup::init()
{
    _pRadialSpellButton = newUIElement<Button>();
    _pRadialSpellButton->init("radial_spell_button", Vector2(0.5f, 0.25f), Vector2(0.05, 0.075), SpellUIGroup::radialButton);

    _pProjectileSpellButton = newUIElement<Button>();
    _pProjectileSpellButton->init("projectile_spell_button", Vector2(0.55f, 0.3f), Vector2(0.05, 0.075), SpellUIGroup::projectileButton);

    UIGroup::init();

    setEnabled(false);
}

void SpellUIGroup::update()
{
    UIGroup::update();

    // createSpell();
}

bool SpellUIGroup::toggleEnabled()
{
    UIGroup::toggleEnabled();

    if (enabled && _spellManager.hasValidSpell())
    {
        if (_spellManager.hasValidSpell())
            _spellManager.killCurrSpell();
        else
            _spellManager.resetCurrSpell();
    }

    return enabled;
}

void SpellUIGroup::radialButton()
{
    Game::getInstance()->pSpellManager->addSpellAttribute(SpellAttribute::Radial);
}

void SpellUIGroup::projectileButton()
{
    Game::getInstance()->pSpellManager->addSpellAttribute(SpellAttribute::Projectile);
}

/* DEBUG UI GROUP */
DebugUIGroup::DebugUIGroup() : UIGroup()
{
}

void DebugUIGroup::init()
{
    _pFPScounter = newUIElement<TextElement>();
    _pFPScounter->init("ui_text_back", "FPS: ", "arial", WHITE, Vector2::zero, Vector2(0.1, 0.025));

    _pUPScounter = newUIElement<TextElement>();
    _pUPScounter->init("ui_text_back", "UPS: ", "arial", WHITE, Vector2(0, 0.026), Vector2(0.1, 0.025));

    _pPPMDisplay = newUIElement<TextElement>();
    _pPPMDisplay->init("ui_text_back", "PPM: ", "arial", WHITE, Vector2(0, 0.052), Vector2(0.1, 0.025));

    _pPosDisplay = newUIElement<TextElement>();
    _pPosDisplay->init("ui_text_back", "Position: ", "arial", WHITE, Vector2(0.7, 0), Vector2(0.3, 0.025));

    _pVelDisplay = newUIElement<TextElement>();
    _pVelDisplay->init("ui_text_back", "Velocity: ", "arial", WHITE, Vector2(0.7, 0.026), Vector2(0.3, 0.025));

    UIGroup::init();

    setEnabled(true);
}

void DebugUIGroup::update()
{
    _pFPScounter->setText("FPS: " + std::to_string(game.getFPS()));
    _pUPScounter->setText("UPS: " + std::to_string(game.getUPS()));
    _pPPMDisplay->setText("PPM: " + std::to_string(game.ppm));
    _pPosDisplay->setText("Position: " + game.pPlayer->getPos().round(3).to_string());
    _pVelDisplay->setText("Velocity: " + game.pPlayer->getVelocity().round(3).to_string());

    UIGroup::update();
}