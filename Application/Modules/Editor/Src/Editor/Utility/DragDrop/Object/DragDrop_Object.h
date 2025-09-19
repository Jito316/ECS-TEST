#pragma once
namespace Utility {
	namespace DragDrop {
		constexpr const char* GAMEOBJECT_PATH_ID = "GAMEOBJECT_PATH_ID";
		bool SourceGameObjectDataPath(std::string _path);
		bool TargetGameObjectDataPath(std::weak_ptr<IGameObject> _obj);
		bool TargetGameObjectDataPath(std::string& _path);

		constexpr const char* GAMEOBJECT_ID = "GAMEOBJECT_ID";
		bool SourceGameObjectData(std::weak_ptr<IGameObject> _obj);
		bool TargetGameObjectData(std::weak_ptr<IGameObject>& _obj);
		bool TargetGameObjectData_ChangeParent(std::weak_ptr<IGameObject> _parent, bool _isInsert = false);
		bool TargetGameObjectData_Insert(std::weak_ptr<IGameObject> _parent, std::shared_ptr<IGameObject> _afterObject);
		void TargetGameObjectDataSave(std::string _path);

		std::shared_ptr<IGameObject> GetSourceGameObjectData();
		bool IsChangeParentPossible(std::shared_ptr<IGameObject> _object, std::weak_ptr<IGameObject> _parent);
		bool IsDragObjectData();
	}
}