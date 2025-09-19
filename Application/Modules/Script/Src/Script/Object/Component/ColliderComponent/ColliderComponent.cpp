#include "ColliderComponent.h"

#include "Script/Manager/ColliderManager/ColliderManager.h"
#include "Script/Event/Collider_Event/Collider_Event.h"
#include "Script/Object/Component/RigidbodyComponent/RigidbodyComponent.h"

void ColliderComponent::Awake()
{
	m_collider = std::make_shared<JTN::Math::KdCollider>();
}

void ColliderComponent::Start()
{
	if (m_mWeak.expired())
	{
		m_mWeak = std::static_pointer_cast<ColliderComponent>(shared_from_this());
		m_pCollider = GETSCRIPT.GetColliderManager();
		m_pCollider->AddCollider(m_mWeak);
	}
	RegisterCollider();
}
void ColliderComponent::PreUpdate()
{

}

void ColliderComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("Checkbox", "isTrigger", &m_isTrigger);
	m_bDirty |= EDITOR_COMMAND("BitEditor_ColliderType", "Type", &m_colliderType);
}
void ColliderComponent::Deserialize(const nlohmann::json& _json)
{
	JsonHelper::ToValue(_json, "isTrigger", m_isTrigger);
	JsonHelper::ToValue(_json, "ColliderType", m_colliderType);
}
void ColliderComponent::Serialize(nlohmann::json& _json)
{
	JsonHelper::ToJson(_json, "isTrigger", m_isTrigger);
	JsonHelper::ToJson(_json, "ColliderType", m_colliderType);
}
void ColliderComponent::SetEnable(bool _flg)
{
	Component::SetEnable(_flg);
	if (m_collider)m_collider->SetEnableAll(_flg);
}

void ColliderComponent::Reset()
{
	m_collider = std::make_shared<Math::KdCollider>();
}
std::shared_ptr<Math::KdCollider> ColliderComponent::Work()
{
	return m_collider;
}

bool ColliderComponent::Intersects(const Math::KdCollider::SphereInfo& targetShape, std::list<CollisionResult>* pResults)
{
	return m_pCollider->Intersects(targetShape, m_isTrigger, pResults);
}

bool ColliderComponent::Intersects(const Math::KdCollider::BoxInfo& targetShape, std::list<CollisionResult>* pResults)
{
	return m_pCollider->Intersects(targetShape, m_isTrigger, pResults);
}

bool ColliderComponent::Intersects(const Math::KdCollider::RayInfo& targetShape, std::list<CollisionResult>* pResults)
{
	return m_pCollider->Intersects(targetShape, m_isTrigger, pResults);
}
void ColliderComponent::OnDestroy()
{
	m_pCollider->RemoveCollider(m_mWeak);
}
std::weak_ptr<ColliderComponent> ColliderComponent::GetMyWeak()
{
	return m_mWeak;
}