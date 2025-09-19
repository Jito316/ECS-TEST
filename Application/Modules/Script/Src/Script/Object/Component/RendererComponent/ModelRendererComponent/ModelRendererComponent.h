#pragma once
#include "Script/Object/Component/RendererComponent/BaseRendererComponent.h"

class ModelRendererComponent
	:public BaseRendererComponent
{
public:
	void Start()override;
	void RenderUpdate()override;

	void ImGuiUpdate()override;

	void Deserialize(const nlohmann::json& _json)override;
	void Serialize(nlohmann::json& _json)override;

	std::weak_ptr<IModelData> GetModelData() { return m_model ? m_model->GetData() : std::weak_ptr<IModelData>(); }
	void SetModelData(std::shared_ptr<IModelData> _data) { m_model->SetModelData(_data); }

	std::weak_ptr<IModelWork> GetModelWork() { return m_model; }
	void SetModelWork(std::shared_ptr<IModelWork> _data) { m_model = _data; }

	bool ImGuiSetNodeName(std::string_view _name, std::string& _str);
private:
	struct BaseRenderState
	{
		virtual void RenderUpdate(ModelRendererComponent& _model) = 0;
	};
	struct RenderModelWork : BaseRenderState
	{
		void RenderUpdate(ModelRendererComponent& _model)override;
	};
	struct RenderModelData : BaseRenderState
	{
		void RenderUpdate(ModelRendererComponent& _model)override;
	};

	std::shared_ptr <BaseRenderState> m_renderState = nullptr;

	std::shared_ptr<IModelWork> m_model = nullptr;
	bool m_dirty = false;
};
