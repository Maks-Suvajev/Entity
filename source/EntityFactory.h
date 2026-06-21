#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include "EntityManager.h"
#include "MaterialManager.h"
#include "SceneModelTypes.h"
#include "PhysicsTypes.h"



class EntityFactory
{
    public:
        EntityFactory(EntityManager* entityManager, gfx::MaterialManager* materialManager)
            : m_entityManager(entityManager),
              m_materialManager(materialManager)

        {
        };

        std::pair<Entity::Entity, Entity::Entity> createRenderableEntityRecurse(const gfx::SceneNode& sceneNode);
        Entity::Entity createRenderableEntity(const gfx::SceneNode& sceneNode);

        
    private:
        EntityManager* m_entityManager;
        gfx::MaterialManager* m_materialManager;


};

#endif