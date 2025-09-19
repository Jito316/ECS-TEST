#pragma once
class GameObjectManager;
class ComponentFactory;
class ColliderManager;

class Script :public IScript
{
public:
	bool Entry()override;
	void SetUp();
	void Update();
	void Shutdown();

	void ImGuiUpdate()override;

	void LoadScene(const std::string& _name, const bool _bSave = false)override;

	const std::string& GetNowSceneName() const override { return m_nowScene; }
	std::string& WorkNowSceneName() override { return m_nowScene; }

	const std::map<std::string, std::string>& GetSceneList() override { return m_sceneList; }
	std::map<std::string, std::string>& WorkSceneList() override { return m_sceneList; }

	const std::string& GetStartName() const override { return m_startScene; }
	std::string& WorkStartName() override { return m_startScene; }

	IGameObjectManager* GetGameObjectManager() override;
	IComponentFactory* GetComponentFactroy() override;
	ColliderManager* GetColliderManager();

	bool SetState(const RuntimeState _state)override;
	bool isPlay()override;
	bool isStop()override;
	bool isEdit()override;

private:
	std::map <std::string, std::string> m_sceneList;
	std::string m_startScene;
	std::string	m_nowScene;
	std::string	m_nextScene;

	bool m_callLoad = false;
	bool m_bSave = false;

	GameObjectManager* m_pGameObjectManager = nullptr;
	ComponentFactory* m_pComponentFactory = nullptr;
	ColliderManager* m_pColliderManager = nullptr;

	std::shared_ptr<JTN::Message::CommandWorker::TaskMap> m_taskMap = nullptr;

	RuntimeState m_runtimeState = RuntimeState::Edit;
	int m_nextRuntimeState = 0;
};

extern"C" SCRIPT_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern"C" SCRIPT_API void DeleteModule(JTN::Module::IModule* _module);

#define GETSCRIPT (*static_cast<Script*>(JTN::GetEngine()->GetModule<IScript>()))
#define GETGAMEOBJECTMANAGER (*static_cast<GameObjectManager*>(GETSCRIPT.GetGameObjectManager()))
#define EDITOR_COMMAND(...)JTN::GetEngine()->GetCommandWorker()->Request("Editor",__VA_ARGS__)
