#pragma once
#include "Interface/ICommand.h"

namespace JTN {
	namespace Message {
		//=========================================
		// モジュール間の通信を行うクラス
		// データ駆動系
		//=========================================
		class CommandWorker : public ICommandWorker
		{
		public:
			CommandWorker() = default;
			~CommandWorker() override = default;

			void RegisterTask(std::string_view _moduleName, std::shared_ptr<TaskMap> _taskMap)override;
			void RegisterTask(std::string_view _moduleName, std::string_view _taskName, std::shared_ptr<ITask> _task)override;
			void RemoveTask(std::string_view _moduleName);

			bool Request(std::string_view _moduleName, std::string_view _taskName, void** values)override;

			void Release();
		private:
			ModuleTaskMap m_tasks;
		};
	}
}