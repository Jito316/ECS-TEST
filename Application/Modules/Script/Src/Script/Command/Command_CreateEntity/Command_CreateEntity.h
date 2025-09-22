#pragma once
namespace ScriptCommand
{
	class CreateEntity :public  JTN::Message::ITask
	{
		bool Task(void** values = nullptr) override;
		bool Task(std::string& _name);
	};
}