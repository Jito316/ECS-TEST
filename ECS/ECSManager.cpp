#include "ECSManager.h"
#include "System.h"

void ECSManager::SetUp()
{
	auto& registry = ComponentInfo::GetRegistry();

	m_vComponentPools.clear();
	m_vComponentPools.resize(registry.size() + 1);
	for (auto& [key, data] : registry)
	{
		m_vComponentPools[data->m_id] = std::shared_ptr<IComponentPool>(data->m_creator());
	}

	m_vEntityToActive.reserve(10000);
	m_vEntityToArchetype.reserve(10000);
	m_sRecycleEntities.resize(1);
}

void ECSManager::excute()
{
	for (auto& system : m_vSystems)
	{
		system->PreUpdate();
	}
	for (auto& system : m_vSystems)
	{
		system->Update();
	}
	for (auto& system : m_vSystems)
	{
		system->PostUpdate();
	}
}

Entity ECSManager::CreateEntity()
{
	Entity newEntity;
	if (m_sRecycleEntities.size() == 1)
	{
		newEntity = ++m_sRecycleEntities.front();
	}
	else
	{
		newEntity = m_sRecycleEntities.back();
		m_sRecycleEntities.pop_back();
	}

	if (m_vEntityToActive.size() <= newEntity)
	{
		m_vEntityToActive.resize(newEntity + 1);
		m_vEntityToArchetype.resize(newEntity + 1);
	}

	m_entityContainer.Add(newEntity);
	m_vEntityToActive[newEntity] = true;
	m_vEntityToArchetype[newEntity] = ComponentMask();

	return newEntity;
}

void ECSManager::DeleteEntity(const Entity _entity)
{
	if (m_vEntityToActive.size() > _entity)
	{
		return;
	}

	m_entityContainer.Remove(_entity);
	m_vEntityToActive[_entity] = false;
	m_vEntityToArchetype[_entity] = ComponentMask();

	m_sRecycleEntities.push_back(_entity);
}

void ECSManager::AddSystemTarget(const Entity _entity, const ComponentMask& _id)
{
	for (auto& system : m_vSystems)
	{
		if ((_id & system->GetID()) == _id)
		{
			system->onCreateEntity(_entity);
		}
	}
}

void ECSManager::RemoveSystemTarget(const Entity _entity, const ComponentMask& _id)
{
	for (auto& system : m_vSystems)
	{
		if ((_id & system->GetID()) == _id)
		{
			system->onDestroyEntity(_entity);
		}
	}
}

void ECSManager::EntityContainer::Add(const Entity _entity)
{
	// すでに追加されていたら
	bool isOveerRange = m_vEntityToIndex.size() < _entity;
	if (!isOveerRange)
	{
		if (m_vEntityToIndex[_entity] != -1)
		{
			return;
		}
	}

	m_vEntities.emplace_back(_entity);

	if (isOveerRange)
	{
		m_vEntityToIndex.resize(_entity + 1);
	}
	m_vEntityToIndex[_entity] = m_vEntities.size() - 1;
}

void ECSManager::EntityContainer::Remove(const Entity _entity)
{
	if (m_vEntityToIndex.size() < _entity)
	{
		return;
	}

	size_t backIndex = m_vEntities.size() - 1;
	size_t backEntity = m_vEntities.back();
	size_t removeIndex = m_vEntityToIndex[_entity];

	// 削除する要素が最後の要素でなければ
	if (_entity != m_vEntities.back())
	{
		m_vEntities[removeIndex] = backEntity;
		m_vEntityToIndex[backIndex] = removeIndex;
	}

	// 最後尾のEntityを削除
	m_vEntities.pop_back();
}
