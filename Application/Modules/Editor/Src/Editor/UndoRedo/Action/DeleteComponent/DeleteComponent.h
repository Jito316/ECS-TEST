#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
	class DeleteComponent : public Base
	{
	public:
		DeleteComponent(std::shared_ptr<IComponent> _cmp);

		void Undo()override;
		void Redo()override;
		void Init()override;

		std::weak_ptr<IComponent> owner;
		std::shared_ptr<IComponent> component;
		int index;

	};
}