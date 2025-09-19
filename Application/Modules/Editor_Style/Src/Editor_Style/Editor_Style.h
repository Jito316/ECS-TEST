#pragma once
class GameObject;
class Component;
class EditorWindowBase;
class DebugLog;
class RedoUndoManager;

class Editor_Style : public JTN::Module::IModule
{
public:
	bool Entry();

private:
	ImGuiIO* m_pImGuiIO = nullptr;
	ImGuiStyle* m_pImGuiStyle = nullptr;
	IEditor* m_pEditor = nullptr;
	std::shared_ptr<IEditorWindow> m_window = nullptr;
};

extern"C" EDITOR_STYLE_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern"C" EDITOR_STYLE_API void DeleteModule(JTN::Module::IModule* _pModule);