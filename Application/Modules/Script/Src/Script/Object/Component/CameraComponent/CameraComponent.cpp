#include "CameraComponent.h"


void CameraComponent::Awake()
{
	m_camera = std::shared_ptr<ICamera>(GETMODULE(IRenderer)->CreateCamera());
}

void CameraComponent::Start()
{
}


void CameraComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("InputInt", "Priority", &m_priority);
}

void CameraComponent::RenderUpdate()
{
	m_camera->SetCameraMatrix(m_trans.lock()->GetWorldMatrix());
	m_camera->Register();
}

void CameraComponent::Deserialize(const nlohmann::json& _json)
{
	m_priority = _json["Priority"];
}

void CameraComponent::Serialize(nlohmann::json& _json)
{
	_json["Priority"] = m_priority;
}

void CameraComponent::OnDestroy()
{
	m_camera.reset();
}
