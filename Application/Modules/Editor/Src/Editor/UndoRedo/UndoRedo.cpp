#include "UndoRedo.h"

void RedoUndoManager::Update()
{
	if (imguiActiving) {
		imguiActiving = false;
		return;
	}

	if (task) {
		task->get()->Init();

		auto script = GETMODULE(IScript);
		if (script && script->isEdit()) {
			undoStack.push(std::move(*task));
			ADDLOG(u8"AddAcition\n");
		}

		delete task; task = nullptr;
	}

}
void RedoUndoManager::Undo()
{
	if (undoStack.empty()) {
		return;
	}

	// undoStack の一番上の要素をムーブ
	std::unique_ptr<UndoRedoAction::Base> undo = std::move(undoStack.top());
	undoStack.pop();

	undo->Undo();
	redoStack.push(std::move(undo));
}
void RedoUndoManager::Redo()
{
	if (redoStack.empty()) {
		return;
	}

	// redoStack の一番上の要素をムーブ
	std::unique_ptr<UndoRedoAction::Base> redo = std::move(redoStack.top());
	redoStack.pop();

	redo->Redo();
	undoStack.push(std::move(redo));
}
void RedoUndoManager::Add(std::unique_ptr<UndoRedoAction::Base> action)
{
	if (task) {
		return;
	}
	else {
		beforeValuePtr = action->editPtr;
		task = new std::unique_ptr<UndoRedoAction::Base>(std::move(action));
	}

	// redoStack をクリア
	while (!redoStack.empty()) {
		redoStack.pop();
	}
}

void RedoUndoManager::Clear()
{
	if (task) {
		delete task;
		task = nullptr;
	}
	while (!undoStack.empty()) {
		undoStack.pop();
	}
	while (!redoStack.empty()) {
		redoStack.pop();
	}
}
