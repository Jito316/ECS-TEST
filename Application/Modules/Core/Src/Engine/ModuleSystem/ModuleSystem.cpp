#include "ModuleSystem.h"
#include "Interface/IEngine.h"
#include "Interface/ILog.h"
#include "Interface/IModule.h"
#include "Utility/DllLoader/DllLoader.h"
#include "Utility/Config/Config.h"
#include "Engine/Command/Command.h"

namespace JTN
{
	namespace Module
	{
		struct IModuleSystem::ModuleSet
		{
			typedef void (*CreateFnc)(JTN::Module::IModule**, const std::type_info**);
			typedef void (*DeleteFnc)(JTN::Module::IModule*);

			const std::type_info* m_id = nullptr;
			JTN::Module::IModule* m_module = nullptr;

			DllLoader m_dllLoader;

			std::list<ModuleEntry_>m_pEntryPoints;

			bool Load(const std::string& _moduleName)
			{
				// ================= dllの読み込み =================
#ifdef _DEBUG
				std::string dllpath = "Modules/" + _moduleName + "/Win32/Debug/" + _moduleName + ".dll";
#else 
				std::string dllpath = "Modules/" + _moduleName + "/Win32/Release/" + _moduleName + ".dll";
#endif // _DEBUG

				if (!m_dllLoader.Load(dllpath.data())) {
					ADDLOG(("LoadFailed : " + _moduleName + ".dll").data())(Log::Error);
					m_dllLoader.Release();
					return false;
				}

				// ================= モジュールの生成関数の取得 =================
				CreateFnc createFunc = (CreateFnc)m_dllLoader.GetFunction("CreateModule");
				if (!createFunc) {
					ADDLOG(("CreateFailed_CreateFunction : " + _moduleName + ".dll").data())(Log::Error);
					m_dllLoader.Release();
					return false;
				}

				// ================= モジュールのインスタンス生成 =================
				if (createFunc(&m_module, &m_id); (m_module && m_id) == false)
				{
					ADDLOG(("CreateFailed_Instance : " + _moduleName + ".dll").data())(Log::Error);
					m_dllLoader.Release();
					return false;
				}

				return true;
			}

			void Release()
			{
				DeleteFnc deleteFunc = (DeleteFnc)m_dllLoader.GetFunction("DeleteModule");
				if (!deleteFunc) {
					ADDLOG("CreateFailed_DeleteModule")(Log::Error);
					return;
				}

				deleteFunc(m_module);
				m_module = nullptr;

				m_dllLoader.Release();
			}

		};

		bool ModuleSystem::Load(const std::string& _moduleName)
		{
			ModuleSet* moduleSet = new ModuleSet;

			if (!moduleSet->Load(_moduleName))
			{
				delete moduleSet;
				return false;
			}

			if (!Entry(moduleSet))
			{
				ADDLOG(("EntryFailed : " + _moduleName).data())(Log::Error);
				moduleSet->Release();
				delete moduleSet;
				return false;
			}

			ADDLOG((std::string("Entry : ") + (std::string(moduleSet->m_id->name()).erase(0, 7))).data());
			m_moduleList.push_back(moduleSet);
			return true;
		}

		bool ModuleSystem::Remove(IModule* _pModule)
		{
			if (m_deleteModule)return false;

			ModuleSet* temp = nullptr;
			for (auto& it : m_moduleList)
			{
				if (it->m_module == _pModule)
				{
					temp = it;
					break;
				}
			}

			m_deleteModule = temp;
			return temp;
		}

		void ModuleSystem::Release()
		{
			m_entryFunctions.clear();

			// nlohmann::json data = nlohmann::json::array();
			for (auto it = m_moduleList.rbegin()
				; it != m_moduleList.rend()
				; it++) {
				auto& modulePack = *it;
				std::string temp = std::string((*it)->m_id->name()).erase(0, 7);
				// data.push_back(temp);
				modulePack->Release();
				delete modulePack;
				modulePack = nullptr;
			}


			// File::Config config; config.Save("ModuleNames", data);
			m_moduleList.clear();
		}

		void ModuleSystem::Updata()
		{
			if (m_deleteModule)
			{
				for (auto& it : m_deleteModule->m_pEntryPoints)
				{
					if (it > ModuleEntry_Finalize)
					{
						m_entryFunctions[it]();
					}
					m_entryFunctions.erase(it);
				}

				((JTN::Message::CommandWorker*)JTN::GetEngine()->GetCommandWorker())->RemoveTask(std::string(m_deleteModule->m_id->name()).erase(0, 7));
				m_deleteModule->Release();
				m_moduleList.erase(std::ranges::find(m_moduleList, m_deleteModule));
				delete m_deleteModule;
				m_deleteModule = nullptr;
			}
		}

		void ModuleSystem::CallEntyFunction(int _begin, int _end, const bool& _isStop)
		{
			for (int i = _begin
				; i != _end; ++i) {
				if (_isStop)break;
				auto it = m_entryFunctions.find((ModuleEntry_)i);
				if (it == m_entryFunctions.end())continue;

				it->second();
			}
		}

		IModule* ModuleSystem::GetModule(const std::type_info& _id)
		{
			for (auto& it : m_moduleList) {
				if (*it->m_id == _id)return it->m_module;
			}

			ADDLOG(u8"指定されたモジュールが存在しません")(Log::Error);
			return nullptr;
		}

		IModule* ModuleSystem::GetNowEntryModule()
		{
			return  m_nowEntryModule->m_module;
		}

		bool ModuleSystem::Entry(ModuleSet* _pEntryModule)
		{
			if (BeginEntry(_pEntryModule))
			{
				if (!_pEntryModule->m_module->Entry())
				{
					ClearEntry();
					return false;
				}
				EndEntry();
			}

			return true;
		}
		bool ModuleSystem::BeginEntry(ModuleSet* _pEntryModule)
		{
			if (m_nowEntryModule) {
				return false;
			}

			m_nowEntryModule = _pEntryModule;
			return true;
		}
		void ModuleSystem::ModuleEntry(std::pair<ModuleEntry_, std::function<void()>> _value)
		{
			if (m_nowEntryModule)
			{
				m_entryFunctionsQueue.push(_value);
			}
		}
		void ModuleSystem::EndEntry()
		{
			if (m_nowEntryModule) {
				while (m_entryFunctionsQueue.size())
				{
					m_nowEntryModule->m_pEntryPoints.push_back(m_entryFunctionsQueue.front().first);
					m_entryFunctions.emplace(m_entryFunctionsQueue.front());
					m_entryFunctionsQueue.pop();
				}
				m_nowEntryModule = nullptr;
			}
		}
		void ModuleSystem::ClearEntry()
		{
			m_nowEntryModule = nullptr;
			while (m_entryFunctionsQueue.size())
			{
				m_entryFunctionsQueue.pop();
			}
		}
	};
};