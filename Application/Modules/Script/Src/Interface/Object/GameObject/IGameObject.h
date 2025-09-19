#pragma once
#include "Interface/Object/IObject.h"

class IComponent;
class ITransform;

class SCRIPT_API IGameObject :public IObject
{
public:
	IGameObject() = default;
	virtual ~IGameObject() override {};

	enum class ObjectTag
	{
		Untagged,
		Respawn,
		Finish,
		EditorOnly,
		MainCamera,
		Player,
		Enemy,
		GameController
	};

	// 更新系
	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void RenderUpdate() = 0;
	virtual void ImGuiUpdate() = 0;

	virtual void Init() = 0;

	//名前
	virtual const std::string& GetName() = 0;
	virtual std::string& WorkName() = 0;
	virtual void SetName(std::string _name) = 0;

	//タグ
	virtual const ObjectTag& GetTag() const = 0;
	virtual ObjectTag& WorkTag() = 0;
	virtual void SetTag(ObjectTag _tag) = 0;

	//親関係
	virtual std::weak_ptr<IGameObject> GetParent() = 0;
	virtual void SetParent(std::weak_ptr<IGameObject> _parent) = 0;

	//子供関係
	virtual const std::list<std::weak_ptr<IGameObject>>& GetChilds() = 0;
	virtual std::list<std::weak_ptr<IGameObject>>& WorkChilds() = 0;
	virtual const std::weak_ptr<IGameObject> GetChild(std::string_view _name) = 0;
	virtual const std::weak_ptr<IGameObject> GetChild(const int _index) = 0;
	virtual void AddChild(std::weak_ptr<IGameObject> _child) = 0;

	//ハイドフラグ
	virtual bool GetHideFlg() = 0;
	virtual void SetHideFlg(bool _flg) = 0;

	//Json係	
	virtual void Serialize(nlohmann::json& _json) = 0;
	virtual void SerializeFamily(nlohmann::json& _json) = 0;
	virtual void Deserialize(const nlohmann::json& _json) = 0;

	virtual bool GetAbleSave() const = 0;
	virtual void DontSave() = 0;
	virtual void EnableSave() = 0;

	//コンポーネント系
	virtual std::shared_ptr<IComponent> AddComponent(size_t _id, const nlohmann::json& _json = nlohmann::json()) = 0;
	virtual std::shared_ptr<IComponent> AddComponent(std::string_view _name, const nlohmann::json& _json = nlohmann::json()) = 0;
	template<class T>std::shared_ptr<IComponent> AddComponent(const nlohmann::json& _json = nlohmann::json())
	{
		return AddComponent(ComponentReflection<T>::id, _json);
	}

	virtual std::shared_ptr<IComponent> GetComponent(size_t _id) = 0;
	virtual std::shared_ptr<IComponent> GetComponent(std::string_view _name) = 0;
	template<class T>std::shared_ptr<T> GetComponent()
	{
		size_t ID = ComponentReflection<T>::id;
		std::shared_ptr<IComponent> cmp = GetComponent(ID);
		return std::static_pointer_cast<T>(cmp);
	}

	virtual std::list<std::weak_ptr<IComponent>> GetComponents(size_t _id) = 0;
	virtual std::list<std::weak_ptr<IComponent>> GetComponents(std::string_view _name) = 0;
	template<class T>std::list<std::weak_ptr<T>> GetComponents()
	{
		size_t ID = ComponentReflection<T>::id;
		std::list<std::weak_ptr<IComponent>> list = GetComponents(ID);
		if (list.empty())ADDLOG(u8"指定したコンポーネントがありません")(JTN::Log::State::Warning);
		return *reinterpret_cast<std::list<std::weak_ptr<T>>*>(&list);
	}

	virtual const std::list<std::shared_ptr<IComponent>>& GetComponentList() = 0;
	virtual std::list<std::shared_ptr<IComponent>>& WorkComponentList() = 0;

	virtual std::weak_ptr<ITransform> GetTransform() const = 0;
	virtual std::weak_ptr<ITransform> WorkTransform() = 0;
};