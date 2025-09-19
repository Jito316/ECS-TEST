#pragma once
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

class SphereColliderComponent
	:public ColliderComponent
{
public:
	void RenderUpdate()override;
	void ImGuiUpdate()override;

	void Deserialize(const nlohmann::json& _json)override;
	void Serialize(nlohmann::json& _json)override;

	void RegisterCollider()override;

	void MakeResult(ColliderManager* _pColliderManager, std::weak_ptr<RigidbodyComponent> _rigidbody,std::list<CollisionResult>& _results)override;

protected:
	Math::Vector3 m_offsetPos;
	float m_radius = 2.f;
};
