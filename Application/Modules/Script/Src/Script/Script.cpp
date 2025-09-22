#include "Script.h"
#include "Script/Command/CommandRegister.h"
#include "Utility/Config/Config.h"

void Script::LoadScene(const std::string& _name, const bool _bSave)
{
	std::map<std::string, std::string>::iterator it = m_sceneList.find(_name);
	if (it == m_sceneList.end())
	{
		ADDLOG("No Found Scene!!");
		return;
	}
	if (!std::filesystem::exists((*it).second))
	{
		ADDLOG("Scene FilePath Broken!!");
		return;
	}
	m_callLoad = true;
	m_nextScene = _name;
	m_bSave = _bSave;
}

bool Script::SetState(const RuntimeState _state)
{
	if (m_nextRuntimeState == _state)
	{
		return false;
	}

	m_nextRuntimeState = _state;

	return true;
}

bool Script::isPlay()
{
	return m_runtimeState == RuntimeState::Play;
}

bool Script::isStop()
{
	return m_runtimeState == RuntimeState::Stop;
}

bool Script::isEdit()
{
	return m_runtimeState == RuntimeState::Edit;
}

void Script::ImGuiUpdate()
{

}


bool Script::Entry()
{
	ModuleEntry(ModuleEntry_InitializeScene, &Script::SetUp);
	ModuleEntry(ModuleEntry_UpdateScene, &Script::Update);
	ModuleEntry(ModuleEntry_FinalizeScene, &Script::Shutdown);
	return true;
}

void Script::SetUp()
{
	ECSManager::SetUp();
	JTN::File::Config config; nlohmann::json data;
	if (config.Load("Scene", data))
	{
		m_nowScene = data["NowScene"];
		m_startScene = data["StartScene"];

		nlohmann::json::iterator map = data["SceneData"].begin();
		while (map != data["SceneData"].end())
		{
			m_sceneList.emplace(map.key(), *map);
			map++;
		}
	}
	else
	{
		m_nowScene = "TitleA";
		m_sceneList["TitleC"] = "path.scene";
		m_sceneList["TitleA"] = "path.scene";
		m_sceneList["TitleB"] = "path.scene";

		m_startScene = "Test";
	}

	Command::Register(m_taskMap);

	LoadScene(m_startScene);
}

void Script::Shutdown()
{
	ECSManager::Shutdown();
	JTN::File::Config config; nlohmann::json data;
	data["NowScene"] = m_nowScene;
	data["StartScene"] = m_startScene;

	data["SceneData"] = nlohmann::json::object();
	for (auto& map : m_sceneList)
	{
		data["SceneData"][map.first] = map.second;
	}
	config.Save("Scene", data);
}

void Script::Update()
{
	// シーンのロード処理
	if (m_callLoad) {
		ECSManager::Load(m_sceneList[m_nowScene]);
		m_nowScene = m_nextScene;
		m_callLoad = false;
	}

	// ステイトの変更処理
	if (m_nextRuntimeState)
	{
		m_runtimeState = static_cast<RuntimeState>(m_nextRuntimeState);
		m_nextRuntimeState = 0;
	}


	if (isPlay())
	{
		ECSManager::Update();
	}
}

extern"C" SCRIPT_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress)
{
	*_pModuleAdress = new Script;
	*_pInterfaceInfoAdress = &typeid(IScript);
}
extern"C" SCRIPT_API void DeleteModule(JTN::Module::IModule* _module)
{
	delete _module;
}