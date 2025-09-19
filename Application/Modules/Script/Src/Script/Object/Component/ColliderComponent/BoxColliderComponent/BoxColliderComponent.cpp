#include "BoxColliderComponent.h"

void BoxColliderComponent::RenderUpdate()
{
	if (JTN::GetEngine()->is_Debug())
	{
		auto renderer = GETMODULE(IRenderer);
		if (renderer)
		{
			auto debug = renderer->GetDebugWireFrame();

			DirectX::BoundingOrientedBox box;
			m_infoBox.Transform(box, m_trans.lock()->GetWorldMatrix());

			auto trans = Math::Matrix::CreateFromQuaternion(box.Orientation) * Math::Matrix::CreateTranslation(box.Center);


			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);

			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);

			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Forward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Left + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Forward)) * trans).Translation()
				, kWhiteColor
			);
			debug->AddDebugLine(
				(Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Up + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, (Math::Matrix::CreateTranslation(box.Extents * (Math::Vector3::Down + Math::Vector3::Right + Math::Vector3::Backward)) * trans).Translation()
				, kWhiteColor
			);
		}
	}
}

void BoxColliderComponent::ImGuiUpdate()
{
	ColliderComponent::ImGuiUpdate();
	m_bDirty |= EDITOR_COMMAND("DragFloat3", "size", &m_size);
	m_bDirty |= EDITOR_COMMAND("DragFloat3", "offsetPos", &m_offsetPos);
	m_bDirty |= EDITOR_COMMAND("DragFloat3", "offsetRotation", &m_offsetRotation);

	m_isTrigger = true;

	if (m_bDirty)
	{
		Reset();
		Start();

		m_bDirty = false;
	}
}

void BoxColliderComponent::Deserialize(const nlohmann::json& _json)
{
	ColliderComponent::Deserialize(_json);
	JsonHelper::ToValue(_json, "size", m_size);
	JsonHelper::ToValue(_json, "offsetPos", m_offsetPos);
	JsonHelper::ToValue(_json, "offsetRot", m_offsetRotation);
}

void BoxColliderComponent::Serialize(nlohmann::json& _json)
{
	ColliderComponent::Serialize(_json);
	JsonHelper::ToJson(_json, "size", m_size);
	JsonHelper::ToJson(_json, "offsetPos", m_offsetPos);
	JsonHelper::ToJson(_json, "offsetPot", m_offsetRotation);
}

void BoxColliderComponent::RegisterCollider()
{
	m_infoBox =
	{
		m_offsetPos,
		m_size,
		ToQuaternion()
	};

	m_collider->RegisterCollisionShape(
		std::to_string((int)this)
		, m_infoBox
		, m_colliderType
	);
}

void BoxColliderComponent::MakeResult(ColliderManager* _pColliderManager, std::weak_ptr<RigidbodyComponent> _rigidbody, std::list<CollisionResult>& _results)
{
	DirectX::BoundingOrientedBox box;
	m_infoBox.Transform(box, m_trans.lock()->GetWorldMatrix());

	Intersects({ m_colliderType,box }, &_results);
}

Math::Quaternion BoxColliderComponent::ToQuaternion()
{
	Math::Vector3 rot = m_offsetRotation;
	std::swap(rot.x, rot.y);
	return Math::Quaternion::CreateFromYawPitchRoll(Utility::MathHelper::ToRadians(rot));
}
