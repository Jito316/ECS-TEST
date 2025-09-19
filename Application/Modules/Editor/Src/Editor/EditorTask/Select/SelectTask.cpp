#include "SelectTask.h"
#include "Editor/EditorTask/EditorTask.h"
#include "Editor/EditorWindow/DebugLog/DebugLog.h"
#include "Editor/UndoRedo/UndoRedo.h"
#include "Editor/Utility/DragDrop/DragDrop.h"
#include "Editor/Utility/DragDrop/Object/DragDrop_Object.h"

// str1 : path
// str2 : title
// str3 : filter
class SelectFile : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string* value = (std::string*)values[0];
		std::string postValue = *value;

		std::string* title = (std::string*)values[1];
		std::string* filter = (std::string*)values[2];

		if (ImGui::Button((value->empty()) ? "EmptyPath" : value->c_str()))
		{
			if (GETMODULE(IWindow)->OpenFileDialog(
				*value
				, (title ? *title : "")
				, (filter ? *filter : "").c_str()))
			{
				GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
				return true;
			}
		}
		return false;
	}
};

class SelectTexturePath : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string title("画像はファイル");
		std::string filter("画像ファイル\0*.png\0");
		std::vector<void*>vec = {
			values[0]
			, &title
			, &filter
		};

		if (m_selector.Task(&vec[0])) {
			return true;
		}

		std::string* value = (std::string*)values[0];
		std::string postValue = *value;

		if (DragDrop::TargetTexture(*value)) {
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
			return true;
		}
		return false;
	}
private:
	SelectFile m_selector;
};

class SelectModelPath : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string title("モデルファイルを開く");
		std::string filter("gltfファイル\0*.gltf\0");
		std::vector<void*>vec = {
			values[0]
			, &title
			, &filter
		};

		if (m_selector.Task(&vec[0])) {
			return true;
		}

		std::string* value = (std::string*)values[0];
		std::string postValue = *value;

		if (DragDrop::TargetModel(*value)) {
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
			return true;
		}

		return false;
	}
private:
	SelectFile m_selector;
};

class SelectWavPath : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string title("wavを開く");
		std::string filter("wavファイル\0*.wav\0");
		std::vector<void*>vec = {
			values[0]
			, &title
			, &filter
		};

		if (m_selector.Task(&vec[0])) {
			return true;
		}

		std::string* value = (std::string*)values[0];
		std::string postValue = *value;

		if (DragDrop::TargetWav(*value)) {
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
			return true;
		}

		return false;
	}
private:
	SelectFile m_selector;
};

class SelectPrefabPath : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string title("prefabを開く");
		std::string filter("prefabファイル\0*.prefab\0");
		std::vector<void*>vec = {
			values[0]
			, &title
			, &filter
		};

		if (m_selector.Task(&vec[0])) {
			return true;
		}

		std::string* value = (std::string*)values[0];
		std::string postValue = *value;

		if (Utility::DragDrop::TargetGameObjectDataPath(*value)) {
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<std::string>>(value, &postValue));
			return true;
		}

		return false;
	}
private:
	SelectFile m_selector;
};


#define REGISTEREDIORMAP(x)taskMap->emplace(#x, std::make_shared<x>())
void RegisterSelectTask(std::shared_ptr<JTN::Message::CommandWorker::TaskMap> taskMap)
{
	// Button
	REGISTEREDIORMAP(SelectFile);
	REGISTEREDIORMAP(SelectTexturePath);
	REGISTEREDIORMAP(SelectModelPath);
	REGISTEREDIORMAP(SelectWavPath);
	REGISTEREDIORMAP(SelectPrefabPath);
}


