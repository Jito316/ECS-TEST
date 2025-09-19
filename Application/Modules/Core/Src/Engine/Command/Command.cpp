#include "Command.h"
#include "Interface/IEngine.h"

namespace JTN {
	namespace Message {
		void CommandWorker::RegisterTask(std::string_view _moduleName, std::shared_ptr<TaskMap> _taskMap)
		{
			m_tasks.emplace(_moduleName.data(), _taskMap);
		}
		void CommandWorker::RegisterTask(std::string_view _moduleName, std::string_view _taskName, std::shared_ptr<ITask> _task)
		{
			m_tasks.emplace(_moduleName.data(), std::make_shared<TaskMap>());
			m_tasks[_moduleName.data()].lock()->emplace(_taskName.data(), _task);
		}

		void CommandWorker::RemoveTask(std::string_view _moduleName)
		{
			m_tasks.erase(_moduleName.data());
		}

		bool CommandWorker::Request(std::string_view _moduleName, std::string_view _taskName, void** values)
		{
			if (auto moduleName = m_tasks.find(_moduleName.data()); moduleName != m_tasks.end())
			{
				if (moduleName->second.lock())
				{
					if (auto task = moduleName->second.lock()->find(_taskName.data()); task != moduleName->second.lock()->end())
					{
						return task->second->Task(values);
					}
					return false;
				}
			}
			return false;
		}

		void CommandWorker::Release()
		{
			m_tasks.clear();
		}
	};
};