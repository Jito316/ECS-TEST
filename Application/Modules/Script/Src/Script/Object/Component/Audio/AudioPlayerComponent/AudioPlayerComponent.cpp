#include "AudioPlayerComponent.h"

void AudioPlayerComponent::Awake()
{
	m_player = GETMODULE(IAudio);
}

void AudioPlayerComponent::Start()
{
	if (!m_player) {
		SetEnable(false);
	}
}

void AudioPlayerComponent::Update()
{
	if (m_soundInstance.index())
	{
		auto& temp = std::get<std::shared_ptr<ISoundInstance3D>>(m_soundInstance);
		if (temp && temp->IsPlaying()) {
			temp->SetPos(m_trans.lock()->GetWorldPosition());
		}
	}
}


void AudioPlayerComponent::ImGuiUpdate()
{
	EDITOR_COMMAND("SelectWavPath", &m_soundPath);
	EDITOR_COMMAND("CheckBox", "isRoop", &m_isRoop);
	EDITOR_COMMAND("DragFloat", "volume", &m_volume);
	EDITOR_COMMAND("DragFloat", "pich", &m_pich);
	EDITOR_COMMAND("DragFloat", "curveDistanceScaler", &m_curveDistanceScaler);
}

std::shared_ptr<ISoundInstance> AudioPlayerComponent::Play()
{
	if (auto& temp = std::get<std::shared_ptr<ISoundInstance>>(m_soundInstance)) {
		if (m_isRoop || temp->IsPlaying()) {
			temp->Play(m_isRoop);
		}
		return temp;
	}

	auto temp = m_player->Play(m_soundPath, m_isRoop);
	temp->SetPitch(m_pich);
	temp->SetVolume(m_volume);
	m_soundInstance = temp;

	return temp;
}

std::shared_ptr<ISoundInstance3D> AudioPlayerComponent::Play3D()
{
	if (auto& temp = std::get<std::shared_ptr<ISoundInstance3D>>(m_soundInstance)) {
		if (m_isRoop || temp->IsPlaying()) {
			temp->Play(m_isRoop);
		}
		return temp;
	}

	auto temp = m_player->Play3D(m_soundPath, m_trans.lock()->GetWorldPosition(), m_isRoop);
	temp->SetPitch(m_pich);
	temp->SetVolume(m_volume);
	temp->SetCurveDistanceScaler(m_curveDistanceScaler);
	m_soundInstance = temp;

	return temp;
}

std::shared_ptr<ISoundInstance> AudioPlayerComponent::GetInstance()
{
	return std::get<std::shared_ptr<ISoundInstance>>(m_soundInstance);
}

std::shared_ptr<ISoundInstance3D> AudioPlayerComponent::GetInstance3D()
{
	return  std::get<std::shared_ptr<ISoundInstance3D>>(m_soundInstance);
}

void AudioPlayerComponent::Deserialize(const nlohmann::json& _json)
{
	m_soundPath = _json["soundPath"];
	m_isRoop = _json["isRoop"];
	m_volume = _json["volume"];
	m_pich = _json["pich"];
	m_curveDistanceScaler = _json["curveDistanceScaler"];
}

void AudioPlayerComponent::Serialize(nlohmann::json& _json)
{
	_json["soundPath"] = m_soundPath;
	_json["isRoop"] = m_isRoop;
	_json["volume"] = m_volume;
	_json["pich"] = m_pich;
	_json["curveDistanceScaler"] = m_curveDistanceScaler;
}
