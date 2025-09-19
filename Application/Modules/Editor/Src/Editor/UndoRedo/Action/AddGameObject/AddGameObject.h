#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
	class AddGameObject : public Base
	{
	public:
		AddGameObject(std::list<std::shared_ptr<IGameObject>> _objects);
		AddGameObject(std::shared_ptr<IGameObject> _object);

		void Undo()override;
		void Redo()override;

		std::weak_ptr<IGameObject> parent;
		std::weak_ptr<IGameObject> head;
		std::list<std::shared_ptr<IGameObject>> objects;
		int index = 0;
	};
}