#include "DragDrop_Object.h"
#include "Editor/UndoRedo/UndoRedo.h"
#include "Editor/UndoRedo/Action/AddGameObject/AddGameObject.h"
#include "Editor/UndoRedo/Action/MoveGameObject/MoveGameObject.h"

namespace Utility {
	namespace DragDrop {
		bool SourceGameObjectDataPath(std::string _path)
		{
			return Utility::ImGuiHelper::DragDropSource(GAMEOBJECT_PATH_ID, _path);
		}
		bool TargetGameObjectDataPath(std::weak_ptr<IGameObject> _obj)
		{
			bool flg = false;
			if (std::string path; Utility::ImGuiHelper::DragDropTarget(GAMEOBJECT_PATH_ID, path))
			{
				std::list<std::shared_ptr<IGameObject>> results;
				SCRIPT_COMMAND("CreateObject", path.c_str(), &_obj, &results);

				GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::AddGameObject>(results));
				flg = true;
			}
			return flg;
		}
		bool TargetGameObjectDataPath(std::string& _path)
		{
			return Utility::ImGuiHelper::DragDropTarget(GAMEOBJECT_PATH_ID, _path);
		}

		bool SourceGameObjectData(std::weak_ptr<IGameObject> _obj)
		{
			return Utility::ImGuiHelper::DragDropSource(GAMEOBJECT_ID, _obj);
		}
		bool TargetGameObjectData(std::weak_ptr<IGameObject>& _obj)
		{
			return Utility::ImGuiHelper::DragDropTarget(GAMEOBJECT_ID, _obj);
		}
		bool TargetGameObjectData_ChangeParent(std::weak_ptr<IGameObject> _parent, bool _isInsert)
		{
			if (IsDragObjectData() && Utility::ImGuiHelper::IsItemMouseHovering()) {
				std::shared_ptr<IGameObject> obj = GetSourceGameObjectData();
				if (obj->GetParent().lock() == _parent.lock()) {
					return false;
				}
				if (!IsChangeParentPossible(obj, _parent)) {
					return false;
				}
			}
			else return false;

			std::shared_ptr<IGameObject> obj;
			if (Utility::ImGuiHelper::DragDropTarget(GAMEOBJECT_ID, obj))
			{
				std::shared_ptr<IGameObject> temp = nullptr;
				if (_isInsert)
				{
					auto& list = GETMODULE(IScript)->GetGameObjectManager()->GetObjectList();
					temp = UndoRedoAction::MoveGameObject::pick_endChild(obj);
				}

				GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::MoveGameObject>(
					_parent
					, obj
					, temp
				));
				return true;
			}
			return obj == nullptr;
		}
		bool TargetGameObjectData_Insert(std::weak_ptr<IGameObject> _parent, std::shared_ptr<IGameObject> _afterObject)
		{
			if (!IsDragObjectData())return false;
			
			// �O��Tree�A�C�e���̋��E���擾
			float previousItemBottom = ImGui::GetItemRectMax().y;

			// ���݂�Tree�A�C�e�����W
			ImVec2 currentItem = ImGui::GetCursorScreenPos();

			// Item�Ԃ�Rect
			ImRect betweenSpace = { { currentItem.x,previousItemBottom }, { ImGui::GetWindowSize().x,currentItem.y } };

			auto& g = *ImGui::GetCurrentContext();
			auto obj = Utility::DragDrop::GetSourceGameObjectData();
			if (ImGui::IsMouseHoveringRect(betweenSpace.Min, betweenSpace.Max)						// �h���b�O���̗v�f������ꍇ
				&& g.DragDropPayload.SourceId != g.LastItemData.ID									// �\�[�X�ւ̃h���b�v�֎~
				&& (obj != _afterObject || (_parent.lock() && _parent.lock() != obj))				// ���g�Ƃ̓���ւ��֎~ �������A�e�q�֌W�̕ύX�̏ꍇ�͗�O
				&& IsChangeParentPossible(obj, _parent)												// �q���ւ̃h���b�v�֎~
				) {

				// �}���ʒu���������C���̕`��
				float lineY = (previousItemBottom + currentItem.y) * 0.5f;

				ImGui::GetWindowDrawList()->AddLine(
					ImVec2(currentItem.x, lineY),
					ImVec2(ImGui::GetWindowSize().x, lineY),
					ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_DragDropTarget]),
					2.0f
				);
				// Drop����
				if (!ImGui::IsMouseDown(g.DragDropMouseButton)) {
					GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::MoveGameObject>(
						_parent
						, obj
						, _afterObject
					));
					ImGui::ClearDragDrop();
				}
				return true;
			}
			return false;
		}
		void TargetGameObjectDataSave(std::string _path)
		{
			if (std::weak_ptr<IGameObject> obj; Utility::ImGuiHelper::DragDropTarget(GAMEOBJECT_ID, obj))
			{
				nlohmann::json data;
				obj.lock()->SerializeFamily(data);
				Utility::JsonHelper::OutputJson(data, (_path.empty() ? "" : _path + "/") + obj.lock()->GetName() + ".prefab");
			}
		}

		std::shared_ptr<IGameObject> GetSourceGameObjectData()
		{
			std::shared_ptr<IGameObject> temp;
			Utility::ImGuiHelper::GetDragData(GAMEOBJECT_ID, temp);
			return temp;
		}

		bool IsChangeParentPossible(std::shared_ptr<IGameObject> _object, std::weak_ptr<IGameObject> _parent) {
			// �h���b�v�悪�����̎q���⎩���ł������Ƃ���
			// �h���b�v���Ȃ�����
			std::function<bool(std::weak_ptr<IGameObject>)>Fn
				= [&](std::weak_ptr<IGameObject> _obj)
				{
					if (_obj.lock() == _parent.lock())
					{
						return false;
					}

					bool flg = true;
					for (auto& it : _obj.lock()->GetChilds())
					{
						flg = Fn(it);
						if (!flg)break;
					}

					return flg;
				};

			return Fn(_object);
		}

		bool IsDragObjectData()
		{
			return Utility::ImGuiHelper::IsDragging(GAMEOBJECT_ID);
		}
	}
}