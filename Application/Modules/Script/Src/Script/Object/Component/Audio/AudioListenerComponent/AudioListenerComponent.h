#pragma once
class AudioListenerComponent
	:public Component 
{
public:
	virtual void Start();
	virtual void PostUpdate();
protected:
	IAudio* m_player = nullptr;
};