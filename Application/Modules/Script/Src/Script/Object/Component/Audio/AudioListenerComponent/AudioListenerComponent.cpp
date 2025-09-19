#include "AudioListenerComponent.h"

void AudioListenerComponent::Start()
{
	if (m_player = GETMODULE(IAudio)) {
		return;
	}
	SetEnable(false);
}

void AudioListenerComponent::PostUpdate()
{
	//m_player->SetAlive():
}
