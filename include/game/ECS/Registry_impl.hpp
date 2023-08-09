#ifndef REGISTRY_IMPL_INCLUDE
#define REGISTRY_IMPL_INCLUDE

#include "Registry.hpp"

#include <type_traits>

using std::is_base_of;
using std::make_shared;

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
    if (_pInstance == nullptr)
        getInstance();

    return _pInstance->_vector;
}

template <typename T>
void TypeVector<T>::push_back(const shared_ptr<T> ptr)
{
    if (_pInstance == nullptr)
        getInstance();

    getVector().push_back(ptr);
}

template <typename T>
shared_ptr<T> TypeVector<T>::back()
{
    if (_pInstance == nullptr)
        getInstance();

    return getVector().back();
}

template <typename T>
shared_ptr<T> Registry::newComponent()
{
    // do not use component creation function if type is not a component
    if (!is_base_of<Component, T>::value)
    {
        throw new std::invalid_argument("Registry::newComponent<Type>() : Type must be derived from Component class.");
    }

    auto vector_ = TypeVector<T>::getInstance();
    vector_->push_back(make_shared<T>());
    return vector_->back();
}

template <typename T>
vector<shared_ptr<T>> &Registry::getComponents()
{
    return TypeVector<T>::getVector();
}

#endif