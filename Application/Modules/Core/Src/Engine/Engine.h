#pragma once
#include "Interface/IEngine.h"
#include "Engine/FPSController/FPSController.h"
#include "Engine/Command/Command.h"
#include "Engine/Config/ConfigSystem.h"
#include "Engine/Log/Log.h"
#include "Engine/ModuleSystem/ModuleSystem.h"

namespace JTN
{
	//=========================
	// ランタイム管理クラス
	//=========================
	class Engine : public IEngine
	{
	public:
		//======== 御三家 ========
		bool Init();
		void Update();
		void Release();

		bool is_End()override;
		void End()override;

		const bool is_Debug()const override;
		void SetDebug(const bool _flg)override;

		IFPSController* GetFPSController()override;
		Log::ISystem* GetLog()override;
		File::IConfigSystem* GetConfig()override;
		Message::ICommandWorker* GetCommandWorker()override;
		Module::IModuleSystem* GetModuleSystem()override;

	private:
		Module::IModule* GetModule(const std::type_info& _info)override;

		FPSController m_fpsController;
		Log::System m_logSystem;
		File::ConfigSystem m_configSystem;
		Message::CommandWorker m_commandWorker;
		Module::ModuleSystem m_moduleManager;

		bool m_bEnd = false;
		bool m_bDebug = false;
	};
};

extern "C" CORE_API void Execute();