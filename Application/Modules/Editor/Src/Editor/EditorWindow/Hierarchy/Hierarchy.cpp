#include "Hierarchy.h"
#include "Editor/EditorWindow/ProjectWindow/ProjectWindow.h"

#include "Editor/Utility/DragDrop/Object/DragDrop_Object.h"
#include "Editor/UndoRedo/Action/AddGameObject/AddGameObject.h"
#include "Editor/UndoRedo/Action/DeleteGameObject/DeleteGameObject.h"

void Hierarchy::UpdateContents()
{
	m_isOptionMenuOpen = false;
	m_isHoverTarget = false;

	auto pScript = GETMODULE(IScript);
	bool beforeSource = true;
	for (auto& it : pScript->GetGameObjectManager()->GetObjectList())
	{
		if (it->GetParent().expired())
		{
			ImGuiGameObject(it, beforeSource);
		}
	}

	/*
	if (ImGui::Begin("A"))
	{
		for (auto& it : pScript->GetGameObjectManager()->GetObjectList()) 
		{
			ImGui::Text(it->GetName().data());
		}
	}
	ImGui::End();
	*/

	if (!m_isHoverTarget)m_isHoverTarget = m_isHoverTarget || Utility::DragDrop::TargetGameObjectData_Insert({}, {});
}

void Hierarchy::GameObjectOption(std::weak_ptr<IGameObject> _parent)
{
	if (m_isOptionMenuOpen)return;
	auto& editor = GETEDITOR;
	std::string ID = "GameObjectOption" + std::to_string((int)_parent.lock().get());
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))ImGui::OpenPopup(ID.c_str());
	if (ImGui::BeginPopup(ID.c_str()))
	{
		m_isOptionMenuOpen = true;
		if (_parent.lock())
		{
			ImGui::InputText("##Path", &m_createPath);
			if (ImGui::SameLine(); ImGui::Button("Save"))
			{
				nlohmann::json json = nlohmann::json::array();
				_parent.lock()->SerializeFamily(json[0]);
				Utility::JsonHelper::OutputJson(json, m_createPath);
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Remove"))
			{
				editor.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::DeleteGameObject>(_parent.lock()));
				editor.SetEditObject({});
			}
		}
		if (ImGui::Button("Create"))
		{
			std::list<std::shared_ptr<IGameObject>> results;
			if (SCRIPT_COMMAND("CreateObject", nullptr, &_parent, &results)) {
				editor.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::AddGameObject>(results));
			}
		}
		ImGui::EndPopup();
	}
}

void Hierarchy::ImGuiGameObject(std::weak_ptr<IGameObject> _object, bool& _isInsert)
{
	if (_object.expired()) return;

	if (!m_isHoverTarget && _isInsert)m_isHoverTarget = m_isHoverTarget || Utility::DragDrop::TargetGameObjectData_Insert(_object.lock()->GetParent().lock(), _object.lock());
	auto& childObjects = _object.lock()->GetChilds();

	// ツリーノードのフラグ設定
	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
	if (childObjects.empty()) treeNodeFlags |= ImGuiTreeNodeFlags_Bullet;
	if (_object.lock() == GETEDITOR.GetEditObject().lock()) treeNodeFlags |= ImGuiTreeNodeFlags_Selected;


	// オブジェクト名とIDを組み合わせたラベル作成
	std::string nodeLabel = _object.lock()->GetName() + "##" + std::to_string((int)_object.lock().get());
	// ツリーノードの描画と操作
	bool isNodeOpen = isNodeOpen = ImGui::TreeNodeEx(nodeLabel.c_str(), treeNodeFlags);

	// 選択処理
	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
		GETEDITOR.SetEditObject(_object);
	}

	// コンテキストメニューの表示
	GameObjectOption(_object);

	// ドラッグ&ドロップ処理
	if (!(m_dragDropHandler->CallSource(_object) && m_isHoverTarget)) {
		m_isHoverTarget = m_isHoverTarget || m_dragDropHandler->CallTarget(_object);
	}
	_isInsert = !(Utility::DragDrop::GetSourceGameObjectData() == _object.lock());

	// 子オブジェクトの描画
	if (isNodeOpen)
	{
		if (childObjects.size())
		{
			bool beforeSource = true;
			auto childIter = childObjects.begin();
			while (childIter != childObjects.end())
			{
				ImGuiGameObject(*childIter, beforeSource);
				++childIter;
			}
		}

		if (!m_isHoverTarget)
		{
			m_isHoverTarget = m_isHoverTarget || Utility::DragDrop::TargetGameObjectData_Insert(_object.lock(), {});
		}
		ImGui::TreePop();
	}
}

void Hierarchy::Init()
{
	//いちアイテムに対してのドラックドロップ処理
	m_dragDropHandler = std::make_shared<Utility::ImGuiHelper::DragDrop<std::weak_ptr<IGameObject>>>();
	m_dragDropHandler->source.push_back([](std::weak_ptr<IGameObject> _obj) {return Utility::DragDrop::SourceGameObjectData(_obj); });					//ゲームオブジェクトの保存/親子関係の移動用
	m_dragDropHandler->target.push_back([](std::weak_ptr<IGameObject> _obj) {return Utility::DragDrop::TargetGameObjectData_ChangeParent(_obj); });		//ゲームオブジェクトの親子関係の移動用
	m_dragDropHandler->target.push_back([](std::weak_ptr<IGameObject> _obj) {return Utility::DragDrop::TargetGameObjectDataPath(_obj); });				//ゲームオブジェクトのPrefabからの生成用
}

void Hierarchy::EndChild()
{
	IEditorWindow::EndChild();
	if (!m_isHoverTarget)
	{
		Utility::DragDrop::TargetGameObjectData_ChangeParent({}, true);
		Utility::DragDrop::TargetGameObjectDataPath({});
	}
	GameObjectOption();
}
