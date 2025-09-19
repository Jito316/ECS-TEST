#include "EditeGizmo.h"
namespace UndoRedoAction {
	EditeGizmo::EditeGizmo(std::weak_ptr<ITransform> _trans, Math::Matrix _prevValue)
		: Base(_trans.lock().get())
		,m_trans(_trans)
		, m_now(_trans.lock()->GetWorldMatrix())
		, m_prev(_prevValue)
	{
	}
	void EditeGizmo::Undo()
	{
		m_trans.lock()->SetWorldMatrix(m_prev);
	}
	void EditeGizmo::Redo()
	{
		m_trans.lock()->SetWorldMatrix(m_now);
	}
}