#pragma once
class IComponent;

class SCRIPT_API IComponentFactory
{
public:
	IComponentFactory() = default;
	virtual ~IComponentFactory() = default;

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void RegisterComponent(const type_info& _cmpInfo, std::function<std::shared_ptr<IComponent>()> _creator) = 0;
	template<class T> void RegisterComponent()
	{
		static_assert(std::is_base_of_v<IComponent, T>, "指定したクラスがコンポーネントを基底としてません");

		RegisterComponent(
			typeid(T)
			, []() {
				std::shared_ptr<T> temp = std::make_shared<T>();
				return temp;
			});
	}
	virtual std::shared_ptr<IComponent>CreateComponent(size_t _id) = 0;
	virtual std::shared_ptr<IComponent>CreateComponent(std::string_view _name) = 0;
	virtual const std::map<std::string, size_t>& GetComponentDataBase() = 0;
};
