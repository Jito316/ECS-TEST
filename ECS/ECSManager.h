#pragma once
#include <shared_mutex>
#include <optional>
#include <tuple>
#include <vector>

#include "ClassInfo.h"
#include "Entity.h"
#include "ComponentPool.h"

class ISystem;

class ECSManager
{
public:
	void excute();

	void CreateEntity(const Entity _entity) {}
	void DeleteEntity(const Entity _entity) {}

	template<class T>
	T* AddComponent(const Entity _index)
	{
		if (m_vComponentPools.size() > _index)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			auto pool = m_vComponentPools[ID];
			if (pool)
			{
				return static_cast<ComponentPool<T>*>(pool.get())->AddComponent(_index);
			}
		}

		return nullptr;
	}

	template<class T>
	void RemoveComponent(const Entity _index)
	{
		if (m_vComponentPools.size() > _index)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			auto pool = m_vComponentPools[ID];
			if (pool)
			{
				static_cast<ComponentPool<T>*>(pool.get())->RemoveComponent(_index);
			}
		}
	}

	template<class T>
	T* GetComponent(const Entity _index)
	{
		if (m_vComponentPools.size() > _index)
		{
			auto& ID = ComponentInfo::GetInstance<T>().m_id;
			auto pool = m_vComponentPools[ID];
			if (pool)
			{
				return static_cast<ComponentPool<T>*>(pool.get())->GetComponent(_index);
			}
		}
		return nullptr;
	}

	template<class... Ts>
	std::optional<std::tuple<Ts*...>> GetComponents(const Entity entity)
	{
		// 必要なコンポーネントのマスクを作成
		ComponentMask archetype = (0 | ... | ComponentInfo::GetInstance<Ts>().m_id);

		// エンティティが必要なコンポーネントを全て持っているか判定
		if ((m_vArchetypes[entity] & archetype).none())
		{
			return std::nullopt;
		}

		// それぞれのコンポーネントを取得
		std::tuple<Ts*...> result{ GetComponent<Ts>(entity)... };

		// nullptr が含まれていたら無効
		if (((std::get<Ts*>(result) != nullptr) && ...))
		{
			return result;
		}
		return std::nullopt;
	}

	const std::vector<Entity>& GetActiveEntitys() { return {}; }

private:
	mutable std::vector<ComponentMask> m_vArchetypes;

	std::vector<std::shared_ptr<IComponentPool>> m_vComponentPools;
	std::vector<std::shared_ptr<ISystem>> m_systems;

public:
	ECSManager() = default;
	~ECSManager() = default;
};