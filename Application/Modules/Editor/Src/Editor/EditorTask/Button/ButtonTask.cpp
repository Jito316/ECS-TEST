#include "ButtonTask.h"
#include "Editor/EditorTask/EditorTask.h"
#include "Editor/EditorWindow/DebugLog/DebugLog.h"
#include "Editor/UndoRedo/UndoRedo.h"

class Button : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::Button(lable.data());
	}
};
class SmallButton : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		std::string_view lable((const char*)values[0]);
		return ImGui::SmallButton(lable.data());
	}
};
class RadioButton : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		UINT postValue = *(UINT*)values[1];
		bool flg = ImGui::RadioButton(lable.data(), (int*)values[2], *(int*)values[1]);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<UINT>>((UINT*)values[2], &postValue));
		return flg;
	}
};
class Checkbox : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		bool* value = (bool*)values[1];
		bool postValue = *value;

		bool flg = ImGui::Checkbox(lable.data(), value);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<bool>>(value, &postValue));
		return flg;
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
class ImageButton : public JTN::Message::ITask
{
public:
	bool Task(void** values)
	{
		return ImGui::ImageButton(
			*(ImTextureID*)values[0]
			, *(ImVec2*)values[1]
		);
	}
};
class AllCheckBitBox : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::vector<std::string_view> vec = *(std::vector<std::string_view>*)values[0];
		UINT* value = (UINT*)values[1];
		UINT postValue = *value;

		bool change = false; int num = 0;
		for (auto& it : vec)
		{
			UINT bit = 1 << (num++);
			bool bitFlg = (*value) & bit;
			if (ImGui::Checkbox(it.data(), &bitFlg))
			{
				if (bitFlg) (*value) |= bit;
				else (*value) &= (~bit);

				change = true;
			}
		}

		if (change)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<UINT>>(value, &postValue));

		return change;
	}
};

#define REGISTEREDIORMAP(x)taskMap->emplace(#x, std::make_shared<x>())
void RegisterButtonTask(std::shared_ptr<JTN::Message::CommandWorker::TaskMap> taskMap)
{
	// Button
	REGISTEREDIORMAP(Button);
	REGISTEREDIORMAP(SmallButton);
	REGISTEREDIORMAP(RadioButton);
	REGISTEREDIORMAP(Checkbox);
	REGISTEREDIORMAP(Selectable);
	REGISTEREDIORMAP(AllCheckBitBox);
}


