#pragma once
#include "Core.h"
#include "IModuleSystem.h"

namespace JTN
{
	namespace Module
	{
		//===============================
		// モジュールのインターフェイス
		// 全モジュールクラスの基底
		//===============================
		class CORE_API IModule
		{
		public:
			IModule() = default;
			virtual ~IModule() = default;

			virtual bool Entry() = 0;

		protected:
			template<class T>
			void ModuleEntry(ModuleEntry_ _moduleEntryPoint, void(T::* _entryFunction)()) {
				static_assert(std::is_base_of<Module::IModule, T>::value, "モジュール以外を指定しました");
				auto Fn = std::bind(_entryFunction, (T*)GetEngine()->GetModuleSystem()->GetNowEntryModule());
				GetEngine()->GetModuleSystem()->ModuleEntry({ _moduleEntryPoint, Fn });
			}
		};
	};
};