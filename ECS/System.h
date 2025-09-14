#pragma once
#include <string>
#include <vector>
#include <shared_mutex>
#include "Math.h"
#include "Component.h"
#include "ECSManager.h"

class ECSManager;

class ISystem
{
public:
	ISystem(ECSManager* _pMgr)
	{

	}

	virtual ~ISystem() = default;

	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual const ComponentMask GetID() = 0;
	virtual void onCreateEntity(const Entity _entity) = 0;
	virtual void onDestroyEntity(const Entity _entity) = 0;
};

template<typename ...ComponentTs>
class System : public ISystem
{
public:
	System(ECSManager* _pMgr)
		: ISystem(_pMgr)
		, m_pManager(_pMgr)
	{
	}
	virtual ~System() override = default;

private:
	void PreUpdate()	override final { foreach(&System::PreUpdate); };
	void Update()		override final { foreach(&System::Update); };
	void PostUpdate()	override final { foreach(&System::PostUpdate); };

	void onCreate()		override final { foreach(&System::onCreate); };
	void onDestroy()	override final { foreach(&System::onDestroy); };

	void foreach(void(System::* _func)(ComponentTs&...))
	{
		for (auto& entity : m_entityContainer.m_vEntities)
		{
			if constexpr (sizeof...(ComponentTs) > 0)
			{
				(this->*_func)(*m_pManager->GetComponent<ComponentTs>(entity)...);
			}
		}
	}

	const ComponentMask GetID()override
	{
		return((1 << ComponentInfo::GetInstance<ComponentTs>().m_id) | ...);
	}
	void onCreateEntity(const Entity _entity)override
	{
		m_entityContainer.Add(_entity);
	}
	void onDestroyEntity(const Entity _entity)override
	{
		m_entityContainer.Remove(_entity);
	}

protected:
	virtual void PreUpdate(ComponentTs&...) {}
	virtual void Update(ComponentTs&...) {}
	virtual void PostUpdate(ComponentTs&...) {}

	virtual void onCreate(ComponentTs&...) {}
	virtual void onDestroy(ComponentTs&...) {}

	ECSManager* m_pManager;
	ECSManager::EntityContainer m_entityContainer;
};
