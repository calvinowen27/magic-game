#include "../../include/game/ECS/Registry.hpp"

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
}

Component* Registry::newComponent(ComponentType type)
{
    switch(type)
    {
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