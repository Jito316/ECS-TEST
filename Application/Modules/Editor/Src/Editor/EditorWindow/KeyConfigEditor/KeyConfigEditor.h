#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class KeyConfigEditor
	:public IEditorWindow
{
public:
	void UpdateContents()override;
	void Init()override;
private:
	void ShowCombContents();
	void ShowTableContents();

	bool SceneTag(std::pair<const std::string, std::string>& _pair);
	bool ScenePath(std::pair<const std::string, std::string>& _pair);
	bool Remove(std::pair<const std::string, std::string>& _pair);

	std::list<std::function<bool(std::pair<const std::string, std::string>&)>>m_tableContents;
};