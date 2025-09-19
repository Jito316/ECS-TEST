#pragma once
#include "Interface/Manager/ObjectManager/IGameObjectManager.h"
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

class GameObject;
class Component;
class Collider_Event;

class GameObjectManager : public IGameObjectManager
{
public:
	virtual ~GameObjectManager()override = default;

	void Start()override;
	// 更新系
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;
	void RenderUpdate()override;

	// 初期化・解放
	void Load(std::string _path)override;
	void Release(std::string _path, bool _enableSave)override;

	const std::list<std::shared_ptr<IGameObject>>& GetObjectList() const override { return m_objectList; }
	std::list<std::shared_ptr<IGameObject>>& WorkObjectList() override { return m_objectList; }

private:
	friend GameObject;
	std::list<std::shared_ptr<IGameObject>> m_objectList;

	std::list<std::weak_ptr<ColliderComponent>>	m_colliderList;
	std::shared_ptr<std::queue<std::shared_ptr<IComponent>>> m_instanceQueue;

	void Deserialize(std::string _path, bool _bOrigin = true);

	void EnqueueForInitialization(std::shared_ptr<IComponent> _cmp);
	void Reset();
};
