#include <iostream>
#include "ClassInfo.h"
#include "Component.h"
#include "System.h"
#include "ECSManager.h"
#include "Math.h"

struct GameObject : public IComponent
{
	bool m_enable = false;
	bool m_destroy = false;
};

struct Transfrom : public IComponent
{
	Math::Vector3 m_scale;
	Math::Vector3 m_rotation;
	Math::Vector3 m_pos;
};

struct Rigidbody : public IComponent
{
	Math::Vector3 m_move;
};

struct Player : IComponent
{
	int hp = 0;
};

COMPONENT_REFLECTION(GameObject);
COMPONENT_REFLECTION(Transfrom);
COMPONENT_REFLECTION(Rigidbody);
COMPONENT_REFLECTION(Player);

class RigidbodyUpdate
	: public System<Transfrom, Rigidbody>
{
protected:
	void Update(Transfrom& _trans, Rigidbody& _rigid) override
	{
		_trans.m_pos += _rigid.m_move;
	}
};

class ShowPosistionUpdate
	: public System<Transfrom>
{
protected:
	void Update(Transfrom& _trans) override
	{
		std::cout << "X : " << _trans.m_pos.v[0];
		std::cout << "Y : " << _trans.m_pos.v[1];
		std::cout << "Z : " << _trans.m_pos.v[2];
		std::cout << std::endl;
	}
};

class ShowHpUpdate
	: public System<Player>
{
protected:
	void Update(Player& _player) override
	{
		std::cout << "HP : " << _player.hp;
		std::cout << std::endl;
	}
};

SYSTEM_REFLECTION(RigidbodyUpdate);
SYSTEM_REFLECTION(ShowPosistionUpdate);
SYSTEM_REFLECTION(ShowHpUpdate);

int main()
{
	ECSManager _ECSManager;
	_ECSManager.SetUp();
	
	_ECSManager.AddSystem<ShowHpUpdate>();
	_ECSManager.AddComponent<Player>(1)->hp = 100;

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

	//_ECSManager.Shutdown();
}
