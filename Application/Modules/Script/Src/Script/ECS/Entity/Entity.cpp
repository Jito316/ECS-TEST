#include "Entity.h"
#include "Script/Utility/BinaryWriteHelper.h"

void EntityContainer::Add(const Entity _entity)
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

void EntityContainer::Remove(const Entity _entity)
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

void EntityContainer::BinaryWrite(std::string_view _path)
{
	BinaryFileWriter writer(_path.data());
	writer.Write(m_vEntities);
	writer.Write(m_vEntityToIndex);
}
void EntityContainer::BinaryRead(std::string_view _path)
{
	BinaryFileReader reader(_path.data());
	reader.Read(m_vEntities);
	reader.Read(m_vEntityToIndex);
}