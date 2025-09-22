#pragma once

struct IComponent
{

};

struct GameObject : IComponent
{
	enum class Tag
	{
		Untagged,
		Respawn,
		Finish,
		EditorOnly,
		MainCamera,
		Player,
		Enemy,
		GameController
	};

	char m_name[25];
	uint32_t m_uuid = 0;
	Tag m_tag = Tag::Untagged;
};

struct Transform : IComponent
{
	Math::Vector3 m_pos;
	Math::Quaternion m_rotation;
	Math::Vector3 m_scale;
};

struct Rigidbody : IComponent
{
	Math::Vector3 m_move;

	bool m_enableGravity = false;
	Math::Vector3 m_gravity;
	Math::Vector3 m_gravityPow;

	bool m_isTrigger = false;
	Math::KdCollider::Type m_type = Math::KdCollider::Type::TypeGround;
};

struct SphereCollider : IComponent
{
	bool m_isTrigger = false;
	Math::KdCollider::Type m_type = Math::KdCollider::Type::TypeGround;

	float m_radius = 0.0f;
};

struct BoxCollider : IComponent
{
	bool m_isTrigger = false;
	Math::KdCollider::Type m_type = Math::KdCollider::Type::TypeGround;

	bool m_isOriented = false;
	Math::Vector3 m_offset;
	Math::Vector3 m_size;
};