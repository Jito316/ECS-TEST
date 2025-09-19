#include "TransformComponent.h"
#include "Script/Script.h"
#include "Interface/Object/GameObject/IGameObject.h"

TransformComponent::TransformComponent()
{
	m_getMatrix['T'] = [this]() {return GetTMat(); };
	m_getMatrix['S'] = [this]() {return GetSMat(); };
	m_getMatrix['R'] = [this]() {return GetRMat(); };
}

void TransformComponent::MatrixUpdate()
{
	if (m_bDirty)
	{
		m_mWorld = m_mLocal = CreateLocalMatrix();
		m_bDirty = false;
	}

	if (m_parent.lock() && m_bFollow)
	{
		m_mWorld = m_mLocal * m_parent.lock()->GetWorldMatrix();
		m_worldEuler = m_mWorld.ToEuler();
	}
}
void TransformComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("DragFloat3", "Position", &m_localPosition.x);
	Math::Vector3 angle = Utility::MathHelper::ToDegrees(m_localEuler);
	if (EDITOR_COMMAND("DragFloat3", "Rotation", &angle.x))
	{
		angle = Utility::MathHelper::ToRadians(angle);
		SetLocalRotation(angle);
	}

	EDITOR_COMMAND("DragFloat3", "Scale", &m_localScale.x);

	EDITOR_COMMAND("InputText", "myMatTag", &m_myMatTag);

	if (EDITOR_COMMAND("TreeNode", "Property"))
	{
		Math::Vector3 wPos = m_mWorld.Translation();
		EDITOR_COMMAND("InputFloat3", "wPos", &wPos.x);
		Math::Vector3 wRot = Utility::MathHelper::ToDegrees(m_worldEuler);
		std::swap(wRot.x, wRot.y);
		EDITOR_COMMAND("InputFloat3", "wRot", &m_worldEuler);

		EDITOR_COMMAND("Checkbox", "Follow", &m_bFollow);

		EDITOR_COMMAND("TreePop");
	}

	m_bDirty = true;
}

void TransformComponent::Deserialize(const nlohmann::json& _json)
{
	SetLocalPosition(Utility::JsonHelper::InPutVec3(_json["position"]));
	SetLocalRotation(Utility::JsonHelper::InPutVec3(_json["rotation"]));
	SetLocalScale(Utility::JsonHelper::InPutVec3(_json["scale"]));
	m_myMatTag = _json["matTag"];
	m_bFollow = _json["Follow"];

	MatrixUpdate();
}
void TransformComponent::Serialize(nlohmann::json& _json)
{
	_json["position"] = Utility::JsonHelper::OutPutVec3(m_localPosition);

	_json["rotation"] = Utility::JsonHelper::OutPutVec3(m_localRotation.ToEuler());

	_json["scale"] = Utility::JsonHelper::OutPutVec3(m_localScale);
	_json["matTag"] = m_myMatTag;
	_json["Follow"] = m_bFollow;
}

const Math::Vector3 TransformComponent::GetWorldPosition() const
{
	return m_mWorld.Translation();
}
const Math::Vector3& TransformComponent::GetLocalPosition() const
{
	return m_localPosition;
}
void TransformComponent::SetLocalPosition(const Math::Vector3& _pos)
{
	m_bDirty = true;
	m_localPosition = _pos;
}
const Math::Vector3& TransformComponent::GetWorldEuler() const
{
	return m_worldEuler;
}
const Math::Vector3& TransformComponent::GetLocalEuler() const
{
	return m_localEuler;
}
const Math::Quaternion& TransformComponent::GetLocalRotation() const
{
	return m_localRotation;
}

void TransformComponent::SetLocalRotation(const Math::Vector3& _rota)
{
	m_bDirty = true;
	m_localEuler = _rota;
	m_localRotation = Math::Quaternion::CreateFromYawPitchRoll(
		m_localEuler.y,
		m_localEuler.x,
		m_localEuler.z
	);
}
void TransformComponent::SetLocalRotation(const Math::Quaternion& _rota)
{
	m_bDirty = true;
	m_localRotation = _rota;
	m_localEuler = _rota.ToEuler();
}

