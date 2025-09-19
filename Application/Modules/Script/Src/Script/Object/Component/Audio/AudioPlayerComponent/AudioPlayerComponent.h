#pragma once
class AudioPlayerComponent
	:public Component
{
public:
	virtual void Awake()override;
	virtual void Start()override;

	virtual void Update()override;

	virtual void ImGuiUpdate()override;

	virtual std::shared_ptr<ISoundInstance> Play();
	virtual std::shared_ptr<ISoundInstance3D> Play3D();

	virtual std::shared_ptr<ISoundInstance>  GetInstance();
	virtual std::shared_ptr<ISoundInstance3D>GetInstance3D();

	virtual void SetIsRoop(const bool  _value) { m_isRoop = _value; }
	virtual void SetVolume(const float _value) { m_volume = _value; }
	virtual void SetPich(const float _value) { m_pich = _value; }
	virtual void SetCurveDistanceScaler(const float _value) { m_curveDistanceScaler = _value; }

	virtual void Deserialize(const nlohmann::json& _json)override;
	virtual void Serialize(nlohmann::json& _json)override;
protected:
	IAudio* m_player = nullptr;

	std::string m_soundPath;

	typedef std::variant< std::shared_ptr<ISoundInstance>, std::shared_ptr<ISoundInstance3D>> SoundInstance;
	SoundInstance m_soundInstance;
	bool  m_isRoop = false;
	float m_volume = 1.f;
	float m_pich = .5f;
	float m_curveDistanceScaler = 0.5f;
};