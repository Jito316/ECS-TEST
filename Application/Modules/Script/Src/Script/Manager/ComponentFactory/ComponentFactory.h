#pragma once
#include "Interface/Manager/ComponentFactory/IComponentFactory.h"

class ComponentFactory : public IComponentFactory
{
public:
	struct RegistryValue
	{
		std::string name = "";
		size_t id = 0;
		std::function<std::shared_ptr<IComponent>()> creator = {};
	};

	ComponentFactory() = default;
	~ComponentFactory()override = default;

	void Init()override;
	void Release()override;

	void RegisterComponent(const type_info& _cmpInfo, std::function<std::shared_ptr<IComponent>()> _creator)override;
	std::shared_ptr<IComponent> CreateComponent(size_t _id)override;
	std::shared_ptr<IComponent> CreateComponent(std::string_view _name)override;

	const std::map<std::string, size_t>& GetComponentDataBase() override {
		return m_componentDataBase;
	}

private:
	std::unordered_map<size_t, RegistryValue> m_registry;
	std::map<std::string, size_t> m_componentDataBase;

};