#pragma once
#include "Interface/Manager/ComponentFactory/IComponentFactory.h"
#include "Script/Object/Component/BuildInComponentRegister.h"
#include "User/UserComponentRegister.h"

inline void AllComponentRegister(IComponentFactory& _factory) {
	BuildInComponentRegister(_factory);
	UserComponentRegister(_factory);
}