#include "KdCollider.h"

namespace JTN {
	namespace Math {
		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
		// KdCollider
		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

		struct KdCollider::ColliderPimple {
			std::unordered_map<std::string, std::shared_ptr<KdCollisionShape>> collisionShapes;
			int disableType = 0;
		};

		KdCollider::KdCollider() {
			m_colliderPimple = new ColliderPimple;
		}
		KdCollider::~KdCollider() {
			delete m_colliderPimple;
			m_colliderPimple = nullptr;
		}

		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 当たり判定形状の登録関数群
		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::RegisterCollisionShape(std::string_view name, std::shared_ptr<KdCollisionShape> spShape)
		{
			if (!spShape) { return; }

			m_colliderPimple->collisionShapes.emplace(name.data(), std::move(spShape));
		}

		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::RegisterCollisionShape(std::string_view name, const DirectX::BoundingSphere& sphere, UINT type)
		{
			RegisterCollisionShape(name, std::make_unique<KdSphereCollision>(sphere, type));
		}

		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::RegisterCollisionShape(std::string_view name, const DirectX::BoundingBox& box, UINT type)
		{
			RegisterCollisionShape(name, std::make_unique<KdBoxCollision>(box, type));
		}

		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::RegisterCollisionShape(std::string_view name, const DirectX::BoundingOrientedBox& box, UINT type)
		{
			RegisterCollisionShape(name, std::make_unique<KdBoxCollision>(box, type));
		}

