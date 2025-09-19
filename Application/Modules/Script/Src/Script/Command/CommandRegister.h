#pragma once

namespace Command {
	void Register(std::shared_ptr<JTN::Message::CommandWorker::TaskMap>& _taskMap);
}