#include "SetUpScene.h"

void SetUpScene::UpdateContents()
{
	SetStartSceneName();

	std::string nowScene = m_pScript->GetNowSceneName();
	if (ImGui::BeginCombo("##NowScene", nowScene.c_str()))
	{
		ShowCombContents();
		ImGui::EndCombo();
	}

	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		Add();
	}

	if (ImGui::BeginTable("SceneList", m_tableContents.size(), ImGuiTableFlags_BordersH))
	{
		ShowTableContents();
		ImGui::EndTable();
	}

}

void SetUpScene::Init()
{
	m_pScript = GETMODULE(IScript);

	//シーン一覧に並べる要素を関数で分けてfunctionで並べる
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return SceneTag(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return ScenePath(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return Remove(_pair); });

}

void SetUpScene::EndChild()
{
	IEditorWindow::EndChild();
	MyDragDrop::TargetScene();
}

void SetUpScene::ShowCombContents()
{
	std::map <std::string, std::string>& list = m_pScript->WorkSceneList();
	for (auto& map : list)
	{
		if (ImGui::Selectable(map.first.c_str()))
		{
			m_pScript->LoadScene(map.first);
		}
	}
}

void SetUpScene::ShowTableContents()
{
	std::map <std::string, std::string>& list = m_pScript->WorkSceneList();
	for (std::pair<const std::string, std::string>& map : list)
	{
		for (auto& content : m_tableContents)
		{
			ImGui::TableNextColumn();
			if (content(map))return;
		}
	}
}

bool SetUpScene::SceneTag(std::pair<const std::string, std::string>& _pair)
{
	std::string first = _pair.first;
	if (ImGui::InputText("", &first))
	{
		std::string path = _pair.second;
		m_pScript->WorkSceneList().erase(_pair.first);
		m_pScript->WorkSceneList().emplace(first, path);
		return true;
	}
	return false;
}
bool SetUpScene::ScenePath(std::pair<const std::string, std::string>& _pair)
{
	ImGui::Text(_pair.second.c_str());
	return false;
}
bool SetUpScene::Remove(std::pair<const std::string, std::string>& _pair)
{
	if (ImGui::SmallButton(("Remove##" + _pair.first).c_str()))
	{
		m_pScript->WorkSceneList().erase(_pair.first);
		return true;
	}
	return false;
}

void SetUpScene::SetStartSceneName()
{
	Utility::ImGuiHelper::Combo<std::map<std::string, std::string>::iterator>(
		"StartScane"
		, m_pScript->WorkSceneList().begin()
		, m_pScript->WorkSceneList().end()
		, [](std::map<std::string, std::string>::iterator it) {return it->first; }
	, m_pScript->WorkStartName());
}

void SetUpScene::Add()
{
	std::string path;
	if (!GETMODULE(IWindow)->SaveFileDialog(path, "ファイルの追加", "Sceneファイル\0*.scene\0"))return;

	std::string nowSceneName = m_pScript->GetNowSceneName();
	auto& SceneList = m_pScript->WorkSceneList();

	nowSceneName = std::filesystem::path(path).filename().string();
	Utility::StringHelper::DeleteWord(nowSceneName, std::filesystem::path(path).filename().extension().string());
	SceneList[nowSceneName] = path;

	Utility::JsonHelper::OutputJson(nlohmann::json(), path);

	m_pScript->LoadScene(nowSceneName);
}

void SetUpScene::Open()
{
	std::string path;
	if (!GETMODULE(IWindow)->OpenFileDialog(path, "ファイルの読み込み", "Sceneファイル\0*.scene\0"))return;

	std::string nowSceneName = std::filesystem::path(path).filename().string();
	Utility::StringHelper::DeleteWord(nowSceneName, std::filesystem::path(path).filename().extension().string());
	auto& SceneList = m_pScript->WorkSceneList();
	SceneList[nowSceneName] = path;

	m_pScript->LoadScene(nowSceneName);
}

void SetUpScene::Save()
{
	m_pScript->LoadScene(m_pScript->GetNowSceneName(), true);
}

void SetUpScene::SaveAs()
{
	std::string path = m_pScript->GetSceneList().at(m_pScript->GetNowSceneName());
	if (!GETMODULE(IWindow)->SaveFileDialog(path, "ファイルの保存", "Sceneファイル\0*.scene\0"))return;
	m_pScript->WorkNowSceneName() = path;

	std::string newSceneName = std::filesystem::path(path).filename().string();
	Utility::StringHelper::DeleteWord(newSceneName, std::filesystem::path(path).filename().extension().string());
	m_pScript->WorkSceneList()[newSceneName] = path;

	m_pScript->LoadScene(newSceneName, true);
}

namespace MyDragDrop
{
	bool SourceScene(std::string _path)
	{
		return Utility::ImGuiHelper::DragDropSource("SceneDragDrop", _path);
	}
	void TargetScene()
	{
		if (std::string _path; Utility::ImGuiHelper::DragDropTarget("SceneDragDrop", _path))
		{
			std::filesystem::path path(_path);
			std::map <std::string, std::string>& list = GETMODULE(IScript)->WorkSceneList();

			std::string str = path.filename().string();
			Utility::StringHelper::DeleteWord(str, path.filename().extension().string());
			list[str] = path.string();
		}
	}
}