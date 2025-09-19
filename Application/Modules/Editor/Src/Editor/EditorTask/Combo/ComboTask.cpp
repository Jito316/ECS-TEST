#include "ComboTask.h"
#include "Editor/EditorTask/EditorTask.h"
#include "Editor/EditorWindow/DebugLog/DebugLog.h"
#include "Editor/UndoRedo/UndoRedo.h"


template<class T>
class BitEditor : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		UINT postValue = *(UINT*)(values[1]);
		bool flg = Utility::ImGuiHelper::BitEditor<T, UINT>(*(UINT*)(values[1]), lable.data());
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<UINT>>((UINT*)(values[1]), &postValue));

		return flg;
	}
};

class Combo : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string changeValue;
		bool is_open = false;

		std::string_view lable((const char*)values[0]);
		std::vector<std::string>& vec = *(std::vector<std::string>*)(values[1]);
		std::string postValue = *(std::string*)values[2];

		if (ImGui::BeginCombo(lable.data(), ((std::string*)values[2])->data())) {

			for (auto& it : vec) {
				if (ImGui::Selectable(it.data())) {
					changeValue = it.data();
				}
			}
			ImGui::EndCombo();
		};

		if (changeValue.size()) {
			auto& value = *(std::string*)values[2];
			value = changeValue;
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>((std::string*)values[2], &postValue));
		}

		return changeValue.size();
	}
};

class BeginCombo : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::BeginCombo(
			lable.data()
			, (values[1]) ? ((std::string*)(values[1]))->data() : nullptr
		);
	}
};
class Selectable : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::Selectable(lable.data());
	}
};
class EndCombo : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::EndCombo();
		return true;
	}
};

template<class T>
class Combo_Enum : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		bool flg = false;

		std::string_view lable((const char*)values[0]);
		T& value = *(T*)(values[1]);
		T postValue = value;

		if (ImGui::BeginCombo(lable.data(), magic_enum::enum_name(value).data())) {
			for (auto& it : magic_enum::enum_values<T>()) {
				if(ImGui::Selectable(magic_enum::enum_name(it).data())) {
					value = it;
					flg = true;
				}
			}
			ImGui::EndCombo();
		}
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<T>>(&value, &postValue));

		return flg;
	}
};

class Combo_ObjectChildNmae : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		std::string& value = *(std::string*)values[1];
		std::string postValue = value;
		std::weak_ptr<IGameObject> object = *(std::weak_ptr<IGameObject>*)values[2];

		bool edited = false;

		if (ImGui::BeginCombo(lable.data(), value.data())) {
			for (auto& it : object.lock()->GetChilds()) {
				if (ImGui::Selectable(it.lock()->GetName().data())) {
					value = it.lock()->GetName();
					edited;
					break;
				}
			}
			ImGui::EndCombo();
		}

		if (edited)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(&value, &postValue));
		return edited;
	}
};

#define REGISTEREDIORMAP(x)taskMap->emplace(#x, std::make_shared<x>())
void RegisterCombo(std::shared_ptr<JTN::Message::CommandWorker::TaskMap> taskMap)
{
	taskMap->emplace("BitEditor_ColliderType", std::make_shared<BitEditor<Math::KdCollider::Type>>());
	taskMap->emplace("BitEditor_DrawType", std::make_shared<BitEditor<IRenderer::DrawType>>());
	taskMap->emplace("BitEditor_Shaft", std::make_shared<BitEditor<ITransform::Shaft>>());
	taskMap->emplace("Combo_ObjectTag", std::make_shared<Combo_Enum<IGameObject::ObjectTag>>());
	REGISTEREDIORMAP(Combo);
	REGISTEREDIORMAP(BeginCombo);
	REGISTEREDIORMAP(Selectable);
	REGISTEREDIORMAP(EndCombo);
	REGISTEREDIORMAP(Combo_ObjectChildNmae);
}


