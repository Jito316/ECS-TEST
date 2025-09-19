#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"
class FreeCamera;

class GameScreen
	:public IEditorWindow
{
	std::shared_ptr<FreeCamera> m_buildCamera = nullptr;
	std::shared_ptr<ITexture> m_spTexture = nullptr;

	enum MouseLButtonState { Gizmo, DragMove };
	MouseLButtonState m_mouseLButtonState = MouseLButtonState::Gizmo;

	ImVec2 m_imageSize;
	ImGuizmo::OPERATION m_zmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE m_zmoMode = ImGuizmo::MODE::LOCAL;

	int m_listSize = 0;
	bool m_itemActive = false;

	std::shared_ptr<Math::Matrix> m_pPrevTrans = nullptr;

	void UpdateContents()override;
	void BeginChild()override;

	void UpdateCamera();
	void UpdateGizmo();
	void PickObject(ImRect itemSize, ImVec2 mousePos);
};
