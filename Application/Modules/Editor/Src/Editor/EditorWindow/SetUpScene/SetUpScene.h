#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class SetUpScene
	:public IEditorWindow
{
public:
	void UpdateContents()override;
	void Init()override;
private:

	void EndChild()override;

	void ShowCombContents();
	void ShowTableContents();

	bool SceneTag(std::pair<const std::string, std::string>& _pair);
	bool ScenePath(std::pair<const std::string, std::string>& _pair);
	bool Remove(std::pair<const std::string, std::string>& _pair);


	IScript* m_pScript = nullptr;
	std::list<std::function<bool(std::pair<const std::string, std::string>&)>>m_tableContents;

public:
	void SetStartSceneName();
	void Add();
	void Open();
	void Save();
	void SaveAs();
};

namespace MyDragDrop
{
	bool SourceScene(std::string _path);
	void TargetScene();
}

