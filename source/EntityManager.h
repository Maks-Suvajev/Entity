#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

// STL
#include <stdint.h>
#include <vector> 
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "ComponentManager.h"

#include "Entity.h"

class EntityManager
{
    public:
        EntityManager();
        Entity getNewID();
        void deleteEntity(Entity entity);
        Entity generateNewEntity();

        void printActiveEntityIDs();
        void printActiveEntityComponents(Entity entity);

        template<typename T>
        bool addComponentData(Entity entity, T&& componentData);

        template<typename T>
        T* getPoolElement(Entity entity);

        template<typename T>
        ComponentManager<T>* getComponentPool();

    private:
        void resizeSparse(Entity entity);
        uint32_t nextID;
        std::vector<Entity> recyclingBucket;
        std::vector<Entity> activeIDs;
        std::vector<Entity> sparse;
        std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> componentPools;
};

template<typename T>
bool EntityManager::addComponentData(Entity entity, T&& componentData)
{
    auto index = std::type_index(typeid(T));

    if (!componentPools.contains(index))
    {
        componentPools[index] = std::make_unique<ComponentManager<T>>();
    }

    return static_cast<ComponentManager<T>*>(componentPools[index].get())->addComponent(entity, std::move(componentData));
}

template<typename T>
T* EntityManager::getPoolElement(Entity entity)
{
    auto pool = getComponentPool<T>();

    if (pool == nullptr)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::RenderSystem::getPoolElement::No pool for type found: " << typeid(T).name() << std::endl;
        #endif
    }

    auto component = pool->getComponentData(entity);

    if (component == nullptr)
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::RenderSystem::getPoolElement::Entity component has no value for some reason." << std::endl;
        #endif

        return nullptr;        
    }
    
    return component;
}

template<typename T>
ComponentManager<T>* EntityManager::getComponentPool()
{
    auto index = std::type_index(typeid(T));

    if (!componentPools.contains(index))
    {
        #ifdef ENABLE_DEBUG_MESSAGES
            std::cout << "ERROR::EntityManager::Component requested doesn't exist and has no pool." << std::endl;
        #endif

        return nullptr;
    }

    return static_cast<ComponentManager<T>*>(componentPools[index].get());
}


#endif