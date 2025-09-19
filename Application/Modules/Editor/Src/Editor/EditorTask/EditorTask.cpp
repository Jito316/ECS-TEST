#include "EditorTask.h"
#include "Editor/UndoRedo/UndoRedo.h"

bool BaseEditorTask::Task(void** values)
{
	bool flg = TaskContents(values);
	if (ImGui::IsItemActive()) {
		GETEDITOR.m_spUndoRedo->Activing();
	}
	return flg;
}