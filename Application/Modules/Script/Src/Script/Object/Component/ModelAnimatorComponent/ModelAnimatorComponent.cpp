#include "ModelAnimatorComponent.h"
#include "Script/Object/Component/RendererComponent/ModelRendererComponent/ModelRendererComponent.h"

void ModelAnimatorComponent::Start()
{
	m_modelComponent = m_owner.lock()->GetComponent<ModelRendererComponent>();
	if (!m_modelComponent.expired())
	{
		m_wpModel = m_modelComponent.lock()->GetModelWork();
	}


	if (m_wpModel.lock())
	{
		m_animator.SetAnimation(m_wpModel.lock()->GetAnimation(0));
		AdvanceTime();
	}

	m_enable = (!m_modelComponent.expired()) && !m_wpModel.expired();
}

void ModelAnimatorComponent::PreUpdate()
{
	if (m_animator.IsAnimationEnd() && m_nowAnimatorData.isLoop)
	{
		ChangeAnime(m_nowAnimatorData);
	}

	if (!m_animator.IsAnimationEnd())
	{
		AdvanceTime();
	}
}

void ModelAnimatorComponent::RenderUpdate()
{
	if (!JTN::GetEngine()->is_Debug()
		&& m_wpModel.expired()
		&& !m_animator.m_spAnimation
		)return;

	/*
	auto& wirefire = *GETMODULE(IRenderer)->GetDebugWireFrame();
	const std::vector<IModelWork::Node>& rNodes = m_wpModel.lock()->GetNodes();

	for (auto& rAnimNode : m_animator.m_spAnimation->m_nodes)
	{
		// 対応するモデルノードのインデックス
		UINT idx = rAnimNode.m_nodeOffset;

		auto prev = rNodes[idx].m_worldTransform;
		wirefire.AddDebugSphere((prev * m_trans.lock()->GetWorldMatrix()).Translation(), 0.01f);
	}
	*/

}

void ModelAnimatorComponent::ChangeAnime(std::string_view _view, const bool _isLoop, const float _speed, const bool _isComplement, const float _time)
{
	if (m_wpModel.expired())return;
	const std::shared_ptr<Math::KdAnimationData> postData = m_nowAnimatorData.animeName.empty() ? m_wpModel.lock()->GetAnimation(0) : m_wpModel.lock()->GetAnimation(m_nowAnimatorData.animeName);
	const std::shared_ptr<Math::KdAnimationData> nextData = _view.empty() ? m_wpModel.lock()->GetAnimation(0) : m_wpModel.lock()->GetAnimation(_view);

	if (nextData == nullptr)return;

	m_nowAnimatorData.animeName = nextData->m_name;
	m_nowAnimatorData.isLoop = _isLoop;
	m_nowAnimatorData.speed = _speed;
	m_nowAnimatorData.isComplement = _isComplement;
	m_nowAnimatorData.complementSpeed = _time;

	if (postData == nextData)
	{
		m_nowAnimatorData.isComplement = false;
	}

	if (m_nowAnimatorData.isComplement)
	{
		m_animator.CreateBonesTrans(
			m_postNodes.pos
			, m_postNodes.rot
			, m_postNodes.scale
		);
		//m_animator.CreateBonesTrans(m_postNodes);
	}

	m_animator.SetAnimation(nextData);

	if (m_nowAnimatorData.isComplement)
	{
		m_animator.CreateBonesTrans(
			m_nextNodes.pos
			, m_nextNodes.rot
			, m_nextNodes.scale
		);
		//m_animator.CreateBonesTrans(m_nextNodes);
	}


	m_isChanging = m_nowAnimatorData.isComplement;
	m_timeCnt = 0;
	m_animator.m_time = 0.f;
}
void ModelAnimatorComponent::ChangeAnime(const AnimatorData& _pData)
{
	if (m_wpModel.expired())return;
	const std::shared_ptr<Math::KdAnimationData> postData = m_nowAnimatorData.animeName.empty() ? m_wpModel.lock()->GetAnimation(0) : m_wpModel.lock()->GetAnimation(m_nowAnimatorData.animeName);
	const std::shared_ptr<Math::KdAnimationData> nextData = _pData.animeName.empty() ? m_wpModel.lock()->GetAnimation(0) : m_wpModel.lock()->GetAnimation(_pData.animeName);

	if (nextData == nullptr)return;

	m_nowAnimatorData = _pData;
	if (nextData == nextData)
	{
		m_nowAnimatorData.isComplement = false;
	}

	if (m_nowAnimatorData.isComplement)
	{
		m_animator.CreateBonesTrans(
			m_postNodes.pos
			, m_postNodes.rot
			, m_postNodes.scale
		);
		//m_animator.CreateBonesTrans(m_postNodes);
	}

	m_animator.SetAnimation(nextData);

	if (m_nowAnimatorData.isComplement)
	{
		m_animator.CreateBonesTrans(
			m_nextNodes.pos
			, m_nextNodes.rot
			, m_nextNodes.scale
		);
		//m_animator.CreateBonesTrans(m_nextNodes);
	}

	m_isChanging = m_nowAnimatorData.isComplement;
	m_timeCnt = 0;
	m_animator.m_time = 0.f;
}

const std::shared_ptr<Math::KdAnimationData> ModelAnimatorComponent::GetAnime(std::string_view _view)
{
	return m_wpModel.lock()->GetAnimation(_view);
}

