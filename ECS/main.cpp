#include <iostream>
#include "ClassInfo.h"
#include "Component.h"
#include "System.h"
#include "ECSManager.h"
#include "Math.h"

COMPONENT_REFLECTION(GameObject);
COMPONENT_REFLECTION(Transfrom);
COMPONENT_REFLECTION(Rigidbody);

struct Player : IComponent
{
	int hp = 0;
};
COMPONENT_REFLECTION(Player);



class RigidbodyUpdate
	: public System<Transfrom, Rigidbody>
{
public:
	RigidbodyUpdate(ECSManager* _pMgr)
		: System(_pMgr)
	{
	}
protected:
	void Update(Transfrom& _trans, Rigidbody& _rigid) override
	{
		_trans.m_pos += _rigid.m_move;
	}
};

class ShowPosistionUpdate
	: public System<Transfrom>
{
public:
	ShowPosistionUpdate(ECSManager* _pMgr)
	: System(_pMgr)
	{}
protected:
	void Update(Transfrom& _trans) override
	{
		std::cout << "X : " << _trans.m_pos.v[0];
		std::cout << "Y : " << _trans.m_pos.v[1];
		std::cout << "Z : " << _trans.m_pos.v[2];
	}
};

int main()
{
	ECSManager _ECSManager;
	_ECSManager.SetUp();
	/*
	_ECSManager.AddSystem<RigidbodyUpdate>();
	_ECSManager.AddSystem<ShowPosistionUpdate>();
	Entity entity = _ECSManager.CreateEntity();
	{
		_ECSManager.AddComponent<Transfrom>(entity)->m_pos = { 100,10,-10 };
		_ECSManager.AddComponent<Rigidbody>(entity)->m_move = {100,0,0};
	}
	entity = _ECSManager.CreateEntity();
	{
		_ECSManager.AddComponent<Transfrom>(entity)->m_pos = { 25,5,-5 };
		_ECSManager.AddComponent<Rigidbody>(entity);
	}
	*/

	_ECSManager.excute();
	_ECSManager.Shutdown();
}
