#include "TextureRendererComponent.h"
#include "Script/Utility/Animation2D/Animation2D.h"

void TextureRendererComponent::Awake()
{
	BaseRendererComponent::Awake();
	m_animation = std::make_shared<Animation2D>();
	m_rect = { 0,0,64,64 };
}

void TextureRendererComponent::PreUpdate()
{
	m_animation->PreUpdate(m_rect);
}

void TextureRendererComponent::RenderUpdate()
{
	if (m_bDirty) 
	{
		m_tex = m_resource->GetTexture(m_path);
		m_rect = { 0,0,(LONG)m_tex->GetWidth(),(LONG)m_tex->GetHeight() };
		m_bDirty = false;
	}

	m_animation->PreUpdate(m_rect);
	if (!m_owner.lock()->GetHideFlg())
	{
		m_render->Draw(m_tex.get(), m_trans.lock()->GetWorldMatrix(), m_rect, m_color);
	}
}

void TextureRendererComponent::ImGuiUpdate()
{
	if (EDITOR_COMMAND("SelectTexturePath", &m_path))
	{
		m_bDirty = true;
	}

	EDITOR_COMMAND("ColorEdit", "color", &m_color);
	EDITOR_COMMAND("DragLong4", "rect", &m_rect.x);

	m_animation->ImGuiUpdate();
}

void TextureRendererComponent::Deserialize(const nlohmann::json& _json)
{
	BaseRendererComponent::Deserialize(_json);
	m_rect = Utility::JsonHelper::InputRect(_json["rect"]);
	m_animation->SetJson(_json["animation"]);

	m_tex = m_resource->GetTexture(m_path);
}

void TextureRendererComponent::Serialize(nlohmann::json& _json)
{
	BaseRendererComponent::Serialize(_json);
	_json["rect"] = Utility::JsonHelper::OutPutRect(m_rect);
	_json["animation"] = m_animation->Serialize();
}

void TextureRendererComponent::SetTexture(std::string_view _path)
{
	m_path = _path.data();
	m_bDirty = true;
}
