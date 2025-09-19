#include "SquarePolygonRendererComponent.h"
#include "Script/Utility/Animation2D/Animation2D.h"

void SquarePolygonRendererComponent::Awake()
{
	BaseRendererComponent::Awake();
	m_animation = std::make_shared<Animation2D>();
	m_asset = m_resource->GetSquarePolygon("Asset/Textures/System/block.png");
}

void SquarePolygonRendererComponent::PreUpdate()
{
	m_asset->SetSplit(m_split);
	m_animation->PreUpdate(m_asset.get());
}

void SquarePolygonRendererComponent::RenderUpdate()
{
	if (m_bDirty) 
	{
		m_asset = m_resource->GetSquarePolygon(m_path);
		m_bDirty = false;
	}

	if (m_cameraFocus)
	{
		m_trans.lock()->SetLocalRotation(m_render->GetMainCamera()->GetCameraMatrix().ToEuler());
	}

	Animation2D::AnimeFrame frame = m_animation->GetAnimeFrame();
	m_asset->SetUVRect(frame.w, frame.h);

	if (!m_owner.lock()->GetHideFlg()) {
		m_render->Draw(
			m_asset->GetPolygon()
			, m_trans.lock()->GetWorldMatrix()
			, m_color
			, (IRenderer::DrawType)m_drawType
		);
	}
}

void SquarePolygonRendererComponent::ImGuiUpdate()
{
	if (EDITOR_COMMAND("SelectTexturePath", &m_path))
	{
		m_bDirty = true;
	}

	BaseRendererComponent::ImGuiUpdate();

	EDITOR_COMMAND("DragFloat3", "OffsetPos", &m_offsetPos.x);
	EDITOR_COMMAND("Checkbox", "CameraFocus", &m_cameraFocus);

	EDITOR_COMMAND("InputInt2", "Split w/h", m_split);
	m_animation->ImGuiUpdate();
}



void SquarePolygonRendererComponent::Deserialize(const nlohmann::json& _json)
{
	BaseRendererComponent::Deserialize(_json);

	m_split[0] = _json["splitW"];
	m_split[1] = _json["splitH"];
	m_animation->SetJson(_json["Animation"]);
	m_offsetPos = Utility::JsonHelper::InPutVec3(_json["OffsetPos"]);
	m_cameraFocus = _json["CameraFocus"];

	m_bDirty = true;
}

void SquarePolygonRendererComponent::Serialize(nlohmann::json& _json)
{
	BaseRendererComponent::Serialize(_json);

	_json["splitW"] = m_split[0];
	_json["splitH"] = m_split[1];
	_json["Animation"] = m_animation->Serialize();
	_json["OffsetPos"] = Utility::JsonHelper::OutPutVec3(m_offsetPos);
	_json["CameraFocus"] = m_cameraFocus;
}

void SquarePolygonRendererComponent::SetAnimeName(std::string _name)
{
	m_animation->ChangeAnime(_name);
}