		///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::RegisterCollisionShape(std::string_view name, const Math::Vector3& localPos, float radius, UINT type)
		{
			RegisterCollisionShape(name, std::make_unique<KdSphereCollision>(localPos, radius, type));
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// コライダーvs球に登録された任意の形状の当たり判定
		// 球に合わせて何のために当たり判定をするのか type を渡す必要がある
		// 第3引数に詳細結果の受け取る機能が付いている
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdCollider::Intersects(const SphereInfo& targetShape, const Math::Matrix& ownerMatrix, std::list<KdCollider::CollisionResult>* pResults) const
		{
			// 当たり判定無効のタイプの場合は返る
			if (targetShape.m_type & m_colliderPimple->disableType) { return false; }

			bool isHit = false;

			for (auto& collisionShape : m_colliderPimple->collisionShapes)
			{
				// 用途が一致していない当たり判定形状はスキップ
				if (!(targetShape.m_type & collisionShape.second->GetType())) { continue; }

				KdCollider::CollisionResult tmpRes;
				KdCollider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;

				if (collisionShape.second->Intersects(targetShape.m_sphere, ownerMatrix, pTmpRes))
				{
					isHit = true;

					// 詳細な衝突結果を必要としない場合は1つでも接触して返す
					if (!pResults) { break; }

					tmpRes.m_objectPos = ownerMatrix.Translation();
					tmpRes.m_type = collisionShape.second->GetType();
					pResults->push_back(tmpRes);
				}
			}

			return isHit;
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// コライダーvsBOXに登録された任意の形状の当たり判定
		// BOXに合わせて何のために当たり判定をするのか type を渡す必要がある
		// 第3引数に詳細結果の受け取る機能が付いている
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdCollider::Intersects(const BoxInfo& targetBox, const Math::Matrix& ownerMatrix, std::list<KdCollider::CollisionResult>* pResults) const
		{
			// 当たり判定無効のタイプの場合は返る
			if (targetBox.m_type & m_colliderPimple->disableType) { return false; }

			bool isHit = false;

			for (auto& collisionShape : m_colliderPimple->collisionShapes)
			{
				// 用途が一致していない当たり判定形状はスキップ
				if (!(targetBox.m_type & collisionShape.second->GetType())) { continue; }

				KdCollider::CollisionResult tmpRes;
				KdCollider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;

				bool isIntersects = (targetBox.CheckBoxType(BoxInfo::BoxType::BoxAABB)) ? collisionShape.second->Intersects(targetBox.m_Abox, ownerMatrix, pTmpRes) :
					collisionShape.second->Intersects(targetBox.m_Obox, ownerMatrix, pTmpRes);
				if (isIntersects)
				{
					isHit = true;

					// 詳細な衝突結果を必要としない場合は1つでも接触して返す
					if (!pResults) { break; }
					tmpRes.m_type = collisionShape.second->GetType();
					pResults->push_back(tmpRes);
				}
			}

			return isHit;
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// コライダーvsレイに登録された任意の形状の当たり判定
		// レイに合わせて何のために当たり判定をするのか type を渡す必要がある
		// 第3引数に詳細結果の受け取る機能が付いている
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdCollider::Intersects(const RayInfo& targetShape, const Math::Matrix& ownerMatrix, std::list<KdCollider::CollisionResult>* pResults) const
		{
			// 当たり判定無効のタイプの場合は返る
			if (targetShape.m_type & m_colliderPimple->disableType) { return false; }

			// レイの方向ベクトルが存在しない場合は判定不能なのでそのまま返る
			if (!targetShape.m_dir.LengthSquared())
			{
				assert(0 && "KdCollider::Intersects：レイの方向ベクトルが存在していないため、正しく判定できません");

				return false;
			}

			bool isHit = false;

			for (auto& collisionShape : m_colliderPimple->collisionShapes)
			{
				// 用途が一致していない当たり判定形状はスキップ
				if (!(targetShape.m_type & collisionShape.second->GetType())) { continue; }

				KdCollider::CollisionResult tmpRes;
				KdCollider::CollisionResult* pTmpRes = pResults ? &tmpRes : nullptr;

				if (collisionShape.second->Intersects(targetShape, ownerMatrix, pTmpRes))
				{
					isHit = true;

					// 詳細な衝突結果を必要としない場合は1つでも接触して返す
					if (!pResults) { break; }
					tmpRes.m_type = collisionShape.second->GetType();
					pResults->push_back(tmpRes);
				}
			}

			return isHit;
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 任意のCollisionShapeを検索して有効/無効を切り替える
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::SetEnable(std::string_view name, bool flag)
		{
			auto targetCol = m_colliderPimple->collisionShapes.find(name.data());

			if (targetCol != m_colliderPimple->collisionShapes.end())
			{
				targetCol->second->SetEnable(flag);
			}
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 特定のタイプの有効/無効を切り替える
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::SetEnable(int type, bool flag)
		{
			// 有効にしたい
			if (flag)
			{
				m_colliderPimple->disableType &= ~type;
			}
			// 無効にしたい
			else
			{
				m_colliderPimple->disableType |= type;
			}
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 全てのCollisionShapeの有効/無効を一気に切り替える
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		void KdCollider::SetEnableAll(bool flag)
		{
			for (auto& col : m_colliderPimple->collisionShapes)
			{
				col.second->SetEnable(flag);
			}
		}


		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
		// SphereCollision
		// 球形の形状
		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 球vs球の当たり判定
		// 判定回数は 1 回　計算自体も軽く最も軽量な当たり判定　計算回数も固定なので処理効率は安定
		// 片方の球の判定を0にすれば単純な距離判定も作れる
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdSphereCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			DirectX::BoundingSphere myShape;

			m_shape.Transform(myShape, world);

			// 球同士の当たり判定
			bool isHit = myShape.Intersects(target);

			// 詳細リザルトが必要無ければ即結果を返す
			if (!pRes) { return isHit; }

			// 当たった時のみ計算
			if (isHit)
			{
				// お互いに当たらない最小距離
				float needDistance = target.Radius + myShape.Radius;

				// 自身から相手に向かう方向ベクトル
				pRes->m_hitDir = (Math::Vector3(target.Center) - Math::Vector3(myShape.Center));
				float betweenDistance = pRes->m_hitDir.Length();

				// 重なり量 = お互い当たらない最小距離 - お互いの実際距離
				pRes->m_overlapDistance = needDistance - betweenDistance;

				pRes->m_hitDir.Normalize();

				// 当たった座標はお互いの球の衝突の中心点
				pRes->m_hitPos = myShape.Center + pRes->m_hitDir * (myShape.Radius + pRes->m_overlapDistance * 0.5f);
			}

			return isHit;
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 球vsBOX(AABB)の当たり判定
		// 判定回数は 1 回　計算自体も軽く最も軽量な当たり判定　計算回数も固定なので処理効率は安定
		// 片方の球の判定を0にすれば単純な距離判定も作れる
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdSphereCollision::Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			DirectX::BoundingSphere myShape;

			m_shape.Transform(myShape, world);

			float hitDistance = 0.0f;

			return  myShape.Intersects(target);
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 球vsBOX(OBB)の当たり判定
		// 判定回数は 1 回　計算自体も軽く最も軽量な当たり判定　計算回数も固定なので処理効率は安定
		// 片方の球の判定を0にすれば単純な距離判定も作れる
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdSphereCollision::Intersects(const DirectX::BoundingOrientedBox& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			DirectX::BoundingSphere myShape;

			m_shape.Transform(myShape, world);

			float hitDistance = 0.0f;

			return  myShape.Intersects(target);
		}

		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		// 球vsレイの当たり判定
		// 判定回数は 1 回　計算回数が固定なので処理効率は安定
		// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
		bool KdSphereCollision::Intersects(const KdCollider::RayInfo& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			DirectX::BoundingSphere myShape;

			m_shape.Transform(myShape, world);

			float hitDistance = 0.0f;

			bool isHit = myShape.Intersects(target.m_pos, target.m_dir, hitDistance);

			// 判定限界距離を加味
			isHit &= (target.m_range >= hitDistance);

			// 詳細リザルトが必要無ければ即結果を返す
			if (!pRes) { return isHit; }

			// 当たった時のみ計算
			if (isHit)
			{
				// レイ発射位置 + レイの当たった位置までのベクトル 
				pRes->m_hitPos = target.m_pos + target.m_dir * hitDistance;

				pRes->m_hitDir = target.m_dir * (-1);

				pRes->m_overlapDistance = target.m_range - hitDistance;
			}

			return isHit;
		}

		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
		// BOXCollision
		// BOXの形状
		// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
		bool KdBoxCollision::Intersects(const DirectX::BoundingSphere& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			if (m_isOriented)
			{
				DirectX::BoundingOrientedBox box;
				m_Obox.Transform(box, world);
				return m_Obox.Intersects(target);
			}
			else
			{
				DirectX::BoundingBox box;
				m_Abox.Transform(box, world);
				return m_Abox.Intersects(target);
			}
		}
		bool KdBoxCollision::Intersects(const DirectX::BoundingBox& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			if (m_isOriented)
			{
				DirectX::BoundingOrientedBox box;
				m_Obox.Transform(box, world);
				return box.Intersects(target);
			}
			else
			{
				DirectX::BoundingBox box;
				m_Abox.Transform(box, world);
				return box.Intersects(target);
			}
		}
		bool KdBoxCollision::Intersects(const DirectX::BoundingOrientedBox& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			if (m_isOriented)
			{
				DirectX::BoundingOrientedBox box;
				m_Obox.Transform(box, world);
				return box.Intersects(target);
			}
			else
			{
				DirectX::BoundingBox box;
				m_Abox.Transform(box, world);
				return box.Intersects(target);
			}
		}
		bool KdBoxCollision::Intersects(const KdCollider::RayInfo& target, const Math::Matrix& world, KdCollider::CollisionResult* pRes)
		{
			if (!m_enable) { return false; }

			float hitDistance = 0.0f;
			if (m_isOriented)
			{
				DirectX::BoundingOrientedBox box;
				m_Obox.Transform(box, world);
				return m_Obox.Intersects(DirectX::XMLoadFloat3(&target.m_pos), DirectX::XMLoadFloat3(&target.m_dir), hitDistance);
			}
			else
			{
				DirectX::BoundingBox box;
				m_Abox.Transform(box, world);
				return m_Abox.Intersects(target.m_pos, target.m_dir, hitDistance);
			}
		}
	}
}
