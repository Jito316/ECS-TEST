#include "Engine.h"
#include "Utility/Config/Config.h"

namespace JTN
{
	bool Engine::Init()
	{
		m_fpsController.Init();
		m_configSystem.Load();

		File::Config config; nlohmann::json data;
		if (config.Load("ModuleNames", data))
		{
			for (auto& it : data)
			{
				m_moduleManager.Load(it);
			}
		}
		else
		{
			m_moduleManager.Load("Window");
		}

		return true;
	}
	void Engine::Update()
	{
		m_moduleManager.CallEntyFunction(ModuleEntry_Initialize, ModuleEntry_Update);
		while (!is_End())
		{
			m_fpsController.UpdateStartTime();
			m_moduleManager.Updata();
			m_moduleManager.CallEntyFunction(ModuleEntry_Update, ModuleEntry_Finalize);
			m_fpsController.Update();
		}
	}
	void Engine::Release()
	{
		m_commandWorker.Release();
		m_moduleManager.CallEntyFunction(ModuleEntry_Finalize, ModuleEntry_Max);
		m_moduleManager.Release();

		m_logSystem.ClearLogs();

		m_configSystem.Save();
	}

	bool Engine::is_End()
	{
		return m_bEnd;
	}
	void Engine::End()
	{
		m_bEnd = true;
	}

	const bool Engine::is_Debug() const
	{
		return m_bDebug;
	}
	void Engine::SetDebug(const bool _flg)
	{
		m_bDebug = _flg;
	}

	IFPSController* Engine::GetFPSController()
	{
		return &m_fpsController;
	}
	Log::ISystem* Engine::GetLog()
	{
		return &m_logSystem;
	}
	File::IConfigSystem* Engine::GetConfig()
	{
		return &m_configSystem;
	}
	Message::ICommandWorker* Engine::GetCommandWorker()
	{
		return &m_commandWorker;
	}
	Module::IModuleSystem* Engine::GetModuleSystem()
	{
		return &m_moduleManager;
	}
	Module::IModule* Engine::GetModule(const std::type_info& _info)
	{
		return m_moduleManager.GetModule(_info);
	}
}