bool ModelAnimatorComponent::is_End()
{
	return m_animator.IsAnimationEnd();
}

bool ModelAnimatorComponent::is_Playing()
{
	return !m_animator.IsAnimationEnd();
}

void ModelAnimatorComponent::AdvanceTime()
{
	if (!m_animator.m_spAnimation) { return; }
	const float AddFramePow = JTN::GetEngine()->GetFPSController()->GetDeltaTime() * JTN::GetEngine()->GetFPSController()->GetMaxFPS();

	if (m_isChanging) {
		//アニメーション切り替え前と切り替え直後ノードの行列補間を実行する
		auto& Nodes = m_wpModel.lock()->WorkNodes();
		for (int i = 0; i < m_nextNodes.size(); i++)
		{
			Math::Matrix trans
				= Math::Matrix::CreateTranslation(
					Math::Vector3::Lerp(
						m_postNodes.pos[i], m_nextNodes.pos[i], m_timeCnt
					)
				);

			Math::Matrix rota
				= Math::Matrix::CreateFromQuaternion(
					Math::Quaternion::Slerp(
						m_postNodes.rot[i], m_nextNodes.rot[i], m_timeCnt
					)
				);

			Math::Matrix scale
				= Math::Matrix::CreateScale(
					Math::Vector3::Lerp(
						m_postNodes.scale[i], m_nextNodes.scale[i], m_timeCnt
					)
				);

			Nodes[i].m_localTransform
				= scale * rota * trans;
		}

		// アニメーションのフレームを進める
		m_timeCnt += m_nowAnimatorData.complementSpeed * AddFramePow;

		// アニメーションデータの最後のフレームを超えたら
		if (m_timeCnt > 1.0f)
		{
			m_isChanging = false;
		}
	}
	else
	{
		// アニメーションデータの最後のフレームを超えたら
		if (m_animator.m_time >= m_animator.m_spAnimation->m_maxLength)
		{
			if (m_nowAnimatorData.isLoop)
			{
				// アニメーションの最初に戻る（ループさせる
				m_animator.m_time = 0.0f;
			}
			else
			{
				return;
			}
		}

		// 全てのアニメーションノード（モデルの行列を補間する情報）の行列補間を実行する
		std::vector<IModelWork::Node>& rNodes = m_wpModel.lock()->WorkNodes();
		for (auto& rAnimNode : m_animator.m_spAnimation->m_nodes)
		{
			// 対応するモデルノードのインデックス
			UINT idx = rAnimNode.m_nodeOffset;

			auto prev = rNodes[idx].m_localTransform;

			// アニメーションデータによる行列補間
			rAnimNode.Interpolate(rNodes[idx].m_localTransform, m_animator.m_time);

			prev = rNodes[idx].m_localTransform;
		}

		// アニメーションのフレームを進める
		m_animator.m_time += AddFramePow * m_nowAnimatorData.speed;
	}
}

bool ModelAnimatorComponent::ImGuiAnimeSet(std::string_view _label, std::string& _str)
{
	if (!this || m_wpModel.expired())return false;

	bool edit = false;
	if (EDITOR_COMMAND("BeginCombo", _label.data(), &_str))
	{
		if (m_wpModel.lock())
		{
			for (auto& it : m_wpModel.lock()->GetData()->GetAnimations())
			{
				if (EDITOR_COMMAND("Selectable", it->m_name.data()))
				{
					_str = it->m_name;
					edit = true;
				}
			}
		}
		EDITOR_COMMAND("EndCombo");
	}
	return edit;
}

bool ModelAnimatorComponent::ImGuiAnimeData(AnimatorData& _data)
{
	bool edited = false;

	EDITOR_COMMAND("PushID", &_data);

	if (EDITOR_COMMAND("TreeNode", "AnimetorData"))
	{

		edited |= ImGuiAnimeSet("animeName", _data.animeName);
		edited |= EDITOR_COMMAND("DragFloat", "speed", &_data.speed);

		edited |= EDITOR_COMMAND("Checkbox", "isLoop", &_data.isLoop);

		edited |= EDITOR_COMMAND("Checkbox", "isComplement", &_data.isComplement);
		EDITOR_COMMAND("SameLine");
		edited |= EDITOR_COMMAND("DragFloat", "complementSpeed", &_data.complementSpeed);


		EDITOR_COMMAND("TreePop");
	}

	EDITOR_COMMAND("PopID");

	return edited;
}

void ModelAnimatorComponent::Serialize_AnimatorData(std::string_view _key, nlohmann::json& _json, const AnimatorData& _data)
{
	auto& dataJson = _json[_key.data()];
	dataJson["name"] = _data.animeName;
	dataJson["speed"] = _data.speed;
	dataJson["isLoop"] = _data.isLoop;
	dataJson["isComplement"] = _data.isComplement;
	dataJson["complementSpeed"] = _data.complementSpeed;
}

void ModelAnimatorComponent::Deserialize_AnimatorData(std::string_view _key, const nlohmann::json& _json, AnimatorData& _data)
{
	if (_json.contains(_key.data()))
	{
		auto& dataJson = _json[_key.data()];
		_data.animeName = dataJson.value("name", "");
		_data.speed = dataJson.value("speed", 60.f);
		_data.isLoop = dataJson.value("isLoop", false);
		_data.isComplement = dataJson.value("isComplement", false);
		_data.complementSpeed = dataJson.value("complementSpeed", 10.f);
	}
}
