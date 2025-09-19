#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
	class AddComponent : public Base
	{
	public:
		AddComponent(std::shared_ptr<IComponent> _cmp);

		void Undo()override;
		void Redo()override;

		std::shared_ptr<IComponent> component;
		int index;
	};
}