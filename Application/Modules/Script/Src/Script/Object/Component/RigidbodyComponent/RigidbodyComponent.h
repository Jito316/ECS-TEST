#pragma once
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"
#include "Script/Event/Collider_Event/Collider_Event.h"

//もどき : 移動量管理
//		 : 当たる側の当たり判定設定
class RigidbodyComponent
	:public Component
	,public Collider_Event
{
public:
	void Awake()override;

	void Update()override;
	void PostUpdate()override;

	void RenderUpdate()override;

	void ImGuiUpdate()override;

	void Deserialize(const nlohmann::json& _json)override;
	void Serialize(nlohmann::json& _json)override;

	void AddMove(const Math::Vector3& _move) { m_move += _move; }
	const Math::Vector3& GetMove()	const { return m_move; }
	void SetMove(const Math::Vector3& _move) { m_move = _move; }
	void ResetMove() { m_move = Math::Vector3::Zero; }

	float GetGravity() const { return m_gravityPow; }
	void  SetGravity(float _gravity) { m_gravity = _gravity; }
	bool GetLanding() const { return m_bLanding; }

	void OnCollisionEnter(const CollisionResult* _collision) override;
	void OnCollisionStay(const CollisionResult* _collision) override;

	void OnDestroy()override;
private:
	friend ColliderComponent;
	void GravityProcess();

	Math::Vector3 m_move = Math::Vector3::Zero;

	bool  m_bActiveGravity = true;
	float m_gravity = 0.0f;
	float m_gravityPow = 1.0f;
	float m_gravityMax = 1.0f;
	float m_height = 0.0f;
	bool  m_bLanding = false;

	float m_deceleration = 0.98f;
};