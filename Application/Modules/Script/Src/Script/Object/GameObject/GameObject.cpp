#include "GameObject.h"
#include "Script/Script.h"
#include "Script/Manager/ObjectManager/GameObjectManager.h"
#include "Script/Object/Component/Component.h"
#include "Script/Manager/ComponentFactory/ComponentFactory.h"
#include "Script/Object/Component/TransformComponent/TransformComponent.h"
#include "Script/Object/Component/RigidbodyComponent/RigidbodyComponent.h"
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

#include "Script/Object/Component/RendererComponent/ModelRendererComponent/ModelRendererComponent.h"
#include "Script/Object/Component/RendererComponent/SquarePolygonRendererComponent/SquarePolygonRendererComponent.h"
#include "Script/Object/Component/RendererComponent/TextureRendererComponent/TextureRendererComponent.h"

void GameObject::PreUpdate()
{
	for (auto& it : m_cpList)
	{
		if (it->GetEnable() && !it->GetDestroy())
		{
			it->PreUpdate();
		}
	}
}
void GameObject::Update()
{
	for (auto& it : m_cpList)
	{
		if (it->GetEnable() && !it->GetDestroy())
		{
			it->Update();
		}
	}
}
void GameObject::PostUpdate()
{
	for (auto& it : m_cpList)
	{
		if (it->GetEnable() && !it->GetDestroy())
		{
			it->PostUpdate();
		}
	}
}

void GameObject::RenderUpdate()
{
	auto cmp = m_cpList.begin();
	while (cmp != m_cpList.end())
	{
		if ((**cmp).GetDestroy())
		{
			cmp = m_cpList.erase(cmp);
			continue;
		}
		if ((**cmp).GetEnable()) 
		{
			(**cmp).RenderUpdate();
		}
		cmp++;
	}
}

void GameObject::ImGuiUpdate()
{
	for (auto& it : m_cpList)
	{
		it->ImGuiUpdate();
	}
}

void GameObject::Init()
{
	SetEnable(true);
	m_trans = std::make_shared<TransformComponent>();
}

void GameObject::SetParent(std::weak_ptr<IGameObject> _parent)
{
	if (m_parent.lock() != _parent.lock())
	{
		if (!m_parent.expired())
		{
			std::list<std::weak_ptr<IGameObject>>& list = m_parent.lock()->WorkChilds();
			std::list<std::weak_ptr<IGameObject>>::iterator it
				= std::find_if
				(
					list.begin(),
					list.end(),
					[this](std::weak_ptr<IGameObject> _object)
					{
						return _object.lock().get() == this;
					}
				);
			if (it != list.end())list.erase(it);
		}

		m_parent = _parent;

		if (m_trans)
		{
			m_trans->SetParent(_parent.expired() ? std::weak_ptr<ITransform>() : _parent.lock()->GetTransform());
		}
	}
}

const std::weak_ptr<IGameObject> GameObject::GetChild(std::string_view _name)
{
	std::list<std::weak_ptr<IGameObject>>::iterator it
		= std::find_if
		(
			m_childs.begin(),
			m_childs.end(),
			[_name](std::weak_ptr<IGameObject> _object)
			{
				return _object.lock()->GetName()
					== _name.data();
			}
		);

	return it == m_childs.end() ? std::weak_ptr<IGameObject>() : *it;
}

const std::weak_ptr<IGameObject> GameObject::GetChild(const int _index)
{
	if (m_childs.size() > static_cast<size_t>(_index))
	{
		return *std::next(m_childs.begin(), _index);
	}
	return std::weak_ptr<IGameObject>();
}

void GameObject::AddChild(std::weak_ptr<IGameObject> _child)
{
	_child.lock()->SetParent(shared_from_this());
	m_childs.push_back(_child);
}

void GameObject::Destroy()
{
	if (!m_bDestroy)
	{
		for (auto& cmp : m_cpList)
		{
			if (cmp)cmp->Destroy();
		}
		for (auto& child : m_childs)
		{
			if (child.lock())child.lock()->Destroy();
		}
		m_bDestroy = true;
	}
}

