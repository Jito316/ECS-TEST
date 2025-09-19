#pragma once
#include "Interface/Object/Component/IComponent.h"

class IGameObject;
class ITransform;

class Component
	: public IComponent
	, public std::enable_shared_from_this<Component>
{
public:
	Component() {}
	virtual	~Component() override {}

	virtual void Awake() override {}
	virtual void Start() override {}

	virtual void PreUpdate()	override {}
	virtual void Update()		override {}
	virtual void PostUpdate()	override {}
	virtual void RenderUpdate()	override {}
	virtual void ImGuiUpdate() override {}

	virtual std::weak_ptr<IGameObject> GetOwner() override { return m_owner; }
	virtual void SetOwner(std::weak_ptr<IGameObject> _owner) override;

	//Json係
	virtual void Serialize(nlohmann::json&)override {}
	virtual void Deserialize(const nlohmann::json&) override {}

	virtual const size_t& GetID() const override { return m_id; }
	virtual void SetID(size_t _id) override { m_id = _id; };

	//有効
	virtual bool GetEnable() const override;
	virtual void SetEnable(bool _active) override;
	virtual bool& WorkEnable() override;

	// イベント-破壊
	virtual void Destroy() final override;
	virtual bool GetDestroy() const override;
	virtual void OnDestroy() {};

protected:

	std::weak_ptr<IGameObject>				m_owner;
	std::weak_ptr<ITransform>				m_trans;

	size_t									m_id = 0;

	bool m_enable = true;
	bool m_bDestroy = false;
};

#define GETCOMPONENT(COMPONENT,RESULT)				\
RESULT = m_owner.lock()->GetComponent<COMPONENT>();	\
if (RESULT.expired())								\
{													\
	std::string str = #COMPONENT;					\
	assert(false && (str + "がないよ！").data());	\
	SetEnable(false);								\
	return;											\
}
