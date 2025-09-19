#pragma once

namespace Command {
	class CreateObject : public JTN::Message::ITask {
	public:
		bool Task(void** value = nullptr);
	};
}
