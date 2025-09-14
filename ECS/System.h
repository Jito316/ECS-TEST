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

	void onCreate()		override final{ foreach(&System::onCreate); };
	void onDestroy()	override final{ foreach(&System::onDestroy); };

	void foreach(void(System::* _func)(ComponentTs&...))
	{
		for (auto& entitiy : m_pManager->GetActiveEntitys())
		{
			// 各コンポーネントを取得
			if constexpr (sizeof...(ComponentTs) > 0)
			{
				// 全てのコンポーネントを揃えられたら呼び出す
				auto tuple = m_pManager->GetComponents<ComponentTs...>(entitiy);
				if (tuple)
				{
					std::apply([&](auto*... comps) {
						(this->*_func)(*comps...);
						}, *tuple);
				}
			}
		}
	}


protected:
	virtual void PreUpdate(ComponentTs&...) {}
	virtual void Update(ComponentTs&...) {}
	virtual void PostUpdate(ComponentTs&...) {}

	virtual void onCreate(ComponentTs&...) {}
	virtual void onDestroy(ComponentTs&...) {}

	ECSManager* m_pManager;
};

class RigidUpdate
	: public System<Transfrom, Rigidbody> 
{
protected:
	void Update(Transfrom& _trans, Rigidbody& _rigid) override
	{
		m_pManager->GetComponents<Transfrom, Rigidbody>(0);
		_trans.m_pos += _rigid.m_move;
	}
};