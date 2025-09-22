#pragma once
#include <vector>
#include "Script/Utility/BinaryWriteHelper.h"
#include "Script/ECS/Entity/Entity.h"

class IComponentPool
{
public:
	virtual void BinaryWrite(const std::string& _name) = 0;
	virtual void BinaryRead(const std::string& _name, size_t _defaultSize) = 0;
	virtual void BinaryWrite(BinaryFileWriter& _writer) = 0;
	virtual void BinaryRead(BinaryFileReader& _reader, size_t _defaultSize) = 0;
};

template<class ComponentT>
class ComponentPool : public IComponentPool
{
public:
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

	void BinaryWrite(const std::string& _name)override
	{
		BinaryFileWriter writer(_name);
		writer.Write(m_vComponentPool);
	}
	void BinaryRead(const std::string& _name, size_t _defaultSize)override
	{
		BinaryFileReader reader(_name);
		if (!reader.Read(m_vComponentPool))
		{
			m_vComponentPool.resize(_defaultSize);
		}
	}

	void BinaryWrite(BinaryFileWriter& _writer)override
	{
		_writer.Write(m_vComponentPool);
	}
	void BinaryRead(BinaryFileReader& _reader, size_t _defaultSize)override 
	{
		if (!_reader.Read(m_vComponentPool))
		{
			m_vComponentPool.resize(_defaultSize);
		}
	}

private:
	std::vector<ComponentT> m_vComponentPool;
};
