#pragma once
#include "src/ecs/System.h"
#include <queue>

namespace DXH
{
class GameObject;

class GameObjectCollector : System
{
    GameObjectCollector();
    ~GameObjectCollector();
public:
    static GameObjectCollector& GetInstance()
    {
        static GameObjectCollector instance;
        return instance;
    }

    void Update(const Timer& gt) override;
    void AddGameObject(GameObject* gameObject) { m_GameObjects.push_back(gameObject); }
    void RemoveGameObject(GameObject* gameObject) { m_GameObjectsToRemove.push(gameObject); }

private:
    std::vector<GameObject*> m_GameObjects = std::vector<GameObject*>();
    std::queue<GameObject*> m_GameObjectsToRemove = std::queue<GameObject*>();
};
}