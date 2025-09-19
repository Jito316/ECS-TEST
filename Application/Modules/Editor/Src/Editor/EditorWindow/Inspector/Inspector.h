#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class Inspector
	:public IEditorWindow
{
public:
	void UpdateContents() override;

	void ImGuiGameObject(std::weak_ptr<IGameObject> _object);
private:
	void ShowGameObject(std::weak_ptr<IGameObject> _object);
	void ShowComponent(std::weak_ptr<IGameObject> _object);
	void AddComponent(std::weak_ptr<IGameObject> _object);

	bool DragComponentSource(std::list<std::shared_ptr<IComponent>>::iterator _cmp);
	bool DragComponentTraget(std::list<std::shared_ptr<IComponent>>::iterator _cmp);

	bool ComponentTreeNode(std::list<std::shared_ptr<IComponent>>::iterator _cmp);

	struct {
		std::shared_ptr<IComponent> cmp;
		std::list<std::shared_ptr<IComponent>>::iterator pos;
	}m_drag;
};	
