#ifndef REGISTRY_IMPL_INCLUDE
#define REGISTRY_IMPL_INCLUDE

#include "Registry.hpp"

#include <type_traits>

using std::is_base_of;
using std::make_shared;

/* COMPONENT VECTOR */
template <typename T>
shared_ptr<TypeVector<T>> TypeVector<T>::_pInstance;

template <typename T>
shared_ptr<TypeVector<T>> TypeVector<T>::getInstance()
{
    if (_pInstance == nullptr)
    {
        _pInstance = make_shared<TypeVector<T>>();
    }

    return _pInstance;
}

template <typename T>
vector<shared_ptr<T>> &TypeVector<T>::getVector()
{
    return _vector;
}

template <typename T>
void TypeVector<T>::push_back(const shared_ptr<T> ptr)
{
    _vector.push_back(ptr);
}

template <typename T>
shared_ptr<T> TypeVector<T>::back()
{
    return _vector.back();
}

/* COMPONENT QUEUE */
template <typename T>
shared_ptr<TypeQueue<T>> TypeQueue<T>::_pInstance;

template <typename T>
shared_ptr<TypeQueue<T>> TypeQueue<T>::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = make_shared<TypeQueue<T>>();
    }

    return _pInstance;
}

template <typename T>
queue<shared_ptr<T>> &TypeQueue<T>::getQueue()
{
    return _queue;
}

template <typename T>
bool TypeQueue<T>::isEmpty()
{
    return _queue.empty();
}

template <typename T>
shared_ptr<T> TypeQueue<T>::pop()
{
    auto el = _queue.front();
    _queue.pop();
    return el;
}

template <typename T>
void TypeQueue<T>::push(shared_ptr<T> component)
{
    _queue.push(component);
}

/* COMPONENT SET */
template <typename T>
shared_ptr<TypeSet<T>> TypeSet<T>::_pInstance;

template <typename T>
shared_ptr<TypeSet<T>> TypeSet<T>::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = make_shared<TypeSet<T>>();
    }

    return _pInstance;
}

template <typename T>
set<shared_ptr<T>> &TypeSet<T>::getSet()
{
    return _set;
}

template <typename T>
void TypeSet<T>::emplace(shared_ptr<T> component)
{
    _set.emplace(component);
}

template <typename T>
size_t TypeSet<T>::erase(shared_ptr<T> component)
{
    return _set.erase(component);
}

/* COMPONENT POOL */
template <typename T>
shared_ptr<TypePool<T>> TypePool<T>::_pInstance;

template <typename T>
TypePool<T>::TypePool()
{
    expandPool(10);
}

template <typename T>
TypePool<T>::TypePool(int size)
{
    expandPool(size);
}

template <typename T>
shared_ptr<TypePool<T>> TypePool<T>::getInstance()
{
    if(_pInstance == nullptr)
    {
        _pInstance = make_shared<TypePool<T>>();
    }

    return _pInstance;
}

template <typename T>
shared_ptr<T> TypePool<T>::instantiate()
{
    if(!_pool.isEmpty())
    {
        shared_ptr<T> component = _pool.pop();
        _alive.emplace(component);
        return component;
    }
    else
    {
        expandPool(1);
        return instantiate();
    }
}

template <typename T>
bool TypePool<T>::release(shared_ptr<T> component)
{
    if(_alive.erase(component) > 0)
    {
        _pool.push(component);
        return true;
    }
    return false;
}

template <typename T>
void TypePool<T>::expandPool(int amount)
{
    for(int i = 0; i < amount; i++)
        _pool.push(make_shared<T>());
}

template <typename T>
set<shared_ptr<T>> &TypePool<T>::getAlive()
{
    return _alive.getSet();
}

template <typename T>
queue<shared_ptr<T>> &TypePool<T>::getPooled()
{
    return _pool.getQueue();
}

/* REGISTRY */
template <typename T>
shared_ptr<T> Registry::newComponent()
{
    // do not use component creation function if type is not a component
    if (!is_base_of<Component, T>::value)
    {
        throw new std::invalid_argument("Registry::newComponent<Type>() : Type must be derived from Component class.");
    }

    auto pool = TypePool<T>::getInstance();
    return pool->instantiate();
}

template <typename T>
void Registry::killComponent(shared_ptr<T> component)
{
    auto pool = TypePool<T>::getInstance();
    pool->release(component);
}

template <typename T>
set<shared_ptr<T>> &Registry::getComponents()
{
    return TypePool<T>::getInstance()->getAlive();
}

#endif