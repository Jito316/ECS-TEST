#pragma once
#include "Core.h"

namespace JTN
{
	class IFPSController;
	namespace Log {class ISystem;}
	namespace File { class IConfigSystem; }
	namespace Message { class ICommandWorker; }
	namespace Module { class IModule; }
	namespace Module { class IModuleSystem; }

	//=========================
	// ランタイム管理クラス
	//=========================
	class CORE_API IEngine
	{
	public:
		virtual bool is_End() = 0;
		virtual void End() = 0;

		virtual const bool is_Debug()const = 0;
		virtual void SetDebug(const bool _flg) = 0;

		virtual IFPSController* GetFPSController() = 0;
		virtual Log::ISystem* GetLog() = 0;
		virtual File::IConfigSystem* GetConfig() = 0;
		virtual Message::ICommandWorker* GetCommandWorker() = 0;
		virtual Module::IModuleSystem* GetModuleSystem() = 0;

		template<class T>
		T* GetModule() {
			static_assert(std::is_base_of<Module::IModule, T>::value && "モジュール以外を指定しました");

			auto pModule = GetModule(typeid(T));

			if (pModule)
			{
				return static_cast<T*>(pModule);
			}

			return nullptr;
		}

	protected:
		virtual Module::IModule* GetModule(const std::type_info& _info) = 0;
	};
};

#define GETMODULE(NAME) JTN::GetEngine()->GetModule<NAME>()
#define ADDLOG(STR,...)	JTN::GetEngine()->GetLog()->AddLog(##__FILE__,(const char*)##STR,##__VA_ARGS__)