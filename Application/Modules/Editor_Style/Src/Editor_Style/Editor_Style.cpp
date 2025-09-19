#include "Editor_Style/Editor_Style.h"
#include "Editor_Style/EditorWindow/Editor_StyleWindow/Editor_StyleWindow.h"

bool Editor_Style::Entry()
{
	m_pEditor = GETMODULE(IEditor);
	if (m_pEditor)
	{
		m_pEditor->AddEditorWindow("Editor_StyleWindow", std::make_shared<Editor_StyleWindow>());
	}
	return m_pEditor;
}

extern"C" EDITOR_STYLE_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress) {
	*_pModuleAdress = new Editor_Style;
	*_pInterfaceInfoAdress = &typeid(Editor_Style);
}
extern"C" EDITOR_STYLE_API void DeleteModule(JTN::Module::IModule* _pModule) {
	delete _pModule;
}