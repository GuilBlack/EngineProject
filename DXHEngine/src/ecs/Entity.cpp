#include "Entity.h"

namespace DXH
{
Entity::Entity(Id* componentIds, uint32_t componentCount)
	: m_id(GetNewId())
{
	m_components.reserve(componentCount);
	for (uint32_t i = 0; i < componentCount; ++i)
	{
		m_components[componentIds[i]] = nullptr;
	}
}

template<ComponentChild C>
inline bool Entity::TryGetComponent(C* component) const
{
	auto it = m_components.find(Component::GetId<C>());
	if (it != m_components.end())
	{
		component = it->second;
		return true;
	}
	component = nullptr;
	return false;
}
}