const Math::Vector3& TransformComponent::GetLocalScale() const
{
	return m_localScale;
}
void TransformComponent::SetLocalScale(const Math::Vector3& _scale)
{
	m_bDirty = true;
	m_localScale = _scale;
}

const Math::Matrix& TransformComponent::GetWorldMatrix()const
{
	return m_mWorld;
}
const Math::Matrix& TransformComponent::GetLocalMatrix()const
{
	return m_mLocal;
}

void TransformComponent::SetWorldMatrix(const Math::Matrix& _matrix)
{
	if (m_parent.lock() && m_bFollow)
	{
		m_mWorld = m_mLocal = _matrix;
		SetLocalMatrix(m_mWorld * m_parent.lock()->GetWorldMatrix().Invert());
	}
	else 
	{
		m_mWorld =_matrix;
		SetLocalMatrix(_matrix);
	}
}
void TransformComponent::SetLocalMatrix(const Math::Matrix& _matrix)
{
	m_mLocal = _matrix;
	m_mLocal.Decompose(m_localScale, m_localRotation, m_localPosition);
	m_localEuler = m_localRotation.ToEuler();
	m_mLocal = _matrix;
}

const std::string& TransformComponent::GetMatTag()
{
	return m_myMatTag;
}
void TransformComponent::SetMatTag(std::string_view _tag)
{
	m_bDirty = true;
	m_myMatTag = _tag.data();
}

void TransformComponent::UnFollow()
{
	m_bFollow = false;
}
const bool TransformComponent::is_Follow() const
{
	return m_bFollow;
}
void TransformComponent::SetFollow(const bool _flg)
{
	m_bFollow = _flg;
}

void TransformComponent::SetParent(std::weak_ptr<ITransform> _parent)
{
	m_parent = _parent;
}

const Math::Matrix TransformComponent::CreateLocalMatrix()
{
	Math::Matrix localMatrix = Math::Matrix::Identity;

	//Matrixの計算順が破損している場合の例外処理
	if (m_myMatTag.empty())
	{
		localMatrix = m_getMatrix['S']() * m_getMatrix['R']() * m_getMatrix['T']();
		return localMatrix;
	}

	std::unordered_map<char, std::function<Math::Matrix()>>::iterator Fn;
	for (auto& it : m_myMatTag)
	{
		Fn = m_getMatrix.find(it);

		//Matrixの計算順が破損している場合の例外処理
		if (Fn == m_getMatrix.end())
		{
			localMatrix = m_getMatrix['S']() * m_getMatrix['R']() * m_getMatrix['T']();
			return localMatrix;
		}

		//Transformの要素を計算
		else
		{
			localMatrix *= Fn->second();
		}
	}
	return localMatrix;
}

const Math::Matrix TransformComponent::GetTMat() const
{
	return Math::Matrix::CreateTranslation(m_localPosition);
}
const Math::Matrix TransformComponent::GetSMat() const
{
	return Math::Matrix::CreateScale(m_localScale);
}
const Math::Matrix TransformComponent::GetRMat() const
{
	return Math::Matrix::CreateFromQuaternion(m_localRotation);
}
const Math::Matrix TransformComponent::GetRMat(UINT _shafts) const
{
	Math::Vector3 radius = m_localRotation.ToEuler();

	return Math::Matrix::CreateFromQuaternion
	(
		Math::Quaternion::CreateFromYawPitchRoll
		(
			(_shafts & Shaft::Y) ? radius.y : 0.0f
			, (_shafts & Shaft::X) ? radius.x : 0.0f
			, (_shafts & Shaft::Z) ? radius.z : 0.0f
		)
	);
}
