#include "../../include/game/ECS/Registry.hpp"
#include "../../include/game/WorldManager.hpp"

Registry::Registry()
{
}

Registry::~Registry()
{
    for (auto transform : _transforms)
    {
        delete transform;
    }

    for(auto collider : _colliders)
    {
        delete collider;
    }

    for(auto rigidbody : _rigidbodies)
    {
        delete rigidbody;
    }
}

void Registry::update(float time)
{
    for (auto transform : _transforms)
    {
        transform->update(time);
    }
}

void Registry::draw(SDL_Renderer *pRenderer)
{
    // y value render order
    for(float y = (WorldManager::WORLD_SIZE / 2) - 1; y > -(WorldManager::WORLD_SIZE / 2); y -= 0.5f)
    {
        for (auto renderer : _renderers)
        {
            if((float)((int)(renderer->pTransform->pos.y*4))/2.f == y)
                renderer->draw(pRenderer);
        }
    }
}

Component* Registry::newComponent(ComponentType type)
{
    switch(type)
    {
        case ComponentType::Renderer:
            _renderers.push_back(new RendererComponent());
            return _renderers.back();
        case ComponentType::Transform:
            _transforms.push_back(new TransformComponent());
            return _transforms.back();
        case ComponentType::Collider:
            _colliders.push_back(new ColliderComponent());
            return _colliders.back();
        case ComponentType::Rigidbody:
            _rigidbodies.push_back(new RigidbodyComponent());
            return _rigidbodies.back();
    }

    return nullptr;
}