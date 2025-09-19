#pragma once
class GameObject;
class Component;
class EditorWindowBase;
class DebugLog;
class RedoUndoManager;
class IWindow_Event;

class Editor : public IEditor
{
public:
	void SetActive(bool _flg) override { m_isActive = _flg; }
	bool GetActive() const override { return m_isActive; }

	void SetEditObject(std::weak_ptr<IGameObject> _obj) override { m_editObject = _obj; }
	std::weak_ptr<IGameObject> GetEditObject() override { return m_editObject; }
	std::weak_ptr<DebugLog> GetDebugLog() { return m_debugLog; }

	ImGuiIO& GetIO() override { return ImGui::GetIO(); }
	ImGuiStyle& GetStyle() override { return ImGui::GetStyle(); }
	virtual ImGuiContext& GetContext() { return *ImGui::GetCurrentContext(); }

	void AddEditorWindow(std::string_view _name, std::shared_ptr<IEditorWindow> _editorWindow);

	std::shared_ptr<JTN::Message::CommandWorker::TaskMap> m_taskMap;
	std::shared_ptr<RedoUndoManager> m_spUndoRedo;
private:
	bool Entry()override;
	void SetUp();
	void Update();
	void Shutdown();
	void Render();

	bool BeginEditor();
	void UpdateEditor();
	void EndEditor();

	void MenuBar();
	void PlayButton();

	void StartButton();
	void StopButton();

	std::shared_ptr<IEditorWindow> GetEditorWindow(const std::type_info& _info)override;

	std::weak_ptr<IGameObject>														m_editObject;
	std::map<std::string, std::shared_ptr<IEditorWindow>>							m_editorWindows;

	std::shared_ptr<DebugLog>														m_debugLog;

	bool m_isActive = true;
	bool m_initialized = false;

	IWindow*    m_pWindow = nullptr;
	IWindow_Event* m_pWindow_Event = nullptr;
	IRenderer*  m_pRenderer = nullptr;
	IInput*     m_pInput = nullptr;
	IScript*    m_pScript = nullptr;
};

extern"C" EDITOR_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern"C" EDITOR_API void DeleteModule(JTN::Module::IModule* _pModule);

#define GETEDITOR (*static_cast<Editor*>(JTN::GetEngine()->GetModule<IEditor>()))
#define SCRIPT_COMMAND(...) JTN::GetEngine()->GetCommandWorker()->Request("Script",__VA_ARGS__)
