#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Entity.h"
#include "Math.h"

class IComponent
{

};

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
