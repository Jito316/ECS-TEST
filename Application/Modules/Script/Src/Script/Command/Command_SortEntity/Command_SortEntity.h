#pragma once

namespace ScriptCommand {
	class Command_SortEntity : public JTN::Message::ITask {
	public:
		bool Task(void** value = nullptr);
	};
}
