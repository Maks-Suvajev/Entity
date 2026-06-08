#include <iostream>
#include "EntityManager.h"

EntityManager::EntityManager()
: nextID(0U)
{
}

void EntityManager::resizeSparse(Entity::Entity entity)
{
    // Double the size of the sparse vector unless entityID is higher
    size_t newSize = std::max(static_cast<size_t>(entity + 1), static_cast<size_t>(sparse.size()) * 2);

    sparse.resize(newSize, Entity::MaxEntityValue);
}


void EntityManager::printActiveEntityComponents(Entity::Entity entity)
{
    uint32_t totalComponents = 0U;

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "| The entity with ID " << entity << " components:  |" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    for (auto& [id, pool] : componentPools)
    {
        if (pool->hasEntity(entity))
        {
            ++totalComponents;
            std::cout << id.name() << std::endl;
        }
    }

    std::cout << "------------------------------------------------" << std::endl;
}

Entity::Entity EntityManager::generateNewEntity()
{
    Entity::Entity newID = getNewID();

    activeIDs.push_back(newID);

    if (newID >= sparse.size())
    {
        resizeSparse(newID);
    }

    sparse[newID] = static_cast<Entity::Entity>(activeIDs.size() - 1);

    return newID;
}

Entity::Entity EntityManager::getNewID()
{
    if (recyclingBucket.empty())
    {
        return nextID++;
    }

    Entity::Entity newID = recyclingBucket.back();
    recyclingBucket.pop_back();

    return newID;
}

void EntityManager::deleteEntity(Entity::Entity entity)
{
    Entity::Entity position = sparse[entity]; // get position
    Entity::Entity backValue = activeIDs.back(); // get back value

    activeIDs[position] = backValue; // Move back data into position of deleted data
    sparse[backValue] = position;

    activeIDs.pop_back(); // remove from back ID

    sparse[entity] = Entity::MaxEntityValue;

    recyclingBucket.push_back(entity); 

    // Delete it in all pools
    for (auto& [key, componentPool] : componentPools)
    {
        componentPool->destroyEntity(entity);
    }
}