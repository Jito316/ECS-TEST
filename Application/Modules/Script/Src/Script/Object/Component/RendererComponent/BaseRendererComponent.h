#pragma once

class BaseRendererComponent
	:public Component
{
public:
	virtual void Awake()override;

	virtual void ImGuiUpdate();

	virtual void Deserialize(const nlohmann::json& _json);
	virtual void Serialize(nlohmann::json& _json);

	virtual void SetColor(const Math::Color& _color);
	virtual const Math::Color& GetColor() const;

protected:

	std::string	m_path					 = "";
	Math::Color m_color					 = kWhiteColor;
	UINT m_drawType						 = IRenderer::Lit;

	IRenderer* m_render					 = nullptr;
	IResource* m_resource				 = nullptr;
};
