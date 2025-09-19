#include "RigidbodyComponent.h"
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"
#include "Script/Manager/ColliderManager/ColliderManager.h"

void RigidbodyComponent::Awake()
{
	auto& colliderManager = *GETSCRIPT.GetColliderManager();
	colliderManager.AddHandler(std::static_pointer_cast<RigidbodyComponent>(shared_from_this()));
}

void RigidbodyComponent::Update()
{
	if (m_bActiveGravity)
	{
		GravityProcess();
	}
	m_trans.lock()->SetLocalPosition(m_trans.lock()->GetLocalPosition() + m_move);
}

void RigidbodyComponent::PostUpdate()
{
	m_move *= m_deceleration;
}

void RigidbodyComponent::RenderUpdate()
{
	if (m_gravity)
	{
		// info設定
		Math::Vector3 pos = m_trans.lock()->GetWorldPosition();
		Math::KdCollider::RayInfo rayInfo(
			(UINT)Math::KdCollider::Type::TypeGround,
			pos + Math::Vector3::Up * m_height,
			pos + Math::Vector3::Down * (m_gravity + m_height)
		);

		GETMODULE(IRenderer)->GetDebugWireFrame()->AddDebugLine(
			rayInfo.m_pos
			, rayInfo.m_dir
			, rayInfo.m_range
		);
	}
}

void RigidbodyComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("DragFloat3", "Move", &m_move.x);
	EDITOR_COMMAND("DragFloat", "Deceleration", &m_deceleration);

	if (EDITOR_COMMAND("Checkbox", "Gravity", &m_bActiveGravity); m_bActiveGravity)
	{
		EDITOR_COMMAND("DragFloat", "Gravity", &m_gravity);
		EDITOR_COMMAND("DragFloat", "GravityPow", &m_gravityPow);
		EDITOR_COMMAND("DragFloat", "GravityMax", &m_gravityMax);
		EDITOR_COMMAND("DragFloat", "Height", &m_height);;
	}
}

void RigidbodyComponent::Deserialize(const nlohmann::json& _json)
{
	m_move = Utility::JsonHelper::InPutVec3(_json["move"]);
	m_deceleration = _json["deceleration"];

	m_bActiveGravity = _json["activeGravityFlag"];
	m_gravityPow = _json["gravity"];
	m_height = _json["height"];
}
void RigidbodyComponent::Serialize(nlohmann::json& _json)
{
	_json["move"] = Utility::JsonHelper::OutPutVec3(m_move);
	_json["deceleration"] = m_deceleration;
	_json["activeGravityFlag"] = m_bActiveGravity;
	_json["gravity"] = m_gravityPow;
	_json["gravityMax"] = m_gravityMax;
	_json["height"] = m_height;
}

void RigidbodyComponent::OnCollisionEnter(const CollisionResult* _collision)
{
	m_move += _collision->m_hitDir * _collision->m_overlapDistance;
}

void RigidbodyComponent::OnCollisionStay(const CollisionResult* _collision)
{
	m_move += _collision->m_hitDir * _collision->m_overlapDistance;
}

void RigidbodyComponent::OnDestroy()
{
	auto& colliderManager = *GETSCRIPT.GetColliderManager();
	colliderManager.RemoveHandler(m_owner);
}

void RigidbodyComponent::GravityProcess()
{
	// 重力加算
	if (m_gravity < m_gravityMax)
	{
		m_gravity += m_gravityPow;
	}

	// info設定
	Math::Vector3 pos = m_trans.lock()->GetWorldPosition();
	Math::KdCollider::RayInfo rayInfo(
		(UINT)Math::KdCollider::Type::TypeGround,
		pos + Math::Vector3::Up * m_height,
		pos + Math::Vector3::Down * (m_gravity + m_height)
	);

	//重力と床判定
	m_bLanding = false;
	float hitRange = 0.0f;
	std::list<CollisionResult>	results;
	CollisionResult* effectResult = nullptr;

	GETSCRIPT.GetColliderManager()->Intersects(m_owner, rayInfo, &results);
	for (auto& result : results)
	{
		float targetRange = Math::Vector3::Distance(result.m_hitPos, m_trans.lock()->GetLocalPosition());
		if (hitRange > targetRange || !m_bLanding)
		{
			hitRange = targetRange;
			effectResult = &result;
			m_bLanding = true;
		}
	}

	if (m_bLanding)
	{
		pos.y = effectResult->m_hitPos.y;
		m_trans.lock()->SetLocalPosition(pos);

		m_move.y = 0.f;
		m_gravity = 0.f;
	}
	else
	{
		m_move.y = m_move.y - m_gravity;
	}
}