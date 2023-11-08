#include "GameObjectCollector.h"
#include "src/ecs/GameObject.h"
#include "src/time/Timer.h"

namespace DXH
{
GameObjectCollector::GameObjectCollector()
{
}

GameObjectCollector::~GameObjectCollector()
{
    while (!m_GameObjectsToRemove.empty())
    {
        GameObject* gameObject = m_GameObjectsToRemove.front();
        m_GameObjectsToRemove.pop();
        m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), gameObject), m_GameObjects.end());
    }

    for (auto& gameObject : m_GameObjects)
        delete gameObject;
}

void GameObjectCollector::Update(const Timer& gt)
{
    while (!m_GameObjectsToRemove.empty())
    {
        GameObject* gameObject = m_GameObjectsToRemove.front();
        m_GameObjectsToRemove.pop();
        m_GameObjects.erase(std::remove(m_GameObjects.begin(), m_GameObjects.end(), gameObject), m_GameObjects.end());

        // Call all the destroy callbacks
        for (auto& callback : gameObject->m_ReleaseCallbacks)
            callback(gameObject);
        gameObject->m_ReleaseCallbacks.clear();

    }
}
}
