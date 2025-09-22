#pragma once
#include <string>
#include <functional>
#include <unordered_map>

#include "Script/ECS/Component/ComponentPool.h"	 
class ISystem;

struct ComponentInfo
{
public:
	struct Data
	{
		std::string m_name;
		size_t m_hash = 0;
		size_t m_id = 0;
		size_t m_size = 0;
		std::function<IComponentPool* ()> m_creator;
	};

	template<class T>
	static const Data& GetInstance()
	{
		static Data data;
		return data;
	}

	template<class T>
	static void Register(std::string_view _name)
	{
		Data& data = *const_cast<Data*>(&GetInstance<T>());
		{
			data.m_name = _name.data();
			data.m_hash = std::hash<std::string>()(data.m_name);
			data.m_id	= m_nextID;
			data.m_size = sizeof(T);
			data.m_creator = []() { return new ComponentPool<T>(); };
			++m_nextID;
		};

		m_registry[data.m_hash] = &data;
		m_nameToID[data.m_name] = data.m_hash;
	}

	static const std::unordered_map<size_t, ComponentInfo::Data*>& GetRegistry() { return m_registry; }

private:
	static std::unordered_map<size_t, ComponentInfo::Data*> m_registry;
	static std::unordered_map<std::string, size_t> m_nameToID;
	static size_t m_nextID;

};

#define COMPONENT_REFLECTION(T)						\
struct T##ReflectionRegister						\
{													\
	T##ReflectionRegister()							\
	{												\
		ComponentInfo::Register<T>(#T);				\
	}												\
													\
};													\
static T##ReflectionRegister reflectionRegister_##T;	

struct SystemInfo
{
public:
	struct Data
	{
		std::string m_name;
		size_t m_hash = 0;
		size_t m_size = 0;
		std::function<ISystem* ()> m_creator;
	};

	template<class T>
	static const Data& GetInstance()
	{
		static Data data;
		return data;
	}

	template<class T>
	static void Register(std::string_view _name)
	{
		Data& data = *const_cast<Data*>(&GetInstance<T>());
		{
			data.m_name = _name.data();
			data.m_hash = std::hash<std::string>()(data.m_name);
			data.m_size = sizeof(T);
			data.m_creator = []() { return new T(); };
		};

		m_registry[data.m_hash] = &data;
		m_nameToID[data.m_name] = data.m_hash;
	}

	static const std::unordered_map<size_t, SystemInfo::Data*>& GetRegistry() { return m_registry; }
	static const SystemInfo::Data* FindData(size_t _hash)
	{
		auto it = m_registry.find(_hash);
		if (it == m_registry.end()) 
		{
			return nullptr;
		}
		return it->second;
	}

private:
	static std::unordered_map<size_t, SystemInfo::Data*> m_registry;
	static std::unordered_map<std::string, size_t> m_nameToID;
};

#define SYSTEM_REFLECTION(T)						\
struct T##ReflectionRegister						\
{													\
	T##ReflectionRegister()							\
	{												\
		SystemInfo::Register<T>(#T);				\
	}												\
													\
};													\
static T##ReflectionRegister reflectionRegister_##T;	