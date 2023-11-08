#include "../../include/game/Entity/Particle.hpp"
#include "../../include/game/Game.hpp"
#include "../../include/game/Animation/ContentManager.hpp"
#include "../../include/game/Entity/Registry.hpp"

Particle::Particle() : game(*Game::getInstance()), contentManager(*game.pContentManager), registry(*game.pRegistry)
{
}

bool Particle::init(std::string type, float lifeDur)
{
    enabled = true;

    pTransform = registry.newComponent<TransformComponent>();
    pTransform->init(Vector2::zero);

    pRenderer = registry.newComponent<RendererComponent>();
    pRenderer->init(type, pTransform);
    pRenderer->refreshDimensions();

    this->type = type;

    return true;
}

void Particle::update(float time)
{
}

void Particle::disable()
{
    enabled = false;
    pTransform->disable();
    pRenderer->disable();
}

void Particle::kill()
{
    registry.killComponent(pTransform);
    registry.killComponent(pRenderer);
}