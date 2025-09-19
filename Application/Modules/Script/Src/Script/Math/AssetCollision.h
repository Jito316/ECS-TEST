#pragma once

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// コライダー：モデル形状(dynamicAnimationModelWork)
// モデル形状vs特定形状（球・BOX・レイ）の当たり判定実行クラス
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class ModelWorkCollision : public JTN::Math::KdCollisionShape
{
public:
	ModelWorkCollision(const std::shared_ptr<
		IModelWork>& model, UINT type) :
		KdCollisionShape(type), m_shape(model) {}

	virtual ~ModelWorkCollision() { m_shape.reset(); }

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingOrientedBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const JTN::Math::KdCollider::RayInfo& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;

private:

	std::shared_ptr<IModelWork> m_shape;
};
class ModelDataCollision : public JTN::Math::KdCollisionShape
{
public:
	ModelDataCollision(const std::shared_ptr<IModelData>& model, UINT type) :
		KdCollisionShape(type), m_shape(model) {}

	virtual ~ModelDataCollision() { m_shape.reset(); }

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingOrientedBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const JTN::Math::KdCollider::RayInfo& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;

private:
	std::shared_ptr<IModelData> m_shape;
};

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// コライダー：ポリゴン形状
// ポリゴン形状vs特定形状（球・BOX・レイ）の当たり判定実行クラス
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdPolygonCollision : public JTN::Math::KdCollisionShape
{
public:
	KdPolygonCollision(const std::shared_ptr<IPolygon>& polygon, UINT type) :
		KdCollisionShape(type), m_shape(polygon) {}

	virtual ~KdPolygonCollision() { m_shape.reset(); }

	bool Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const DirectX::BoundingOrientedBox& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;
	bool Intersects(const JTN::Math::KdCollider::RayInfo& target, const Math::Matrix& world, JTN::Math::KdCollider::CollisionResult* pRes) override;

private:
	std::shared_ptr<IPolygon> m_shape;
};

