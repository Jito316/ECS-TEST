#include "Script.h"
#include "Script/Manager/ObjectManager/GameObjectManager.h"
#include "Script/Manager/ColliderManager/ColliderManager.h"
#include "Script/Manager/ComponentFactory/ComponentFactory.h"
#include "Script/Manager/ComponentFactory/AllComponentRegister.h"
#include "Script/Command/CommandRegister.h"
#include "Utility/Config/Config.h"

bool Script::Entry()
{
	ModuleEntry(ModuleEntry_InitializeScene, &Script::SetUp);
	ModuleEntry(ModuleEntry_UpdateScene, &Script::Update);
	ModuleEntry(ModuleEntry_FinalizeScene, &Script::Shutdown);
	return true;
}

void Script::SetUp()
{
	m_pGameObjectManager = new GameObjectManager;
	m_pComponentFactory = new ComponentFactory;
	m_pColliderManager = new ColliderManager;

	AllComponentRegister(*m_pComponentFactory);

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
	JTN::File::Config config; nlohmann::json data;
	data["NowScene"] = m_nowScene;
	data["StartScene"] = m_startScene;

	data["SceneData"] = nlohmann::json::object();
	for (auto& map : m_sceneList)
	{
		data["SceneData"][map.first] = map.second;
	}
	config.Save("Scene", data);

	m_pColliderManager->Clear();

	delete m_pColliderManager;
	delete m_pGameObjectManager;
	delete m_pComponentFactory;
}

void Script::Update()
{
	// シーンのロード処理
	if (m_callLoad) {
		m_pColliderManager->Clear();
		m_pGameObjectManager->Release(m_sceneList[m_nowScene], m_bSave);
		m_pGameObjectManager->Load(m_sceneList[m_nextScene]);
		m_nowScene = m_nextScene;
		m_callLoad = false;
	}

	// ステイトの変更処理
	if (m_nextRuntimeState)
	{
		m_runtimeState = static_cast<RuntimeState>(m_nextRuntimeState);
		m_nextRuntimeState = 0;
	}

	// コンポーネントのスタート呼び出し
	m_pGameObjectManager->Start();

	if (isPlay())
	{
		m_pGameObjectManager->PreUpdate();

		m_pColliderManager->Update();
		m_pGameObjectManager->Update();

		m_pGameObjectManager->PostUpdate();
	}
	else 
	{
		for (auto& it : m_pGameObjectManager->GetObjectList()) 
		{
			it->GetTransform().lock()->MatrixUpdate();
		}
	}

	// 描画予約
	m_pGameObjectManager->RenderUpdate();
}

void Script::ImGuiUpdate()
{

}

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

IGameObjectManager* Script::GetGameObjectManager()
{
	return m_pGameObjectManager;
}

IComponentFactory* Script::GetComponentFactroy()
{
	return m_pComponentFactory;
}

ColliderManager* Script::GetColliderManager()
{
	return m_pColliderManager;
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

extern"C" SCRIPT_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress)
{
	*_pModuleAdress = new Script;
	*_pInterfaceInfoAdress = &typeid(IScript);
}
extern"C" SCRIPT_API void DeleteModule(JTN::Module::IModule* _module)
{
	delete _module;
}