#include "Command_CreateObject.h"
namespace Command {
	bool CreateObject::Task(void** value) {

		nlohmann::json json;
		if (value[0])
		{
			std::string_view path = (const char*)value[0];
			json = Utility::JsonHelper::InputJson(path.data());
		}

		std::list<std::shared_ptr<IGameObject>>* results = (std::list<std::shared_ptr<IGameObject>>*)value[3];
		GameObject::CreateObject(
			json,
			value[1] ? (*(std::weak_ptr<IGameObject>*)value[1]) : std::weak_ptr<IGameObject>(),
			value[2] ? (std::list<std::shared_ptr<IGameObject>>*)value[2] : nullptr
			);
		return true;
	}
}