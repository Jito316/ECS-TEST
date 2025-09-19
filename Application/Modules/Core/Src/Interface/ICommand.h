#pragma once
#include "Core.h"

namespace JTN {
	namespace Message {
		class ITask;

		//=========================================
		// モジュール間の通信を行うクラス
		// データ駆動系
		//=========================================
		class CORE_API ICommandWorker
		{
		public:
			typedef std::unordered_map<std::string, std::shared_ptr<ITask>>TaskMap;
			typedef std::unordered_map<std::string, std::weak_ptr<TaskMap>> ModuleTaskMap;

			ICommandWorker() = default;
			virtual ~ICommandWorker() = default;

			virtual void RegisterTask(std::string_view _moduleName, std::shared_ptr<TaskMap> _taskMap) = 0;
			virtual void RegisterTask(std::string_view _moduleName, std::string_view _taskName, std::shared_ptr<ITask> _task) = 0;

			template<typename ...Values>
				bool Request(std::string_view _moduleName, std::string_view _taskName, Values... values)
			{
				void* array[] = { (void*)(values)... };
				return Request(_moduleName, _taskName, (void**)&array);
			}

			virtual bool Request(std::string_view _moduleName, std::string_view _taskName, void** values = nullptr) = 0;
		};

		class CORE_API ITask
		{
		public:
			ITask() = default;
			virtual ~ITask() = default;

			virtual bool Task(void** value = nullptr) = 0;
		};
	};
};