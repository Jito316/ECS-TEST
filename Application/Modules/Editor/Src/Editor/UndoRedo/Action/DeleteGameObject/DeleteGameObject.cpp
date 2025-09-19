#include "DeleteGameObject.h"
namespace UndoRedoAction {
	DeleteGameObject::DeleteGameObject(std::list<std::shared_ptr<IGameObject>> _objects)
		: Base(_objects.begin()->get())
	{
		head = *_objects.begin();
		parent = head.lock()->GetParent();
		if (parent.lock())childIndex = std::distance(parent.lock()->GetChilds().begin(), std::find_if(parent.lock()->GetChilds().begin(), parent.lock()->GetChilds().end(), [&](const auto& _it) {return head.lock() == _it.lock(); }));
		for (auto& obj : _objects)
		{
			auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
			auto it = std::find(list.begin(), list.end(), obj);

			objectMap[std::distance(list.begin(), it)] = obj;
		}
	}
	DeleteGameObject::DeleteGameObject(std::shared_ptr<IGameObject> _object)
		: Base(_object.get())
	{
		std::list<std::shared_ptr<IGameObject>> result;
		std::function<void(std::shared_ptr<IGameObject> _object, std::list<std::shared_ptr<IGameObject>>& _result)> Fn
			= [&](std::shared_ptr<IGameObject> _object, std::list<std::shared_ptr<IGameObject>>& _result)
			{
				_result.push_back(_object);
				for (auto& it : _object->GetChilds())
				{
					if (it.expired())continue;
					Fn(it.lock(), _result);
				}
			};
		Fn(_object, result);
		*this = DeleteGameObject::DeleteGameObject(result);
	}
	void DeleteGameObject::Undo()
	{
		if (parent.lock())
		{
			parent.lock()->WorkChilds().insert(std::next(parent.lock()->WorkChilds().begin(), childIndex), head);
			head.lock()->SetParent(parent);
		}
		for (auto& [index, obj] : objectMap)
		{
			auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
			list.insert(std::next(list.begin(), index), obj);
		}
	}
	void DeleteGameObject::Redo()
	{
		head.lock()->SetParent(std::weak_ptr<IGameObject>());
		auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		for (
			auto it = objectMap.rbegin()
			; it != objectMap.rend()
			; ++it)
		{
			auto& [index, obj] = *it;
			list.erase(std::next(list.begin(), index));
		}
	}
	void DeleteGameObject::Init()
	{
		head.lock()->SetParent(std::weak_ptr<IGameObject>());
		auto& list = GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList();
		for (
			auto it = objectMap.rbegin()
			; it != objectMap.rend()
			; ++it)
		{
			auto& [index, obj] = *it;
			list.erase(std::next(list.begin(), index));
		}
	}
}