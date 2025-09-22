#include "ECSManager.h"
#include "Script/Utility/BinaryWriteHelper.h"
#include "Script/ECS/System/System.h"

void ECSManager::SetUp()
{
	//コンポーネントの確保
	auto& registry = ComponentInfo::GetRegistry();
	m_vComponentPools.clear();
	m_vComponentPools.resize(registry.size() + 1);
	for (auto& [key, data] : registry)
	{
		auto temp = std::shared_ptr<IComponentPool>(data->m_creator());
		m_vComponentPools[data->m_id] = temp;
	}

}
void ECSManager::Update()
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
	m_vEntityToActive.clear();
	m_vEntityToArchetype.clear();
	m_sRecycleEntities.clear();
	m_vComponentPools.clear();
}

bool ECSManager::Load(std::string_view _scenePath)
{
	BinaryFileReader reader(_scenePath.data());
	if (reader)
	{
		//Entityの読み込み
		reader.Read(m_vEntityToActive);
		reader.Read(m_vEntityToArchetype);
		reader.Read(m_sRecycleEntities);
		reader.Read(m_entityContainer.m_vEntities);
		reader.Read(m_entityContainer.m_vEntityToIndex);

		//コンポーネントの書き込み
		size_t poolID = 0;
		auto& registry = ComponentInfo::GetRegistry();
		for (size_t i = 0; i < m_vComponentPools.size(); ++i)
		{
			if (!reader.Read(&poolID, sizeof(poolID)))continue;
			auto it = registry.find(poolID);
			if (it == registry.end())continue;
			if (m_vComponentPools.size() > it->second->m_id)
			{
				m_vComponentPools[it->second->m_id]->BinaryRead(reader, MAXENTITIES);
			}
		}

		//システムの読み込み
		size_t length = 0;
		reader.Read(&length, sizeof(size_t));
		m_vSystems.clear();
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
		return true;
	}
	return false;
}

void ECSManager::Release()
{
	m_vSystems.clear();
}

void ECSManager::Release(std::string_view _scenePath)
{
	BinaryFileWriter writer(_scenePath.data());
	if (writer)
	{
		//Entityの書き込み
		writer.Write(m_vEntityToActive);
		writer.Write(m_vEntityToArchetype);
		writer.Write(m_sRecycleEntities);
		writer.Write(m_entityContainer.m_vEntities);
		writer.Write(m_entityContainer.m_vEntityToIndex);

		//コンポーネントの書き込み
		size_t poolID = 0;
		auto& registry = ComponentInfo::GetRegistry();
		for (size_t i = 0; i < m_vComponentPools.size(); ++i)
		{

			if (!writer.Write(&poolID, sizeof(poolID)))continue;
			auto it = registry.find(poolID);
			if (it == registry.end())continue;
			if (m_vComponentPools.size() > it->second->m_id)
			{
				m_vComponentPools[it->second->m_id]->BinaryWrite(writer);
			}
		}

		//システムの書き込み
		size_t length = m_vSystems.size();
		writer.Write(&length, sizeof(size_t));
		for (auto& system : m_vSystems)
		{
			size_t id = system->getID();
			auto& temp = system->GetTragetEntities();
			writer.Write(&id, sizeof(size_t));
			writer.Write(temp.m_vEntities);
			writer.Write(temp.m_vEntityToIndex);
		}
		m_vSystems.clear();
	}
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