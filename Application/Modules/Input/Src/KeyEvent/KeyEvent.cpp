#include "KeyEvent.h"

const bool KeyEvent::IsFree() const
{
	return Kernel::GetEngine()->m_kdInputManager.IsFree(m_buttonID);
}
const bool KeyEvent::IsPress() const
{
	return  Kernel::GetEngine()->m_kdInputManager.IsPress(m_buttonID);
}
const bool KeyEvent::IsHold() const
{
	return  Kernel::GetEngine()->m_kdInputManager.IsHold(m_buttonID);
}
const bool KeyEvent::IsRelease() const
{
	return  Kernel::GetEngine()->m_kdInputManager.IsRelease(m_buttonID);
}

void KeyEvent::Serialize(nlohmann::json& _json)
{
	if (m_name.empty())return;
	_json[m_name.data()] = m_buttonID;
}
void KeyEvent::Deserialize(const nlohmann::json& _json)
{
	if (m_name.empty())return;
	m_buttonID = _json[m_name.data()].get<std::string>();
}

void KeyEvent::ImGuiUpdate()
{
	ImGui::PushID(std::to_string((int)this).c_str());
	if (ImGui::BeginCombo(m_name.data(), m_buttonID.c_str())) {
		for (auto& [key, value] :  Kernel::GetEngine()->m_kdInputManager.GetDevice(KEYSTAG)->GetButtons())
		{
			if (ImGui::Selectable(key.c_str()))
			{
				m_buttonID = key;
				break;
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();
}


std::shared_ptr<const KeyEvent> KeyEvents::operator[](std::string_view key)
{
	return m_keyMap.emplace(key.data(), std::make_shared<KeyEvent>(key)).first->second;
}
void KeyEvents::Serialize(nlohmann::json& _json)
{
	nlohmann::json keysData;
	for (auto& [key, value] : m_keyMap)
	{
		if (value == nullptr)continue;
		value->Serialize(keysData);
	}
	_json["Keys"] = keysData;
}
void KeyEvents::Deserialize(const nlohmann::json& _json)
{
	auto it = _json["Keys"].begin();
	while (it != _json["Keys"].end())
	{
		m_keyMap.emplace(it.key(), std::make_shared<KeyEvent>(it.key()))
			.first->second->Deserialize(_json["Keys"]);
		it++;
	}
}

void KeyEvents::ImGuiUpdate()
{
	if (EDITOR_COMMAND("TreeNode", ("Keys##" + std::to_string((int)this)).c_str()))
	{
		for (auto& [key, value] : m_keyMap)
		{
			if (value == nullptr)continue;
			value->ImGuiUpdate();
		}
		EDITOR_COMMAND("TreePop","" );
	}
}
