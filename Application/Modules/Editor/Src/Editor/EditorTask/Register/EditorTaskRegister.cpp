#include"EditorTaskRegister.h"
#include"Editor/EditorTask/EditorTask.h"
#include "Editor/EditorWindow/DebugLog/DebugLog.h"
#include "Editor/UndoRedo/UndoRedo.h"

class Render : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		if (!GETMODULE(IWindow)->GetWndHandle())return false;

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		return true;
	}
};

class InputText : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		std::string* value = (std::string*)values[1];
		std::string postValue = *value;

		bool flg = ImGui::InputText(lable.data(), value, ImGuiInputTextFlags_EnterReturnsTrue);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
		return flg;
	}
};

class OpenPopup : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::OpenPopup((const char*)values);
		return true;
	}
};
class BeginPopup : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		return ImGui::BeginPopup((const char*)values);
	}
};
class EndPopup : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::EndPopup();
		return true;
	}
};
class Separator : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::Separator();
		return true;
	}
};
class SeparatorText : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		ImGui::SeparatorText(lable.data());
		return true;
	}
};


class TreeNode : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::TreeNode(lable.data());
	}
};
class TreePop : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::TreePop();
		return true;
	}
};

class SameLine : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::SameLine();
		return true;
	}
};

class BeginTable : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::BeginTable(lable.data(), *(int*)(values[1]));
	}
};


class TableNextColumn : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		return ImGui::TableNextColumn();
	}
}

; class TableNextRow : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::TableNextRow();
		return true;
	}
};

class EndTable : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::EndTable();
		return true;
	}
};

class IsItemClicked : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		return ImGui::IsItemClicked();
	}
};

class PushID : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::PushID(*values);
		return true;
	}
};

class PopID : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		ImGui::PopID();
		return true;
	}
};

class listObject : public UndoRedoAction::Base
{
public:
	listObject(int* pValue_int, std::weak_ptr<IGameObject>* pObject, int* postValue_int, std::weak_ptr<IGameObject>* postObject)
		:UndoRedoAction::Base(pValue_int)
		, value_int(pValue_int, postValue_int)
		, value_gameObject(pObject, postObject)
	{
	}

	void Undo()override
	{
		value_int.Undo();
		value_gameObject.Undo();
	}
	void Redo()override
	{
		value_int.Redo();
		value_gameObject.Redo();
	}
	UndoRedoAction::ValueEdit<int> value_int;
	UndoRedoAction::ValueEdit<std::weak_ptr<IGameObject>> value_gameObject;
};

#include "Editor/Utility/DragDrop/Object/DragDrop_Object.h"
class CreateObjectIndex : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		int* index = (int*)values[1];
		int postIndex = *index;

		std::weak_ptr<IGameObject>* object = (std::weak_ptr<IGameObject>*)values[2];
		std::weak_ptr<IGameObject> postObject = *object;

		std::string name = (*object).expired() ? "emptyObject" : (*object).lock()->GetName();
		ImGui::InputText(lable.data(), &name, ImGuiInputTextFlags_ReadOnly);
		std::weak_ptr<IGameObject> temp;
		if (Utility::DragDrop::TargetGameObjectData(temp))
		{
			auto& list = GETMODULE(IScript)->GetGameObjectManager()->GetObjectList();
			auto it = std::ranges::find(list, temp.lock());

			(*index) = std::distance(list.begin(), it);
			(*object) = temp;

			GETEDITOR.m_spUndoRedo->Add(std::make_unique<listObject>(index, object, &postIndex, &postObject));
			return true;
		}

		return false;
	}
};

#include"Editor/EditorTask/DragScalar/DragScalarTask.h"
#include"Editor/EditorTask/Combo/ComboTask.h"
#include"Editor/EditorTask/Button/ButtonTask.h"
#include"Editor/EditorTask/Select/SelectTask.h"
#define REGISTEREDIORMAP(x)taskMap->emplace(#x, std::make_shared<x>())
void RegisterEditorTask() {
	auto& taskMap = GETEDITOR.m_taskMap = std::make_shared<JTN::Message::CommandWorker::TaskMap>();
	REGISTEREDIORMAP(Render);

	REGISTEREDIORMAP(InputText);
	// Popup
	REGISTEREDIORMAP(OpenPopup);
	REGISTEREDIORMAP(BeginPopup);
	REGISTEREDIORMAP(EndPopup);

	REGISTEREDIORMAP(Separator);
	REGISTEREDIORMAP(SeparatorText);

	// Tree
	REGISTEREDIORMAP(TreeNode);
	REGISTEREDIORMAP(TreePop);

	REGISTEREDIORMAP(SameLine);

	REGISTEREDIORMAP(BeginTable);
	REGISTEREDIORMAP(TableNextColumn);
	REGISTEREDIORMAP(TableNextRow);
	REGISTEREDIORMAP(EndTable);

	REGISTEREDIORMAP(IsItemClicked);
	REGISTEREDIORMAP(SeparatorText);

	REGISTEREDIORMAP(PushID);
	REGISTEREDIORMAP(PopID);

	REGISTEREDIORMAP(CreateObjectIndex);

	RegisterDragScalar(taskMap);
	RegisterCombo(taskMap);
	RegisterButtonTask(taskMap);
	RegisterSelectTask(taskMap);

	JTN::GetEngine()->GetCommandWorker()->RegisterTask("Editor", taskMap);
}