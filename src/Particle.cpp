#include "../include/game/Particle.hpp"
#include "../include/game/Game.hpp"
#include "../include/game/ContentManager.hpp"
#include "../include/game/ECS/Registry.hpp"

Particle::Particle() : game(*Game::getInstance()), contentManager(*game.pContentManager), registry(*game.pRegistry)
{
}

bool Particle::init(string type, Vector2 pos, Vector2 dims, float lifeDur)
{
    enabled = true;

    pTransform = registry.newComponent<TransformComponent>();
    pTransform->init(pos, dims);

    pRenderer = registry.newComponent<RendererComponent>();
    pRenderer->init(type, pTransform);

    this->type = type;
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