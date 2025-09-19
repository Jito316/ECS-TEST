#include "Inspector.h"
#include "Editor/UndoRedo/Action/AddComponent/AddComponent.h"
#include "Editor/UndoRedo/Action/DeleteComponent/DeleteComponent.h"
#include "Editor/UndoRedo/Action/MoveComponent/MoveComponent.h"

void Inspector::UpdateContents()
{
	std::weak_ptr<IGameObject> object = GETEDITOR.GetEditObject();
	ImGuiGameObject(object);
}

void Inspector::ImGuiGameObject(std::weak_ptr<IGameObject> _object)
{
	if (_object.expired())return;
	ImGui::Separator();
	{
		ShowGameObject(_object);
	}
	ImGui::Separator();
	ImGui::BeginChild((ImGuiID)_object.lock().get());
	{
		ShowComponent(_object);
		AddComponent(_object);
	}
	ImGui::EndChild();
}

void Inspector::ShowGameObject(std::weak_ptr<IGameObject> _object)
{
	bool prevEnable = _object.lock()->GetEnable();
	if (ImGui::Checkbox("##Active", &_object.lock()->WorkEnable()))
	{
		GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<bool>>(&_object.lock()->WorkEnable(), &prevEnable));
	}

	ImGui::SameLine();
	ImGui::InputText("Name", &_object.lock()->WorkName());
	Utility::ImGuiHelper::ComboEnum("Tag", _object.lock()->WorkTag());
}
void Inspector::ShowComponent(std::weak_ptr<IGameObject> _object)
{
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Transform"))
	{
		_object.lock()->GetTransform().lock()->ImGuiUpdate();
		ImGui::TreePop();
	}

	for (auto it = _object.lock()->WorkComponentList().begin()
		; it != _object.lock()->WorkComponentList().end()
		; it++)
	{
		DragComponentTraget(it);
		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		bool opened = ComponentTreeNode(it);
		DragComponentSource(it);
		if (opened)
		{
			(*it)->ImGuiUpdate();
			ImGui::TreePop();
		}

	}
	DragComponentTraget(_object.lock()->WorkComponentList().end());
}
void Inspector::AddComponent(std::weak_ptr<IGameObject> _object)
{
	static std::string keyword;

	ImGui::PushID((void*)_object.lock().get());
	if (Utility::ImGuiHelper::SmallButtonWindowCenter("AddComponent"))ImGui::OpenPopup("Components");
	if (ImGui::BeginPopup("Components"))
	{
		ImGui::InputText("serch", &keyword);
		ImGui::SeparatorText("Component");

		const auto& factory = GETMODULE(IScript)->GetComponentFactroy()->GetComponentDataBase();
		for (auto& [name, id] : factory) {
			if (!keyword.empty() && name.find(keyword) == std::string::npos)continue;
			if (!ImGui::MenuItem(name.data()))continue;

			auto temp = _object.lock()->AddComponent(id);
			if (temp)
			{
				GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::AddComponent>(temp));
			}
		}
		ImGui::EndPopup();
	}
	else keyword.clear();
	ImGui::PopID();
}

bool Inspector::DragComponentSource(std::list<std::shared_ptr<IComponent>>::iterator _cmpIt)
{
	// コンポーネントのドラッグ開始を設定
	if (Utility::ImGuiHelper::DragDropSource("Component", m_drag.cmp)) {
		m_drag.cmp = *_cmpIt;
		m_drag.pos = _cmpIt;
		return true;
	}
	return false;
}

bool Inspector::DragComponentTraget(std::list<std::shared_ptr<IComponent>>::iterator _cmpIt)
{
	// ドラッグ操作が無い、または無効な場合は早期リターン
	if (!m_drag.cmp) return false;
	if (_cmpIt == m_drag.pos) return false;

	auto& list = GETEDITOR.GetEditObject().lock()->WorkComponentList();

	// 直前の要素が既にドラッグ中のコンポーネントの場合は処理をスキップ
	if (_cmpIt != list.begin()) {
		auto prevIt = std::prev(_cmpIt);
		if (*prevIt == m_drag.cmp) return false;
	}

	// ドロップ領域の設定
	ImGui::InvisibleButton(std::to_string((int)&_cmpIt).data(), { ImGui::GetWindowSize().x, ImGui::GetTextLineHeight() });

	bool flg = false;
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Component")) {
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::MoveComponent>(
				std::distance(list.begin(), m_drag.pos)
				, m_drag.cmp
				, _cmpIt == list.end() ? nullptr : (*_cmpIt)
				, &list
			));

			// ドロップ位置に応じてコンポーネントを移動
			if (_cmpIt != list.end()) {
				list.insert(_cmpIt, m_drag.cmp);
			}
			else {
				list.push_back(m_drag.cmp);
			}
			list.erase(m_drag.pos);

			m_drag.cmp = nullptr;
			flg = true;
		}
		ImGui::EndDragDropTarget();
	}
	return flg;
}

bool Inspector::ComponentTreeNode(std::list<std::shared_ptr<IComponent>>::iterator _cmp)
{
	// IDの設定
	auto& component = *_cmp->get();
	int ID = component.GetID();
	float prevCursorPosY = ImGui::GetCursorPosY();
	ImGui::PushID(&component);


	ImGui::Indent();
	{
		float availX = ImGui::GetContentRegionAvail().x;

		// 有効化フラグ
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0.25f));
		{
			bool prevEnable = component.GetEnable();
			if (ImGui::Checkbox("##Active", &component.WorkEnable()))
			{
				GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<bool>>(&component.WorkEnable(), &prevEnable));
			}
		}
		ImGui::PopStyleVar();

		// コンポーネント名
		ImGui::SameLine();
		std::string name = "Component";
		auto& dataBase = GETMODULE(IScript)->GetComponentFactroy()->GetComponentDataBase();
		auto it = std::find_if(dataBase.begin(), dataBase.end(), [ID](const std::pair<std::string, size_t>& _pair) {return ID == _pair.second; });
		if (dataBase.end() != it)
		{
			name = it->first;
		}
		ImGui::Text(name.c_str());

		// 削除
		ImGui::SameLine();
		ImGui::SetCursorPosX(availX);
		if (ImGui::SmallButton((const char*)u8"×"))
		{
			GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::DeleteComponent>(*_cmp));
		}

	}
	ImGui::Unindent();

	// TreeNode開始
	ImGui::SetCursorPosY(prevCursorPosY);
	bool opened = ImGui::TreeNodeEx("##Component", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen);

	ImGui::PopID();
	return opened;
}
