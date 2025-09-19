#pragma once
class IGameObject;
class RigidbodyComponent;
class ColliderComponent;
class Collider_Event;
struct CollisionResult;

#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"

class ColliderManager
{
public:
	// オブザーバーの設定
	void AddHandler(std::weak_ptr<RigidbodyComponent> _rigid);
	void AddLisner(Collider_Event* _pEnvet, std::weak_ptr<IGameObject> _object);
	void AddCollider(std::weak_ptr<ColliderComponent> _collider);

	void RemoveHandler(std::weak_ptr<IGameObject> _object);
	void RemoveHandler(std::weak_ptr<RigidbodyComponent> _rigid);
	void RemoveLisner(Collider_Event* _pEnvet, std::weak_ptr<IGameObject> _object);
	void RemoveCollider(std::weak_ptr<ColliderComponent> _collider);

	// 当たり判定生成
	bool Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::SphereInfo& targetShape, std::list<CollisionResult>* pResults);
	bool Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::BoxInfo& targetShape, std::list<CollisionResult>* pResults);
	bool Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::RayInfo& targetShape, std::list<CollisionResult>* pResults);

private:
	struct Observer
	{
		bool operator == (const Observer& _other);
		bool operator == (const std::weak_ptr<RigidbodyComponent>& _other);
		bool operator == (const std::weak_ptr<IGameObject>& _other);

		struct Handler
		{
			std::weak_ptr<RigidbodyComponent> rigidbody;
			std::weak_ptr<IGameObject> object;
			std::list <std::weak_ptr<ColliderComponent>> info;
		}_handler;

		struct Lisner
		{
			std::list<Collider_Event*> _events;
			std::list<Observer*> _enteredHandler;
			std::list<Observer*> _calledHandler;
		}_lisner;
	};

	friend class ColliderComponent;
	bool Intersects(const Math::KdCollider::SphereInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults);
	bool Intersects(const Math::KdCollider::BoxInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults);
	bool Intersects(const Math::KdCollider::RayInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults);
	
	void OnIntersects(const Math::KdCollider::CollisionResult& _collision, const bool _isTrigger, std::list<CollisionResult>* _pResults = nullptr);
	void OnHit(Observer::Lisner& _lisner, Observer& _handler, CollisionResult& _result, const bool _isTrigger)const;
	void OnExit(Observer::Lisner& _lisner,const CollisionResult& _result)const;

	struct
	{
		Observer* _info = nullptr;
		Observer* _collider = nullptr;
	}m_nowData;


	std::list<Observer> m_observers;
	std::list<Observer*> m_infoList;

public:
	void Update();
	void Clear();
};