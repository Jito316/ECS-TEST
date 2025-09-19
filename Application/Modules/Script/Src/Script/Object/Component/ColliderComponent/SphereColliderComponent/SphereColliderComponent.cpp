#include "SphereColliderComponent.h"

void SphereColliderComponent::RenderUpdate()
{
	if (JTN::GetEngine()->is_Debug())
	{
		auto renderer = GETMODULE(IRenderer);
		if (renderer)
		{
			auto debug = renderer->GetDebugWireFrame();
			debug->AddDebugSphere(
				m_trans.lock()->GetWorldPosition() + m_offsetPos
				, m_radius
			);
		}
	}
}

void SphereColliderComponent::ImGuiUpdate()
{
	ColliderComponent::ImGuiUpdate();
	m_bDirty |= EDITOR_COMMAND("DragFloat", "radius", &m_radius);
	m_bDirty |= EDITOR_COMMAND("DragFloat3", "offsetPos", &m_offsetPos);

	if (m_bDirty)
	{
		Reset();
		Start();
		m_bDirty = false;
	}
}

void SphereColliderComponent::Deserialize(const nlohmann::json& _json)
{
	ColliderComponent::Deserialize(_json);
	JsonHelper::ToValue(_json, "radius", m_radius);
	JsonHelper::ToValue(_json, "offsetPos", m_offsetPos);
}

void SphereColliderComponent::Serialize(nlohmann::json& _json)
{
	ColliderComponent::Serialize(_json);
	JsonHelper::ToJson(_json, "radius", m_radius);
	JsonHelper::ToJson(_json, "offsetPos", m_offsetPos);
}

void SphereColliderComponent::RegisterCollider()
{
	m_collider->RegisterCollisionShape(
		std::to_string((int)this)
		, m_offsetPos
		, m_radius
		, m_colliderType
	);
}



#include "Script/Manager/ColliderManager/ColliderManager.h"
#include "Script/Object/Component/RigidbodyComponent/RigidbodyComponent.h"
void SphereColliderComponent::MakeResult(ColliderManager* _pColliderManager, std::weak_ptr<RigidbodyComponent> _rigidbody, std::list<CollisionResult>& _results)
{
	Math::KdCollider::SphereInfo info(m_colliderType, m_trans.lock()->GetWorldPosition() + m_offsetPos, m_radius);
	Intersects(info, &_results);
}
