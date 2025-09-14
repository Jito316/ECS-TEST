#pragma once
#include <shared_mutex>
#include <vector>
#include <list>
#include <unordered_map>

#include "ClassInfo.h"
#include "Entity.h"
#include "ComponentPool.h"

class ISystem;

class ECSManager
{
public:
	struct EntityContainer
	{
		EntityContainer()
			:m_vEntityToIndex(10000, -1)
		{

		}

		void Add(const Entity _entity);
		void Remove(const Entity _entity);

		std::vector<Entity> m_vEntities;
		std::vector<size_t> m_vEntityToIndex;
	};

	void SetUp();
	void excute();

	Entity CreateEntity();
	void DeleteEntity(const Entity _entity);

	template<class T>
	T* AddComponent(const Entity _entity)
	{
		if (m_vComponentPools.size() > _entity)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			auto pool = m_vComponentPools[ID];
			if (pool)
			{
				m_vEntityToArchetype[_entity] |= 1 << ID;
				AddSystemTarget(_entity, m_vEntityToArchetype[_entity]);
				return static_cast<ComponentPool<T>*>(pool.get())->AddComponent(_entity);
			}
		}

		return nullptr;
	}

	template<class T>
	void RemoveComponent(const Entity _entity)
	{
		if (m_vComponentPools.size() > _entity)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			auto pool = m_vComponentPools[ID];
			if (pool)
			{
				m_vEntityToArchetype[_entity] &= ~(1 << ID);
				RemoveSystemTarget(_entity, m_vEntityToArchetype[_entity]);
				static_cast<ComponentPool<T>*>(pool.get())->RemoveComponent(_entity);
			}
		}
	}

	template<class T>
	T* GetComponent(const Entity _entity)
	{
		if (m_vComponentPools.size() > _entity)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			if (m_vEntityToArchetype[_entity].test(ID))
			{
				return static_cast<ComponentPool<T>*>(m_vComponentPools[ID].get())->GetComponent(_entity);
			}
		}
		return nullptr;
	}

	template<class T>
	void AddSystem()
	{
		m_vSystems.push_back(std::make_shared<T>(this));
	}

	const std::vector<Entity>& GetEntity() { return m_entityContainer.m_vEntities; }

private:
	void AddSystemTarget(const Entity _entity, const ComponentMask& _id);
	void RemoveSystemTarget(const Entity _entity, const ComponentMask& _id);

	//エンティティの状態管理
	EntityContainer m_entityContainer;
	std::vector<bool> m_vEntityToActive;
	std::vector<ComponentMask> m_vEntityToArchetype;
	std::list<Entity> m_sRecycleEntities;

	//コンポーネント管理
	std::vector<std::shared_ptr<IComponentPool>> m_vComponentPools;

	//システム管理
	std::vector<std::shared_ptr<ISystem>> m_vSystems;
	std::unordered_map<ComponentMask, size_t> m_umSystemToID;


public:
	ECSManager() = default;
	~ECSManager() = default;
};