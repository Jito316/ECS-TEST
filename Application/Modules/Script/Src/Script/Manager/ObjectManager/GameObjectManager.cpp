#include "GameObjectManager.h"
#include "Interface/Object/GameObject/IGameObject.h"
#include "Script/Object/GameObject/GameObject.h"
#include "Interface/Object/Component/IComponent.h"
#include "Script/Object/Component/Component.h"
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

void GameObjectManager::Start()
{
	if (m_instanceQueue)
	{
		while (m_instanceQueue->size())
		{
			auto& cmp = m_instanceQueue->front();
			cmp->Start();
			m_instanceQueue->pop();
		}
		m_instanceQueue.reset();
	}
}

void GameObjectManager::PreUpdate()
{
	for (auto& object : m_objectList)
	{
		if (object->GetEnable() && !object->GetDestroy())
		{
			object->PreUpdate();
		}
	}
}
void GameObjectManager::Update()
{
	for (auto& object : m_objectList)
	{
		if (object->GetEnable() && !object->GetDestroy())
		{
			object->Update();
			object->GetTransform().lock()->MatrixUpdate();
		}
	}
}
void GameObjectManager::PostUpdate()
{
	for (auto& object : m_objectList)
	{
		if (object->GetEnable() && !object->GetDestroy())
		{
			object->PostUpdate();
		}
	}
}
void GameObjectManager::RenderUpdate()
{
	for (auto it = m_objectList.begin()
		; it != m_objectList.end();
		)
	{
		std::shared_ptr<IGameObject>object = *it;
		if (object->GetDestroy())
		{
			(*it)->SetParent(std::weak_ptr<IGameObject>());
			it = m_objectList.erase(it);
			continue;
		}

		if (object->GetEnable()) 
		{
			object->RenderUpdate();
		}

		it++;
	}
}

void GameObjectManager::Load(std::string _path)
{
	Deserialize(_path);
}

void GameObjectManager::Release(std::string _path, bool _enableSave)
{
	if (_enableSave)
	{
		nlohmann::json json = nlohmann::json::array();
		for (auto& object : m_objectList)
		{
			if (object->GetParent().lock())continue;
			if (!object->GetAbleSave())continue;
			nlohmann::json data;
			object->SerializeFamily(data);
			json.push_back(data);
			object->Destroy();
		}
		Utility::JsonHelper::OutputJson(json, _path);
	}
	else
	{
		for (auto& object : m_objectList)object->Destroy();
	}

	m_objectList.clear();
	m_colliderList.clear();
	m_instanceQueue.reset();
}
void GameObjectManager::EnqueueForInitialization(std::shared_ptr<IComponent> _cmp)
{
	if (!m_instanceQueue)m_instanceQueue = std::make_shared<std::queue<std::shared_ptr<IComponent>>>();
	m_instanceQueue->push(_cmp);
}

void GameObjectManager::Reset()
{
	m_instanceQueue.reset();
	m_objectList.clear();
}

void GameObjectManager::Deserialize(std::string _path, bool)
{
	nlohmann::json json = Utility::JsonHelper::InputJson(_path);
	auto name = json.begin();
	if (name == json.end())ADDLOG(u8"not found json by GameObjectManager\n");
	while (name != json.end())
	{
		std::shared_ptr<GameObject>object = GameObject::CreateObject(*name);
		name++;
	}
}
