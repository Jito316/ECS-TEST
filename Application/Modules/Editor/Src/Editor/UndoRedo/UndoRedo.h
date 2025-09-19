#pragma once
namespace UndoRedoAction
{
	class Base;
}

class RedoUndoManager {
public:
	void Update();
	void Undo();

	void Redo();
	void Add(std::unique_ptr<UndoRedoAction::Base> action);
	void Activing() { imguiActiving = true; }

	void Clear();
private:
	bool imguiActiving = false;
	void* beforeValuePtr = nullptr;

	std::unique_ptr<UndoRedoAction::Base>* task = nullptr;

	std::stack<std::unique_ptr<UndoRedoAction::Base>> undoStack;
	std::stack<std::unique_ptr<UndoRedoAction::Base>> redoStack;
};

namespace UndoRedoAction
{
	class Base
	{
	public:
		Base(void* _editPtr) :editPtr(_editPtr) {}
		virtual ~Base() = default;

		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual void Init() {};

		void* editPtr = nullptr;
	};
	template<class T, size_t Array = 1>class  ValueEdit : public Base
	{
	public:
		ValueEdit(T* _now, T* _prev)
			:now(_now)
			, Base(_now)
		{
			for (size_t i = 0; i < Array; ++i) {
				prev[i] = _prev[i];
			}
		}
		virtual ~ValueEdit()override = default;


		virtual void Undo()override
		{
			for (size_t i = 0; i < Array; ++i) {
				std::swap(now[i], prev[i]);
			}
		}
		virtual void Redo()override
		{
			for (size_t i = 0; i < Array; ++i) {
				std::swap(now[i], prev[i]);
			}
		}

		T* now = nullptr;
		std::array<T, Array> prev;
	};
}
