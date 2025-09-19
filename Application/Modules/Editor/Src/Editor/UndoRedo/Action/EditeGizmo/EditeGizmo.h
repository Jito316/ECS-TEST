#pragma once
#include "Editor/UndoRedo/UndoRedo.h"

namespace UndoRedoAction 
{
	// GameObjectのリスト内の移動
	// 親子関係の変更もできる
	class EditeGizmo : public Base
	{
	public:
		EditeGizmo(std::weak_ptr<ITransform> _trans, Math::Matrix _prevValue);

		void Undo()override;
		void Redo()override;

	private:
		std::weak_ptr<ITransform> m_trans;
		Math::Matrix m_now;
		Math::Matrix m_prev;
	};
}