#include "ECSManager.h"
#include "BinaryWriteHelper.h"
#include "System.h"

void ECSManager::SetUp()
{
	{
		BinaryFileVecotrReader reader("ECSManager.dat");
		if (!reader.Read(m_vEntityToActive))m_vEntityToActive.resize(MAXENTITIES);
		if (!reader.Read(m_vEntityToArchetype))m_vEntityToArchetype.resize(MAXENTITIES);
		if (!reader.Read(m_sRecycleEntities))m_sRecycleEntities.resize(1);
	}

	auto& registry = ComponentInfo::GetRegistry();
	m_vComponentPools.clear();
	m_vComponentPools.resize(registry.size() + 1);
	for (auto& [key, data] : registry)
	{
		auto temp = std::shared_ptr<IComponentPool>(data->m_creator());
		temp->BinaryRead(data->m_name +".dat", MAXENTITIES);
		m_vComponentPools[data->m_id] = temp;
	}

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

void ECSManager::Shutdown()
{
	{
		BinaryFileVecotrWriter reader("ECSManager.dat");
		reader.Write(m_vEntityToActive);
		reader.Write(m_vEntityToArchetype);
		reader.Write(m_sRecycleEntities);
	}

	auto& registry = ComponentInfo::GetRegistry();
	for (auto& [key, data] : registry)
	{
		m_vComponentPools[data->m_id]->BinaryWrite(data->m_name + ".dat");
	}

	m_vEntityToActive.clear();
	m_vEntityToArchetype.clear();
	m_sRecycleEntities.clear();
	m_vComponentPools.clear();
}

Entity ECSManager::CreateEntity()
{
	Entity newEntity;
	if (m_sRecycleEntities.size() == 1)
	{
		newEntity = m_sRecycleEntities.front();
		++m_sRecycleEntities.front();
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
	Entity backEntity = m_vEntities.back();
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
