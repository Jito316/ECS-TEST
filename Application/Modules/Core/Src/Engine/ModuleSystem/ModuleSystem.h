#pragma once
#include "Interface/IModuleSystem.h"

namespace JTN
{
	namespace Module
	{

		class ModuleSystem : public IModuleSystem
		{
		public:
			bool Load(const std::string& _moduleName)override;
			bool Remove(IModule* _pModule)override;
			void Release();

			void Updata();
			void CallEntyFunction(int _begin, int _end, const bool& _isStop = false);

			IModule* GetModule(const std::type_info& _id)override;
		private:
			IModule* GetNowEntryModule() override;

			bool Entry(ModuleSet* _pEntryModule);
			bool BeginEntry(ModuleSet* _pEntryModule);
			void ModuleEntry(std::pair<ModuleEntry_, std::function<void()>>)override;
			void EndEntry();
			void ClearEntry();

			ModuleSet* m_nowEntryModule = nullptr;
			ModuleSet* m_deleteModule = nullptr;
			std::map<ModuleEntry_, std::function<void()>> m_entryFunctions;
			std::queue<std::pair<ModuleEntry_, std::function<void()>>> m_entryFunctionsQueue;

			std::list<ModuleSet*> m_moduleList;
		};
	};
};

