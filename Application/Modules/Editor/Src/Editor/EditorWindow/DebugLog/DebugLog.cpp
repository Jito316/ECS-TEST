#include "DebugLog.h"
#include "Editor/Utility/Helper/ImGuiHelper.h"

void DebugLog::Init()
{
	m_filter.elementNum = CountActiveElement(m_filter.element);
}

void DebugLog::UpdateContents()
{
	ImGui::PushID((int)this);

	FilterUpdate();
	ImGui::Separator();
	if (BeginLogs()) {
		Logs();
		EndLogs();
	}

	ImGui::PopID();
}

void DebugLog::FilterUpdate()
{
	auto& list = JTN::GetEngine()->GetLog()->WorkLogs();

	if (ImGui::Button("::"))ImGui::OpenPopup("Option"); ImGui::SameLine();
	if (ImGui::Button("Clear"))list.clear(); ImGui::SameLine();
	ImGui::Checkbox("AutoScroll", &m_autoScroll.active); ImGui::SameLine();
	ImGui::InputText("Filter", &m_filter.name);

	if (ImGui::BeginPopup("Option")) {
		ImGui::SeparatorText("State");
		Utility::ImGuiHelper::CheckBoxAllBit<JTN::Log::State>(m_filter.state);
		ImGui::SeparatorText("Element");
		if (Utility::ImGuiHelper::CheckBoxAllBit<Element>(m_filter.element))m_filter.elementNum = CountActiveElement(m_filter.element);
		ImGui::EndPopup();
	}
}

const bool DebugLog::BeginLogs()const
{
	if (m_filter.element)ImGui::BeginChild("Logs");
	return m_filter.element;
}

void DebugLog::Logs()
{
	auto& list = JTN::GetEngine()->GetLog()->GetLogs();

	if (ImGui::BeginTable("LogsTable", m_filter.elementNum, ImGuiTableFlags_BordersInnerH | (m_filter.element ? ImGuiTableFlags_ScrollX : ImGuiTableFlags_None))) {
		for (auto& it : list) {
			if (m_filter.name.size() && it->GetLog().find(m_filter.name))continue;
			if (!(m_filter.state & it->GetState()))continue;
			if (m_filter.element & Element::Time) {
				ImGui::TableNextColumn();
				ImGui::Text(it->GetTime().data());
			}
			if (m_filter.element & Element::Text) {
				ImGui::TableNextColumn();
				ImGui::PushStyleColor(0,GetStateColor(it->GetState()));
				ImGui::Text(it->GetLog().data());
				ImGui::PopStyleColor();
			}
			if (m_filter.element & Element::File) {
				ImGui::TableNextColumn();
				ImGui::Text(it->GetFile().data());
			}

		}
		ImGui::EndTable();
	}
}

void DebugLog::EndLogs()
{
	auto& list = JTN::GetEngine()->GetLog()->GetLogs();
	if (m_autoScroll.active) {
		if (m_autoScroll.postSize != list.size()) {
			ImGui::SetScrollHereY(1.0f);
			m_autoScroll.postSize = list.size();
		}
	}
	ImGui::EndChild();
}

const ImVec4 DebugLog::GetStateColor(const JTN::Log::State& _state)
{
	ImVec4 color = { 1.f,1.f, 1.f, 1.f };
	switch (_state)
	{
	case JTN::Log::State::Warning:
		color = { 1.f,1.f, 0.f, 1.f };
		break;
	case JTN::Log::State::Error:
		color = { 1.f,0.f, 0.f, 1.f };
		break;
	case JTN::Log::State::Default:
		color = { 1.f,1.f, 1.f, 1.f };
		break;
	}
	return color;
}

int DebugLog::CountActiveElement(UINT& _element)
{
	int num = 0;
	for (auto& it : magic_enum::enum_values<Element>()) {
		if (_element & it)num++;
	}
	return num;
}