bool GameObject::GetDestroy() const
{
	return m_bDestroy;
}

std::shared_ptr<IComponent> GameObject::AddComponent(size_t _id, const nlohmann::json& _json)
{
	auto& script = GETSCRIPT;
	std::shared_ptr<IComponent> addCp = script.GetComponentFactroy()->CreateComponent(_id);
	return AddComponent(addCp, _json);
}

std::shared_ptr<IComponent> GameObject::AddComponent(std::string_view _name, const nlohmann::json& _json)
{
	auto temp = GETSCRIPT.GetComponentFactroy()->CreateComponent(_name);
	return AddComponent(temp, _json);
}

std::shared_ptr<IComponent> GameObject::AddComponent(std::shared_ptr<IComponent> _instance, const nlohmann::json& _json)
{
	if (_instance) {
		if (_instance->GetID() == ComponentReflection<RigidbodyComponent>::id)
		{
			if (m_rigidbody)
			{
				ADDLOG(u8"重複不可能なコンポーネントです。");
				return nullptr;
			}
			else
			{
				m_rigidbody = std::static_pointer_cast<RigidbodyComponent>(_instance);
			}
		}
		if (_instance->GetID() == ComponentReflection<ModelRendererComponent>::id
			|| _instance->GetID() == ComponentReflection<SquarePolygonRendererComponent>::id
			|| _instance->GetID() == ComponentReflection<RigidbodyComponent>::id)
		{
			if (m_hasRendererComponent)
			{
				ADDLOG(u8"重複不可能なコンポーネントです。");
				return nullptr;
			}
			else
			{
				m_hasRendererComponent = true;
			}
		}

		auto& script = GETSCRIPT;
		m_cpList.push_back(_instance);

		_instance->SetOwner(shared_from_this());
		_instance->Awake();

		if (!_json.is_null())
		{
			_instance->Deserialize(_json);
		}

		GETGAMEOBJECTMANAGER.EnqueueForInitialization(_instance);
	}
	return _instance;
}

std::shared_ptr<IComponent> GameObject::GetComponent(size_t _id)
{
	for (auto& it : m_cpList) {
		if (_id == it->GetID()) {
			return it;
		}
	}

	return nullptr;
}
std::shared_ptr<IComponent> GameObject::GetComponent(std::string_view _name)
{
	auto factroy = GETSCRIPT.GetComponentFactroy();
	const auto& dataBase = factroy->GetComponentDataBase();
	auto it = dataBase.find(_name.data());
	if (it != dataBase.end()) {
		return GetComponent(it->second);
	}

	return nullptr;
}

std::list<std::weak_ptr<IComponent>> GameObject::GetComponents(size_t _id)
{
	std::list<std::weak_ptr<IComponent>> list;
	for (auto& it : m_cpList)
	{
		const size_t& targetID = it->GetID();
		if (_id == targetID)
		{
			list.push_back(it);
		}
	}

	return list;
}

std::list<std::weak_ptr<IComponent>> GameObject::GetComponents(std::string_view _name)
{
	auto factroy = GETSCRIPT.GetComponentFactroy();
	const auto& dataBase = factroy->GetComponentDataBase();
	auto it = dataBase.find(_name.data());
	if (it != dataBase.end()) {
		return GetComponents(it->second);
	}

	return std::list<std::weak_ptr<IComponent>>();
}

std::weak_ptr<ITransform> GameObject::GetTransform() const
{
	return m_trans;
}

std::weak_ptr<ITransform> GameObject::WorkTransform()
{
	return m_trans;
}

