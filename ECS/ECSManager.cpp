#include "System.h"
#include "ECSManager.h"

void ECSManager::excute()
{
	for (auto& system : m_systems)
	{
		system->PreUpdate();
	}
	for (auto& system : m_systems)
	{
		system->Update();
	}
	for (auto& system : m_systems)
	{
		system->PostUpdate();
	}
}
