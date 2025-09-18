#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#define MAXCOMPONENTS 100
#define MAXENTITIES 1000

using ComponentMask = std::bitset<MAXCOMPONENTS>;
using Entity = std::uint32_t;	// インデックス

struct EntityContainer
{
	EntityContainer()
		:m_vEntityToIndex(MAXENTITIES, -1)
	{

	}

	void Add(const Entity _entity);
	void Remove(const Entity _entity);

	void BinaryWrite(std::string_view _path);
	void BinaryRead(std::string_view _path);

	std::vector<Entity> m_vEntities;
	std::vector<size_t> m_vEntityToIndex;
};