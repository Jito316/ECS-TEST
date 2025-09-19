#pragma once
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

//===================================
// 注意！！押し返し未実装！！
//===================================
class BoxColliderComponent
	:public ColliderComponent
{
public:
	void RenderUpdate()override;
	void ImGuiUpdate()override;

	void Deserialize(const nlohmann::json& _json)override;
	void Serialize(nlohmann::json& _json)override;

	void RegisterCollider()override;

	void MakeResult(ColliderManager* _pColliderManager, std::weak_ptr<RigidbodyComponent> _rigidbody, std::list<CollisionResult>& _results)override;

private:
	Math::Quaternion ToQuaternion();
	DirectX::BoundingOrientedBox m_infoBox;

	Math::Vector3 m_offsetPos;
	Math::Vector3 m_size;
	Math::Vector3 m_offsetRotation;
};
