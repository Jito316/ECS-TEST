#pragma once
class CameraComponent
	:public Component 
{
public:
	virtual void Awake()override; 
	virtual void Start()override; 
	virtual void ImGuiUpdate()override;

	virtual void RenderUpdate()override;

	virtual void Deserialize(const nlohmann::json& _json)override;
	virtual void Serialize(nlohmann::json& _json)override;

	virtual void OnDestroy()override;
	virtual std::shared_ptr<ICamera> GetCamera() { return m_camera; };

private:
	int m_priority = 0;
	std::shared_ptr<ICamera> m_camera = nullptr;
};