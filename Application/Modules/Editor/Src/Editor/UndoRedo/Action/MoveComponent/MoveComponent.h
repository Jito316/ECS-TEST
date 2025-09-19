#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction{
class MoveComponent : public Base
{
public:
	MoveComponent(int _srcIndex, std::shared_ptr<IComponent> _srcCmp, std::shared_ptr<IComponent> _trtCmp, std::list<std::shared_ptr<IComponent>>* _list);

	void Init();

	void Undo()override;
	void Redo()override;

	int m_srcIndex = 0;
	std::shared_ptr<IComponent> m_srcCmp;
	std::shared_ptr<IComponent> m_trtCmp;

	std::list<std::shared_ptr<IComponent>>* m_list;
};
}