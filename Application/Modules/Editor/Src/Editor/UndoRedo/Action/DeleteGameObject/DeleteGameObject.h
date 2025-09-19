#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
	class DeleteGameObject : public Base
	{
	public:
		DeleteGameObject(std::list<std::shared_ptr<IGameObject>> _objects);
		DeleteGameObject(std::shared_ptr<IGameObject> _objects);

		void Undo()override;
		void Redo()override;

		void Init()override;

		int childIndex = 0;
		std::weak_ptr<IGameObject> parent;
		std::weak_ptr<IGameObject> head;
		std::map<int, std::shared_ptr<IGameObject>> objectMap;
	};
}