void GameObject::Serialize(nlohmann::json& _objData)
{
	if (!m_bSave)return;
	nlohmann::json cmpsData = nlohmann::json::array();

	{
		nlohmann::json transData;
		m_trans->Serialize(transData);
		cmpsData.push_back(transData);
	}

	for (auto&& it : m_cpList)
	{
		nlohmann::json cmpData;
		it->Serialize(cmpData);
		cmpData["Enable"] = it->GetEnable();
		cmpData["ID"] = it->GetID();
		cmpsData.push_back(cmpData);
	}

	nlohmann::json object;
	object["tag"] = m_tag;
	object["Component"] = cmpsData;
	object["name"] = m_name;
	object["active"] = m_enable;

	_objData["_Data"] = object;
	_objData["Childs"] = nlohmann::json::array();
}
void GameObject::SerializeFamily(nlohmann::json& _json)
{
	Serialize(_json);
	for (auto& child : m_childs)
	{
		if (child.expired())continue;
		nlohmann::json childData;
		child.lock()->SerializeFamily(childData);
		if (child.lock()->GetAbleSave())_json["Childs"].push_back(childData);
	}
}
void GameObject::Deserialize(const nlohmann::json& _objData)
{
	if (_objData.is_null())return;

	m_trans->Deserialize(_objData["_Data"]["Component"][0]);

	m_tag = _objData["_Data"]["tag"];
	m_name = _objData["_Data"]["name"];
	SetEnable(_objData["_Data"].value("active", m_enable));


	m_cpList.clear();
	for (auto& cmpData : _objData["_Data"]["Component"])
	{
		auto Key = cmpData.begin();
		while (Key != cmpData.end())
		{
			if (Key.key() == "ID")
			{
				auto temp = AddComponent(cmpData["ID"].get<int>(), cmpData);
				if (temp)
				{
					temp->SetEnable(cmpData.value("Enable", temp->GetEnable()));
				}
				break;
			}
			Key++;
		}
	}
}

bool GameObject::GetEnable() const
{
	if (m_enable)
	{
		if (m_parent.lock())
		{
			return m_parent.lock()->GetEnable();
		}
		return true;
	}
	return false;
}

bool& GameObject::WorkEnable()
{
	return m_enable;
}

void GameObject::SetEnable(bool _flg)
{
	m_enable = _flg;
}

std::shared_ptr<GameObject> GameObject::CreateObject(std::weak_ptr<IGameObject> _parent)
{
	return CreateObject(nlohmann::json(), _parent);
}
std::shared_ptr<GameObject> GameObject::CreateObject(std::string_view _tag, std::weak_ptr<IGameObject> _parent, std::list<std::shared_ptr<IGameObject>>* _plist)
{
	return CreateObject(Utility::JsonHelper::InputJson(_tag.data()), _parent, _plist);
}
std::shared_ptr<GameObject> GameObject::CreateObject(const nlohmann::json& _json, std::weak_ptr<IGameObject> _parent, std::list<std::shared_ptr<IGameObject>>* _plist, std::list<std::shared_ptr<IGameObject>>::iterator* _pWhere)
{
	bool root = false;
	auto& list = GETGAMEOBJECTMANAGER.WorkObjectList();
	auto object = std::make_shared<GameObject>();
	auto parent = _parent.lock();

	if (_pWhere == nullptr)
	{
		root = false;
		_pWhere = new std::list<std::shared_ptr<IGameObject>>::iterator(list.end());
		if (parent && parent->GetChilds().size())
		{
			std::shared_ptr<IGameObject> temp = parent->GetChilds().back().lock();
			while (temp->GetChilds().size())
			{
				temp = temp->GetChilds().back().lock();
			}

			(*_pWhere) = std::ranges::find(list, temp);
			(*_pWhere)++;
		}
	}
	(*_pWhere) = list.insert((*_pWhere), object);
	(*_pWhere)++;

	if (_plist)
	{
		_plist->push_back(object);
	}

	object->Init();
	if (parent)
	{
		object->SetParent(parent);
		parent->AddChild(object);
	}
	object->Deserialize(_json);

	if (_json.contains("Childs"))
	{
		for (auto& child : _json["Childs"])CreateObject(child, object, _plist, _pWhere);
	}

	if (root) 
	{
		delete _pWhere;
	}
	return object;
}