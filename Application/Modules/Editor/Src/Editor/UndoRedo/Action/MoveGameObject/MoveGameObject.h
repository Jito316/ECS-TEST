#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
// GameObjectのリスト内の移動
// 親子関係の変更もできる
class MoveGameObject : public Base
{
public:
	MoveGameObject(
		std::weak_ptr<IGameObject> _parent
		,std::shared_ptr<IGameObject> _object
		,std::shared_ptr<IGameObject> _afterObject = nullptr
	);
	void Init()override;

	void Undo()override;
	void Redo()override;

	static void push_child(std::shared_ptr<IGameObject>, std::list<std::shared_ptr<IGameObject>>&);
	static void erase_objects(std::list<std::shared_ptr<IGameObject>>*,const std::list<std::shared_ptr<IGameObject>>&);
	static std::shared_ptr<IGameObject> pick_endChild(std::shared_ptr<IGameObject>);
private:

	std::list<std::shared_ptr<IGameObject>>* m_pList;
	std::weak_ptr<IGameObject> m_parent;

	std::shared_ptr<IGameObject> m_object;
	std::list<std::shared_ptr<IGameObject>> m_objects;
	std::shared_ptr<IGameObject> m_afterObject;

	int m_objectListindex;
	int m_childListindex;
};
}