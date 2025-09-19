#pragma once
#include "Script/Object/Component/RendererComponent/BaseRendererComponent.h"

class Animation2D;
class RenderManager;

class SquarePolygonRendererComponent
	:public BaseRendererComponent
{
public:
	void Awake()override;
	void PreUpdate()override;
	void RenderUpdate()override;
	void ImGuiUpdate()override;

	void Deserialize(const nlohmann::json& _json)override;
	void Serialize(nlohmann::json& _json)override;

	std::weak_ptr<ISquarePolygon>GetSquarePolygon() { return m_asset; };
	void SetAnimeName(std::string _name);
private:

	std::shared_ptr<ISquarePolygon>		m_asset = nullptr;

	Math::Vector3						m_offsetPos = Math::Vector3::Zero;
	bool								m_cameraFocus = false;

	int									m_split[2] = { 1,1 };
	std::shared_ptr<Animation2D>		m_animation = nullptr;

	bool m_bDirty = false;
};
