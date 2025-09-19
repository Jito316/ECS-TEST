#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class Hierarchy : public IEditorWindow
{
public:
    void UpdateContents() override;
    void GameObjectOption(std::weak_ptr<IGameObject> parentObject = std::weak_ptr<IGameObject>());
    void Init() override;

private:
    void EndChild() override;
    void ImGuiGameObject(std::weak_ptr<IGameObject> _object,bool& _isInsert);

    // DragDrop関連
    std::shared_ptr<Utility::ImGuiHelper::DragDrop<std::weak_ptr<IGameObject>>> m_dragDropHandler;

    // コンテキストメニュー状態
    bool m_isOptionMenuOpen = false;
    std::string m_createPath;

    bool m_isHoverTarget = false;
};