#pragma once
class Animation2D;
#include "Script/Object/Component/RendererComponent/BaseRendererComponent.h"

class TextureRendererComponent
	:public BaseRendererComponent
{
public:
	virtual void Awake()override;

	virtual void PreUpdate()override;
	virtual void RenderUpdate()override;

	virtual void ImGuiUpdate()override;
	virtual void Deserialize(const nlohmann::json& _json)override;
	virtual void Serialize(nlohmann::json& _json)override;

	virtual void SetTexture(std::string_view _path);
	virtual std::weak_ptr<ITexture>GetTexture() { return m_tex; };
	virtual std::shared_ptr<Animation2D>GetAnime() { return m_animation; };

	virtual const Math::Rectangle& GetRect() const { return m_rect; }
	virtual void SetRect(const Math::Rectangle& _rect) { m_rect = _rect; }

protected:

	std::shared_ptr<ITexture> m_tex;
	Math::Rectangle m_rect;
	std::shared_ptr<Animation2D> m_animation;

	bool m_bDirty = false;

};
