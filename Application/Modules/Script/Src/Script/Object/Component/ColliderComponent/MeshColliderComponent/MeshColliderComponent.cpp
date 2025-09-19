#include "MeshColliderComponent.h"

#include "Script/Object/Component/RendererComponent/ModelRendererComponent/ModelRendererComponent.h"
#include "Script/Object/Component/RendererComponent/SquarePolygonRendererComponent/SquarePolygonRendererComponent.h"

#include "Script/Math/AssetCollision.h"

void MeshColliderComponent::ImGuiUpdate()
{
	ColliderComponent::ImGuiUpdate();
	if (m_bDirty)
	{
		Reset();
		Start();
		m_bDirty = false;
	}
}

void MeshColliderComponent::RegisterCollider()
{
	if (auto cmp = m_owner.lock()->GetComponent(ComponentReflection<ModelRendererComponent>::id); cmp) RegisterCollider(std::static_pointer_cast<ModelRendererComponent>(cmp));
	if (auto cmp = m_owner.lock()->GetComponent(ComponentReflection<SquarePolygonRendererComponent>::id); cmp) RegisterCollider(std::static_pointer_cast<SquarePolygonRendererComponent>(cmp));
}
bool MeshColliderComponent::RegisterCollider(std::weak_ptr<ModelRendererComponent> _asset)
{
	if (_asset.lock()->GetModelData().expired())return false;
	m_collider->RegisterCollisionShape(
		"ObjectCollision" + std::to_string((int)_asset.lock().get()),
		std::make_unique<ModelDataCollision>(
			_asset.lock()->GetModelData().lock(),
			m_colliderType
		)
	);
	m_wpModel = _asset;
	return true;
}
bool MeshColliderComponent::RegisterCollider(std::weak_ptr<SquarePolygonRendererComponent> _asset)
{
	if (_asset.lock()->GetSquarePolygon().expired())return true;
	const std::shared_ptr<IPolygon> poly = std::shared_ptr<IPolygon>(_asset.lock()->GetSquarePolygon().lock()->WorkPolygon());
	m_collider->RegisterCollisionShape(
		"ObjectCollision" + std::to_string((int)_asset.lock().get()),
		std::make_unique<KdPolygonCollision>(
			poly,
			m_colliderType
		)
	);
	m_wpPoly = _asset;
	return true;
}
