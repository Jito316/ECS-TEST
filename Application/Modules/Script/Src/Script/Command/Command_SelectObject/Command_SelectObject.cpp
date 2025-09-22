#include "Command_SelectObject.h"
#include "Script/Math/AssetCollision.h"

namespace Command {
	bool SelectGameObject::Task(void** value)
	{
		if (!(value[0])) {
			return false;
		}
		if (!(value[1])) {
			return false;
		}

		m_pInfo = (Math::KdCollider::RayInfo*)(value[0]);
		m_pInfo->m_type = Math::KdCollider::Type::TypeEvent;
		m_pInfo->m_dir.Normalize();

		if (!CreateColls()) {
			return false;
		}

		if (!CreateResults()) {
			return false;
		}

		if (!PickUpObject(*(Entity*)value[1])) {
			return false;
		}

		return true;
	}

	bool SelectGameObject::CreateColls()
	{
		/*
		GETSCRIPT.GetComponent<>
		m_colls.clear();
		for (auto& it : GameObject::FindObjectOfComponents<ModelRendererComponent>()) {
			auto coll = std::make_unique<Math::KdCollider>(); coll->RegisterCollisionShape(std::to_string((int)it.lock().get()), std::make_shared<ModelWorkCollision>(it.lock()->GetModelWork().lock(), Math::KdCollider::Type::TypeEvent));
			m_colls.push_back({ std::move(coll),it.lock()->GetOwner() });
		}
		for (auto& it : GameObject::FindObjectOfComponents<SquarePolygonRendererComponent>()) {
			const auto poly = std::shared_ptr<IPolygon>(it.lock()->GetSquarePolygon().lock()->WorkPolygon());
			auto coll = std::make_unique<Math::KdCollider>(); coll->RegisterCollisionShape(std::to_string((int)it.lock().get()), std::make_shared<KdPolygonCollision>(poly, Math::KdCollider::Type::TypeEvent));
			m_colls.push_back({ std::move(coll),it.lock()->GetOwner() });
		}
		*/

		return m_colls.size();
	}

	bool SelectGameObject::CreateResults()
	{
		/*
		m_results.clear();
		Math::KdCollider::RayInfo& info = *m_pInfo;
		std::list<Math::KdCollider::CollisionResult> result;
		for (auto& [coll, object] : m_colls) {
			if (coll->Intersects(info, object.lock()->GetTransform().lock()->GetWorldMatrix(), &result)) {
				if(result.size())m_results.push_back({ *result.begin(), object });
				result.clear();
			}
		}
		*/
		return m_results.size();
	}

	bool SelectGameObject::PickUpObject(Entity& _object)
	{
		Entity resultObject = -1;
		float hitLength = 0.f;
		for (auto it = m_results.begin()
			; it != m_results.end()
			; it++) {
			auto& [result, object] = *it;

			if (it == m_results.begin()) {
				hitLength = (m_pInfo->m_pos - result.m_hitPos).Length();
			}

			float length = (m_pInfo->m_pos - result.m_hitPos).Length();
			hitLength = std::min(hitLength, length);
			if (hitLength == length) {
				resultObject = object;
			}
		}

		if (resultObject == -1) {
			return false;
		}

		_object = resultObject;
		return true;
	}


}