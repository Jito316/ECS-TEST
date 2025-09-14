#pragma once
#include <vector>
class IComponentPool
{
};

template<class ComponentT>
class ComponentPool : public IComponentPool
{
public:
	ComponentPool()
		:m_vComponentPool(10000)
	{

	}

	ComponentT* AddComponent(const Entity _index)
	{
		if (m_vComponentPool.size() <= _index)
		{
			m_vComponentPool.resize(_index, ComponentT());
		}
		m_vComponentPool[_index] = ComponentT();

		return &m_vComponentPool[_index];
	}

	void RemoveComponent(const Entity _index)
	{
		if (m_vComponentPool.size() <= _index)
		{
			m_vComponentPool.resize(_index, ComponentT());
		}
		m_vComponentPool[_index] = ComponentT();
	}

	ComponentT* GetComponent(const Entity _index)
	{
		if (_index < m_vComponentPool.size())
		{
			return &m_vComponentPool[_index];
		}

		return nullptr;
	}

private:
	std::vector<ComponentT>m_vComponentPool;
};
