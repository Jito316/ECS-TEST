#include "Command_CreateEntity.h"
#include "Script/ECS/Manager/ECSManager.h"
#include "Script/ECS/Component/Component.h"

namespace ScriptCommand
{

	bool CreateEntity::Task(void** values)
	{
		if (values)
		{
			return Task(*static_cast<std::string*>(values[0]));
		}
		return false;
	}

	bool CreateEntity::Task(std::string& _name)
	{
		auto& script = GETSCRIPT;
		Entity entity = script.CreateEntity();
		script.AddComponent<GameObject>(entity);
		script.AddComponent<Transform>(entity);
		return false;
	}


}