#include "IEditorWindow.h"
#include "Utility/Config/Config.h"

struct IEditorWindow::IEditorWindowImpl
{
public:
	IEditor* owner = nullptr;
	std::string name = "EditorWindow";
	bool bActive = true;
};

IEditorWindow::IEditorWindow()
{
	m_Impl = new IEditorWindowImpl;
}

IEditorWindow::~IEditorWindow()
{
	delete m_Impl;
}

void IEditorWindow::Update()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	ImGui::Begin(m_Impl->name.c_str());
	ImGui::PopStyleVar();
	{
		BeginChild();
		{
			UpdateContents();
		}
		EndChild();
	}
	ImGui::End();
}

void IEditorWindow::SetOwner(IEditor* _editor)
{
	m_Impl->owner = _editor;
}

void IEditorWindow::SetName(std::string _str)
{
	m_Impl->name = _str;
}

bool IEditorWindow::GetActive() const
{
	return m_Impl->bActive;
}

void IEditorWindow::SetActive(const bool _active)
{
	m_Impl->bActive = _active;
}

IEditor* IEditorWindow::GetOwner()
{
	return m_Impl->owner;
}

void IEditorWindow::ConfigLoad()
{
	JTN::File::Config config;nlohmann::json json;
	if (config.Load(m_Impl->name, json))
	{
		ConfigLoadContents(json);
	}
}
void IEditorWindow::ConfigSave()
{
	nlohmann::json json;
	ConfigSaveContents(json);

	JTN::File::Config().Save(m_Impl->name, json);
}

void IEditorWindow::BeginChild()
{
	ImGui::BeginChild(std::to_string((int)this).c_str(), ImGui::GetContentRegionAvail(), ImGuiChildFlags_Border);
}

void IEditorWindow::EndChild()
{
	ImGui::EndChild();
}
