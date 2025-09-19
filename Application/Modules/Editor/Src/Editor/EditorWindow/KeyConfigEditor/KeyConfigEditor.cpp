#include "KeyConfigEditor.h"

void KeyConfigEditor::UpdateContents()
{
	std::string nowScene = GETMODULE(IScript)->GetNowSceneName();
	if (ImGui::BeginCombo("NowScene", nowScene.c_str()))
	{
		ShowCombContents();
		ImGui::EndCombo();
	}


	if (ImGui::BeginTable("SceneList", m_tableContents.size(), ImGuiTableFlags_BordersH))
	{
		ShowTableContents();
		ImGui::EndTable();
	}
}

void KeyConfigEditor::Init()
{
	//シーン一覧に並べる要素を関数で分けてfunctionで並べる
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return SceneTag(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return ScenePath(_pair); });
	m_tableContents.push_back([&](std::pair<const std::string, std::string>& _pair) {return Remove(_pair); });
}

void KeyConfigEditor::ShowCombContents()
{
	std::map <std::string, std::string>& list = GETMODULE(IScript)->WorkSceneList();
	for (auto& map : list)
	{
		if (ImGui::Selectable(map.first.c_str()))
		{
			GETMODULE(IScript)->LoadScene(map.first);
		}
	}
}

void KeyConfigEditor::ShowTableContents()
{
	std::map <std::string, std::string>& list = GETMODULE(IScript)->WorkSceneList();
	for (std::pair<const std::string, std::string>& map : list)
	{
		for (auto& content : m_tableContents)
		{
			ImGui::TableNextColumn();
			if (content(map))return;
		}
	}
}

bool KeyConfigEditor::SceneTag(std::pair<const std::string, std::string>& _pair)
{
	std::string first = _pair.first;
	if (ImGui::InputText("", &first))
	{
		std::string path = _pair.second;
		GETMODULE(IScript)->WorkSceneList().erase(_pair.first);
		GETMODULE(IScript)->WorkSceneList().emplace(first, path);
		return true;
	}
	return false;
}
bool KeyConfigEditor::ScenePath(std::pair<const std::string, std::string>& _pair)
{
	ImGui::Text(_pair.second.c_str());
	return false;
}
bool KeyConfigEditor::Remove(std::pair<const std::string, std::string>& _pair)
{
	if (ImGui::SmallButton(("Remove##" + _pair.first).c_str()))
	{
		GETMODULE(IScript)->WorkSceneList().erase(_pair.first);
		return true;
	}
	return false;
}