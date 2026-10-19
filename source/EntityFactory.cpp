#include "EntityFactory.h"

Entity::Entity EntityFactory::createRenderableEntityRecurse(const gfx::SceneNode& sceneNode, size_t parentIndex, bool hasParent)
{
    // Create new entityID
    Entity::Entity newEntity = m_entityManager->generateNewEntity();

    auto hierarchyPool = m_entityManager->getComponentPool<gfx::RenderHierarchy>();

    size_t currentIndex = 0;

    if (hierarchyPool != nullptr)
    {
        currentIndex = hierarchyPool->size();
    }

    m_entityManager->addComponentData<gfx::RenderHierarchy>(newEntity, gfx::RenderHierarchy{
        .parentIndex = parentIndex,
        .hasParent = hasParent
    });

    gfx::Transform transform = gfx::convertToTransform(sceneNode.transform);

    m_entityManager->addComponentData<gfx::Transform>(newEntity, std::move(transform));

    if (sceneNode.renderAssets.mesh.VAO != gfx::invalidHandler)
    {
        gfx::GpuHandles mesh = sceneNode.renderAssets.mesh; // Copy for hot loop

        m_entityManager->addComponentData<gfx::GpuHandles>(newEntity, std::move(mesh));
    }

    if (!sceneNode.renderAssets.materialKey.empty())
    {
        // Create material component
        gfx::Material nodeMaterial = m_materialManager->getElement(sceneNode.renderAssets.materialKey);
        m_entityManager->addComponentData<gfx::Material>(newEntity, std::move(nodeMaterial));
    }

    for (const auto& childNode : sceneNode.children)
    {
        createRenderableEntityRecurse(childNode, currentIndex, true);
    }

    return newEntity;
}

Entity::Entity EntityFactory::createRenderableEntity(const gfx::SceneNode& rootSceneNode)
{
    return createRenderableEntityRecurse(rootSceneNode, 0, false);
}

