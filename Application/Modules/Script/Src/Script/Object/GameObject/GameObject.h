#pragma once
#include "Interface/Object/GameObject/IGameObject.h"
class IComponent;
class Component;
class TransformComponent;
class RigidbodyComponent;
class BaseRendererComponent;

class GameObject final
	: public IGameObject
	, public std::enable_shared_from_this<GameObject>
{
public:
	GameObject() = default;
	virtual ~GameObject() override = default;

	// 更新
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

	void RenderUpdate()override;
	void ImGuiUpdate()override;

	void Init()override;

	// 名前
	const std::string& GetName() override { return m_name; };
	std::string& WorkName() override { return m_name; };
	void SetName(std::string _name) override { m_name = _name; };

	// タグ
	const ObjectTag& GetTag() const override { return m_tag; };
	ObjectTag& WorkTag() override { return m_tag; };
	void SetTag(ObjectTag _tag) override { m_tag = _tag; };

	// 親関係
	std::weak_ptr<IGameObject> GetParent() override { return m_parent; }
	void SetParent(std::weak_ptr<IGameObject> _parent)override;

	// 子供関係
	const std::list<std::weak_ptr<IGameObject>>& GetChilds() override { return m_childs; }
	std::list<std::weak_ptr<IGameObject>>& WorkChilds() override { return m_childs; }
	const std::weak_ptr<IGameObject> GetChild(std::string_view _name)override;
	const std::weak_ptr<IGameObject> GetChild(const int _index)override;
	void AddChild(std::weak_ptr<IGameObject> _child)override;

	// ハイドフラグ
	bool GetHideFlg() override { return m_bHide; };
	void SetHideFlg(bool _flg) override { m_bHide = _flg; };

	// Json係	
	bool GetAbleSave() const override { return m_bSave; }
	void DontSave()override { m_bSave = false; }
	void EnableSave() override { m_bSave = true; }

	void Serialize(nlohmann::json& _json)override;
	void SerializeFamily(nlohmann::json& _json)override;
	void Deserialize(const nlohmann::json& _json)override;

	// コンポーネント系
	std::shared_ptr<IComponent> AddComponent(size_t _id, const nlohmann::json& _json = nlohmann::json())override;
	std::shared_ptr<IComponent> AddComponent(std::string_view _name, const nlohmann::json& _json = nlohmann::json())override;
	std::shared_ptr<IComponent> AddComponent(std::shared_ptr<IComponent> _instance, const nlohmann::json& _json = nlohmann::json());

	std::shared_ptr<IComponent> GetComponent(size_t _id)override;
	std::shared_ptr<IComponent> GetComponent(std::string_view _name)override;

	std::list<std::weak_ptr<IComponent>> GetComponents(size_t _id)override;
	std::list<std::weak_ptr<IComponent>> GetComponents(std::string_view _name)override;

	const std::list<std::shared_ptr<IComponent>>& GetComponentList() override { return m_cpList; }
	std::list<std::shared_ptr<IComponent>>& WorkComponentList() override { return m_cpList; }

	std::weak_ptr<ITransform> GetTransform() const override;
	std::weak_ptr<ITransform> WorkTransform() override;


	// アクティブフラグ
	bool  GetEnable() const override;
	bool& WorkEnable()override;
	void  SetEnable(bool _flg)override;

	// イベント-破壊
	void Destroy()override;
	bool GetDestroy() const override;

private:
	// オブジェクト
	bool										m_bDestroy = false;
	bool										m_enable = false;

	std::string									m_name = "GameObject";
	ObjectTag									m_tag = ObjectTag::Untagged;
	bool										m_bHide = false;

	// 親子関係＆家族関係
	std::weak_ptr<IGameObject>	m_parent;
	std::list<std::weak_ptr<IGameObject>>	m_childs;

	// トランスフォーム
	std::shared_ptr<TransformComponent>	m_trans;
	// リジットボディ
	std::shared_ptr<RigidbodyComponent>	m_rigidbody;
	// 描画コンポーネント
	bool m_hasRendererComponent = false;

	// コンポ
	std::list<std::shared_ptr<IComponent>>	m_cpList;

	// Json係
	bool										m_bSave = true;
public:
	// 生成系
	static std::shared_ptr<GameObject> CreateObject(std::weak_ptr<IGameObject> _parent = {});
	static std::shared_ptr<GameObject> CreateObject(std::string_view _tag, std::weak_ptr<IGameObject> _parent = {}, std::list<std::shared_ptr<IGameObject>>* _plist = nullptr);
	static std::shared_ptr<GameObject> CreateObject(
		const nlohmann::json& _json,
		std::weak_ptr<IGameObject> _parent = {},
		std::list<std::shared_ptr<IGameObject>>* _plist = nullptr,
		std::list<std::shared_ptr<IGameObject>>::iterator* _pWhere = nullptr);

	// サーチ系
	// 全オブジェクトから指定のコンポーネントを探す
	template<class T> static std::shared_ptr<T> FindObjectOfComponent()
	{
		for (auto& object : GETMODULE(IScript)->GetGameObjectManager()->GetObjectList())
		{
			// 破壊予定もしくは非アクティブの時飛ばす
			if (object->GetDestroy() && (!object->GetEnable()))continue;

			auto cmp = object->GetComponent(ComponentReflection<T>::id);
			if (!cmp)continue;									// 指定のコンポーネントがなければ飛ばす
			return std::static_pointer_cast<T>(cmp);			// 指定のコンポーネントを返す
		}

		ADDLOG(u8"指定したコンポーネントは存在しません");
		return std::shared_ptr<T>();
	}

	// 全オブジェクトから指定のコンポーネントをすべて探す
	template<class T> static std::list<std::weak_ptr<T>> FindObjectOfComponents()
	{
		std::list<std::weak_ptr<T>> list;
		for (auto& object : GETMODULE(IScript)->GetGameObjectManager()->GetObjectList())
		{
			//破壊予定もしくは非アクティブの時飛ばす
			if (object->GetDestroy() && (!object->GetEnable()))continue;

			auto cmps = object->GetComponents(ComponentReflection<T>::id);
			if (cmps.empty())continue;							// 指定のコンポーネントがなければ飛ばす
			for (auto& it : cmps) {
				list.push_back(std::static_pointer_cast<T>(it.lock()));
			}
		}

		return list;
	}

	//　特定オブジェクトの検索
	static std::weak_ptr<IGameObject> FindObject(std::string_view _name)
	{
		auto& list = GETMODULE(IScript)->GetGameObjectManager()->GetObjectList();
		auto it = std::find_if(list.begin(), list.end(), [&_name](const auto& _object) {return _object->GetName() == _name.data(); });
		if (it == list.end()) {
			return std::weak_ptr<IGameObject>();
		}
		return *it;
	}
};