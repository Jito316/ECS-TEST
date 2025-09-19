#include "MoveComponent.h"
namespace UndoRedoAction {
	MoveComponent::MoveComponent(int _srcIndex, std::shared_ptr<IComponent> _srcCmp, std::shared_ptr<IComponent> _trtCmp, std::list<std::shared_ptr<IComponent>>* _list)
		: Base(_list)
		, m_list(_list)
		, m_srcIndex(_srcIndex)
		, m_srcCmp(_srcCmp)
		, m_trtCmp(_trtCmp)
	{

	}
	void MoveComponent::Init()
	{
		Redo();
	}
	void MoveComponent::Undo()
	{
		auto nowSrcIt = std::find(m_list->begin(), m_list->end(), m_srcCmp);
		m_list->erase(nowSrcIt);

		auto postSrcIt = std::next(m_list->begin(), m_srcIndex);
		m_list->insert(postSrcIt, m_srcCmp);
	}
	void MoveComponent::Redo()
	{
		auto nowSrcIt = std::find(m_list->begin(), m_list->end(), m_srcCmp);
		m_list->erase(nowSrcIt);

		auto postSrcIt = std::find(m_list->begin(), m_list->end(), m_trtCmp);
		m_list->insert(postSrcIt, m_srcCmp);
	}
}