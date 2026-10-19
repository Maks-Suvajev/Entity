#include "LevelManager.h"


void LevelManager::registerRootEntity(Entity::Entity rootEntity)
{
    m_rootEntities.push_back(rootEntity);
}

const std::vector<Entity::Entity>& LevelManager::getRootEntities() const
{
    return m_rootEntities;
}