#pragma once
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

class ModelRendererComponent;
class SquarePolygonRendererComponent;

class MeshColliderComponent
	:public ColliderComponent
{
	void ImGuiUpdate()override;

	void RegisterCollider()override;
	bool RegisterCollider(std::weak_ptr<ModelRendererComponent> _asset);
	bool RegisterCollider(std::weak_ptr<SquarePolygonRendererComponent> _asset);

	void MakeResult(ColliderManager*, std::weak_ptr<RigidbodyComponent>,std::list<CollisionResult>&) override {};

	std::weak_ptr<ModelRendererComponent> m_wpModel;
	std::weak_ptr<SquarePolygonRendererComponent> m_wpPoly;
};
