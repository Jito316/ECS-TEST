#include "AddGameObject.h"

namespace UndoRedoAction {
	AddGameObject::AddGameObject(std::list<std::shared_ptr<IGameObject>> _objects)
		: Base(_objects.begin()->get())
		, objects(_objects)
	{
		head = _objects.front();
		parent = head.lock()->GetParent();

		auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		index = std::distance(list.begin(), std::find(list.begin(), list.end(), _objects.front()));
	}

	AddGameObject::AddGameObject(std::shared_ptr<IGameObject> _object)
		: Base(_object.get())
	{
		objects.push_back(_object);

		head = objects.front();
		parent = head.lock()->GetParent();

		auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		index = std::distance(list.begin(), std::find(list.begin(), list.end(), objects.front()));
	}

	void AddGameObject::Undo()
	{
		head.lock()->SetParent(std::weak_ptr<IGameObject>());
		auto& pList = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		for (auto& object : objects)
		{
			auto it = std::ranges::find(pList, object);
			if (it != pList.end())
			{
				pList.erase(it);
			}
		}
	}

	void AddGameObject::Redo()
	{
		if (!parent.expired())
		{
			parent.lock()->AddChild(head);
		}

		auto rIt = objects.rbegin();
		auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		while (rIt != objects.rend())
		{
			list.insert(std::next(list.begin(), index), *rIt);
			rIt++;
		}
	}
}