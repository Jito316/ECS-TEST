#include "Component.h"
#include "Interface/Object/GameObject/IGameObject.h"

void Component::SetOwner(std::weak_ptr<IGameObject> _owner)
{
	m_owner = _owner;
	if (m_owner.expired()) {
		m_trans.reset();
		return;
	}
	m_trans = m_owner.lock()->GetTransform();
}

bool Component::GetEnable() const
{
	return m_enable;
}

bool& Component::WorkEnable()
{
	return m_enable;
}

void Component::SetEnable(bool _active)
{
	m_enable = _active;
}

bool Component::GetDestroy() const
{
	return m_bDestroy;
}


void Component::Destroy()
{
	if (!m_bDestroy) 
	{
		OnDestroy();
	}
	m_bDestroy = true;
}