#include "CommandRegister.h"
#include "Command_CreateObject/Command_CreateObject.h"
#include "Command_SelectObject/Command_SelectObject.h"

void Command::Register(std::shared_ptr<JTN::Message::CommandWorker::TaskMap>& _taskMap)
{
	if (!_taskMap)_taskMap = std::make_shared<JTN::Message::CommandWorker::TaskMap>();
	_taskMap->emplace("CreateObject", std::make_shared<CreateObject>());
	_taskMap->emplace("SelectGameObject", std::make_shared<SelectGameObject>());
	JTN::GetEngine()->GetCommandWorker()->RegisterTask("Script", _taskMap);
}
