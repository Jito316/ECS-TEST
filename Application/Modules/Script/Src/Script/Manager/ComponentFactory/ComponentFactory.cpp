#include "ComponentFactory.h"
#include "AllComponentRegister.h"

void ComponentFactory::Init()
{
	AllComponentRegister(*this);
}

void ComponentFactory::Release()
{
	m_componentDataBase.clear();
	m_registry.clear();
}

void ComponentFactory::RegisterComponent(const type_info& _cmpInfo, std::function<std::shared_ptr<IComponent>()> _creator)
{
	RegistryValue value;
	//コンポーネントの名前
	value.name = _cmpInfo.name();
	Utility::StringHelper::DeleteWord(value.name, "class ");
	Utility::StringHelper::DeleteWord(value.name, "Component");

	//コンポーネントのID
	value.id = std::hash<std::string>()(_cmpInfo.name());

	//コンポーネント生成用の関数
	value.creator = _creator;

	//登録
	m_registry.emplace(value.id, value);
	m_componentDataBase.emplace(value.name, value.id);
}

std::shared_ptr<IComponent> ComponentFactory::CreateComponent(size_t _id)
{
	auto it = m_registry.find(_id);
	if (it == m_registry.end()) {
		ADDLOG(u8"指定されたコンポーネントが見つかりません\n")(JTN::Log::Warning);
		return std::shared_ptr<IComponent>();
	}
	auto cmp = it->second.creator();
	cmp->SetID(_id);
	return cmp;
}

std::shared_ptr<IComponent> ComponentFactory::CreateComponent(std::string_view _name)
{
	auto it = m_componentDataBase.find(_name.data());
	if (it == m_componentDataBase.end()) {
		ADDLOG(u8"指定されたコンポーネントが見つかりません\n")(JTN::Log::Warning);
		return std::shared_ptr<IComponent>();
	}
	return CreateComponent(it->second);
}
