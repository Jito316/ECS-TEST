#include "ECSManager.h"
#include "BinaryWriteHelper.h"
#include "System.h"

void ECSManager::SetUp()
{
	{
		BinaryFileReader reader("ECSManager.dat");
		if (!reader.Read(m_vEntityToActive))m_vEntityToActive.resize(MAXENTITIES);
		if (!reader.Read(m_vEntityToArchetype))m_vEntityToArchetype.resize(MAXENTITIES);
		if (!reader.Read(m_sRecycleEntities))m_sRecycleEntities.resize(1);
		reader.Read(m_entityContainer.m_vEntities);
		reader.Read(m_entityContainer.m_vEntityToIndex);
	}

	{
		BinaryFileReader reader("System.dat");
		size_t length = 0;
		reader.Read(&length, sizeof(size_t));
		for (size_t i = 0; i < length; i++)
		{
			size_t hash = 0;
			reader.Read(&hash, sizeof(size_t));
			if (const SystemInfo::Data* data; (data = SystemInfo::FindData(hash)))
			{
				auto system = std::shared_ptr<ISystem>(data->m_creator());
				system->setID(data->m_hash);
				system->setOwner(this);

				auto& temp = system->GetTragetEntities();
				reader.Read(temp.m_vEntities);
				reader.Read(temp.m_vEntityToIndex);

				m_vSystems.push_back(system);
			}
		}
	}

	auto& registry = ComponentInfo::GetRegistry();
	m_vComponentPools.clear();
	m_vComponentPools.resize(registry.size() + 1);
	for (auto& [key, data] : registry)
	{
		auto temp = std::shared_ptr<IComponentPool>(data->m_creator());
		temp->BinaryRead(data->m_name + ".dat", MAXENTITIES);
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
		BinaryFileWriter writer("ECSManager.dat");
		writer.Write(m_vEntityToActive);
		writer.Write(m_vEntityToArchetype);
		writer.Write(m_sRecycleEntities);
		writer.Write(m_entityContainer.m_vEntities);
		writer.Write(m_entityContainer.m_vEntityToIndex);
	}

	{
		BinaryFileWriter writer("System.dat");
		size_t length = m_vSystems.size();
		writer.Write(&length, sizeof(size_t));
		for (auto& system : m_vSystems) 
		{
			size_t id = system->getID();
			auto& temp =  system->GetTragetEntities();
			writer.Write(&id, sizeof(size_t));
			writer.Write(temp.m_vEntities);
			writer.Write(temp.m_vEntityToIndex);
		}
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
		ComponentMask archetype = system->GetArchetype();
		if (archetype == (_id & archetype))
		{
			system->onCreateEntity(_entity);
		}
	}
}

void ECSManager::RemoveSystemTarget(const Entity _entity, const ComponentMask& _id)
{
	for (auto& system : m_vSystems)
	{
		ComponentMask archetype = system->GetArchetype();
		if (archetype == (_id & archetype))
		{
			system->onDestroyEntity(_entity);
		}
	}
}