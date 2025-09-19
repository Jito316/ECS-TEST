#pragma once
#include "Interface/Object/IObject.h"
class IGameObject;

class SCRIPT_API IComponent :public IObject
{
public:
	IComponent() {}
	virtual	~IComponent() {}

	virtual void Awake() = 0;
	virtual void Start() = 0;

	virtual void PreUpdate()	= 0;
	virtual void Update()		= 0;
	virtual void PostUpdate()	= 0;

	virtual void RenderUpdate() = 0;

	virtual void ImGuiUpdate() = 0;

	virtual std::weak_ptr<IGameObject> GetOwner() = 0;
	virtual void SetOwner(std::weak_ptr<IGameObject> _owner) = 0;

	virtual const size_t& GetID() const = 0;
	virtual void SetID(size_t _id) = 0;

	//Json係
	virtual void Serialize(nlohmann::json&) = 0;
	virtual void Deserialize(const nlohmann::json&) = 0;
};

template<typename T>concept FromComponent = std::is_base_of_v<IComponent, T>;
template<FromComponent T> struct ComponentReflection
{
	static constexpr const std::type_info& info = typeid(T);
	inline static size_t id = std::hash<std::string>()(info.name());
};