#pragma once
class Collider_Event;
class RigidbodyComponent;
class ColliderManager;

struct CollisionResult : Math::KdCollider::CollisionResult {
	std::weak_ptr<IGameObject> m_hitObject;
	std::weak_ptr<RigidbodyComponent> m_rigidbody;
	std::weak_ptr<ITransform> m_transform;
};

class ColliderComponent
	:public Component
{
public:
	virtual void Awake()override;
	virtual void Start()override;
	virtual void PreUpdate()override;

	virtual void ImGuiUpdate()override;
	virtual void Deserialize(const nlohmann::json& _json)override;
	virtual void Serialize(nlohmann::json& _json)override;

	virtual void SetEnable(bool _flg)override;

	virtual const UINT GetType() { return m_colliderType; };

	virtual void Reset();
	virtual std::shared_ptr<Math::KdCollider> Work();

	virtual const bool IsTrigger() const { return m_isTrigger; };
	virtual void MakeResult(ColliderManager* _pColliderManager, std::weak_ptr<RigidbodyComponent> _rigidbody,std::list<CollisionResult>& _results) = 0;
	virtual std::weak_ptr<Math::KdCollider> GetCollider() { return m_collider; }

	virtual bool Intersects(const Math::KdCollider::SphereInfo& targetShape, std::list<CollisionResult>* pResults);
	virtual bool Intersects(const Math::KdCollider::BoxInfo& targetShape, std::list<CollisionResult>* pResults);
	virtual bool Intersects(const Math::KdCollider::RayInfo& targetShape, std::list<CollisionResult>* pResults);

	virtual void OnDestroy()override;

protected:
	virtual void RegisterCollider() = 0;
	virtual std::weak_ptr<ColliderComponent> GetMyWeak();

	std::shared_ptr<Math::KdCollider> m_collider = nullptr;
	UINT m_colliderType = 0;
	bool m_isTrigger = false;

	bool m_bDirty = true;
private:
	std::weak_ptr<ColliderComponent> m_mWeak;
	ColliderManager* m_pCollider = nullptr;
};
