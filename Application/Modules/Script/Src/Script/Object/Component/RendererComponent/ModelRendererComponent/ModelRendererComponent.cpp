#include "ModelRendererComponent.h"

void ModelRendererComponent::Start()
{
	if (m_dirty)
	{
		m_model = GETMODULE(IResource)->GetModelWork(m_path);
		if (!m_model)
		{
			m_model = GETMODULE(IResource)->GetModelWork("Asset/Data/Model/Box/Box.gltf");
		}

		auto temp = m_model->GetAnimation(0);
		if (temp)
		{
			Math::KdAnimator animator;
			animator.SetAnimation(temp);
			m_renderState = std::make_shared<RenderModelWork>();
		}
		else m_renderState = std::make_shared<RenderModelData>();

		m_dirty = false;
	}
}

void ModelRendererComponent::RenderUpdate()
{
	Start();

	if (!m_owner.lock()->GetHideFlg())
	{
		if (m_model) 
		{
			m_renderState->RenderUpdate(*this);
		}
	}
}

void ModelRendererComponent::ImGuiUpdate()
{
	if (EDITOR_COMMAND("SelectModelPath", &m_path))
	{
		m_dirty = true;
	}

	BaseRendererComponent::ImGuiUpdate();
}

void ModelRendererComponent::Deserialize(const nlohmann::json& _json)
{
	BaseRendererComponent::Deserialize(_json);
	m_dirty = true;
	Start();
}

void ModelRendererComponent::Serialize(nlohmann::json& _json)
{
	BaseRendererComponent::Serialize(_json);
}

bool ModelRendererComponent::ImGuiSetNodeName(std::string_view _name, std::string& _str)
{
	std::vector<std::string> vec;
	std::transform(
		m_model->GetNodes().begin()
		, m_model->GetNodes().end()
		, std::back_inserter(vec)
		, [](auto& _it) {return _it.m_name; }
	);

	return EDITOR_COMMAND("Combo", _name.data(), &vec, &_str);
}

void ModelRendererComponent::RenderModelWork::RenderUpdate(ModelRendererComponent& _model)
{
	GETMODULE(IRenderer)->Draw(
		_model.m_model.get()
		, _model.m_trans.lock()->GetWorldMatrix()
		, _model.m_color
		, (IRenderer::DrawType)_model.m_drawType
	);
}

void ModelRendererComponent::RenderModelData::RenderUpdate(ModelRendererComponent& _model)
{
	GETMODULE(IRenderer)->Draw(
		_model.m_model->GetData().get()
		, _model.m_trans.lock()->GetWorldMatrix()
		, _model.m_color
		, (IRenderer::DrawType)_model.m_drawType
	);
}
