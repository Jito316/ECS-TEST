#include "BaseRendererComponent.h"

void BaseRendererComponent::Awake()
{
	m_render = GETMODULE(IRenderer);
	m_resource = GETMODULE(IResource);
}

void BaseRendererComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("ColorEdit", "color", &m_color);
	EDITOR_COMMAND("BitEditor_DrawType", "drawType", &m_drawType);
}

void BaseRendererComponent::Deserialize(const nlohmann::json& _json)
{
	m_color = Utility::JsonHelper::InPutColor(_json["color"]);
	m_drawType = _json["drawType"];
	m_path = _json["path"];
}

void BaseRendererComponent::Serialize(nlohmann::json& _json)
{
	_json["color"] = Utility::JsonHelper::OutPutColor(m_color);
	_json["drawType"] = m_drawType;
	_json["path"] = m_path;
}

void BaseRendererComponent::SetColor(const Math::Color& _color)
{
	m_color = _color;
}

const Math::Color& BaseRendererComponent::GetColor() const
{
	return m_color;
}
