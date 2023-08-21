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

    pCollider = registry.newComponent<ColliderComponent>();
    pRigidbody = registry.newComponent<RigidbodyComponent>();

    pCollider->init(Vector2::zero, Vector2(1, 0.5), pTransform, pRigidbody);
    pRigidbody->init(pTransform, pCollider);

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
    registry.killComponent(pRigidbody);
    registry.killComponent(pCollider);
}