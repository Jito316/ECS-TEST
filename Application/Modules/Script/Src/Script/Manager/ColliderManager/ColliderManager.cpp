#include "ColliderManager.h"
#include "Script/Object/Component/ColliderComponent/ColliderComponent.h"
#include "Script/Object/Component/RigidbodyComponent/RigidbodyComponent.h"
#include "Script/Event/Collider_Event/Collider_Event.h"

void ColliderManager::AddHandler(std::weak_ptr<RigidbodyComponent> _rigid)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _rigid.lock()->GetOwner());
	if (it == m_observers.end())
	{
		Observer obs;
		obs._handler.object = _rigid.lock()->GetOwner();
		obs._handler.rigidbody = _rigid;
		obs._lisner._events.push_back(_rigid.lock().get());
		m_observers.push_back(obs);
		m_infoList.push_back(&m_observers.back());
	}
}
void ColliderManager::AddLisner(Collider_Event* _pEnvet, std::weak_ptr<IGameObject> _object)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _object);
	if (it != m_observers.end())
	{
		it->_lisner._events.push_back(_pEnvet);
	}
}

void ColliderManager::AddCollider(std::weak_ptr<ColliderComponent> _collider)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _collider.lock()->GetOwner());
	if (it != m_observers.end())
	{
		it->_handler.info.push_back(_collider);
	}
	else
	{
		Observer obs;
		obs._handler.object = _collider.lock()->GetOwner();
		obs._handler.info.push_back(_collider);
		m_observers.push_back(obs);
	}
}

void ColliderManager::RemoveHandler(std::weak_ptr<IGameObject> _object)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _object);
	if (it != m_observers.end())
	{
		m_infoList.erase(std::find(m_infoList.begin(), m_infoList.end(), &(*it)));
		m_observers.erase(it);
	}
}

void ColliderManager::RemoveHandler(std::weak_ptr<RigidbodyComponent> _rigid)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _rigid);
	if (it != m_observers.end())
	{
		m_infoList.erase(std::find(m_infoList.begin(), m_infoList.end(), &(*it)));
		m_observers.erase(it);
	}
}

void ColliderManager::RemoveLisner(Collider_Event* _pEnvet, std::weak_ptr<IGameObject> _object)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _object);
	if (it != m_observers.end())
	{
		it->_lisner._events.erase(std::ranges::find(it->_lisner._events, _pEnvet));
	}
}

void ColliderManager::RemoveCollider(std::weak_ptr<ColliderComponent> _collider)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), _collider.lock()->GetOwner());
	if (it == m_observers.end())return;
	if (it->_handler.rigidbody.expired() && it->_handler.info.size() == 1)
	{
		m_observers.erase(it);
		return;
	}

	it->_handler.info.erase(std::ranges::find_if(it->_handler.info, [&_collider](auto& _it) { return _it.lock() == _collider.lock(); }));
	if (it->_handler.rigidbody.lock() && it->_handler.info.empty())
	{
		m_infoList.erase(std::ranges::find(m_infoList, &(*it)));
	}
}

bool ColliderManager::Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::SphereInfo& targetShape, std::list<CollisionResult>* pResults)
{
	bool flg = false;

	for (auto& obs : m_observers)
	{
		if (obs == _object)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		for (auto& it : obs._handler.info)
		{
			if (!it.lock()->GetEnable())continue;

			auto object = it.lock()->GetOwner().lock();
			auto collider = it.lock()->GetCollider().lock();

			std::list<Math::KdCollider::CollisionResult> results;
			if (collider->Intersects(targetShape, object->GetTransform().lock()->GetWorldMatrix(), &results))
			{
				if (pResults)
				{
					CollisionResult result
					(
						results.front(),
						object,
						obs._handler.rigidbody,
						object->GetTransform()
					);
					pResults->push_back(result);
				}

				flg = true;
			}
		}
	}

	return flg;
}
bool ColliderManager::Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::BoxInfo& targetShape, std::list<CollisionResult>* pResults)
{
	bool flg = false;

	for (auto& obs : m_observers)
	{
		if (obs == _object)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		for (auto& it : obs._handler.info)
		{
			if (!it.lock()->GetEnable())continue;

			auto object = it.lock()->GetOwner().lock();
			auto collider = it.lock()->GetCollider().lock();

			std::list<Math::KdCollider::CollisionResult> results;
			if (collider->Intersects(targetShape, object->GetTransform().lock()->GetWorldMatrix(), &results))
			{
				if (pResults)
				{
					CollisionResult result
					(
						results.front(),
						object,
						obs._handler.rigidbody,
						object->GetTransform()
					);
					pResults->push_back(result);
				}

				flg = true;
			}
		}
	}

	return flg;
}
bool ColliderManager::Intersects(std::weak_ptr<IGameObject> _object, const Math::KdCollider::RayInfo& targetShape, std::list<CollisionResult>* pResults)
{
	bool flg = false;

	for (auto& obs : m_observers)
	{
		if (obs == _object)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		for (auto& it : obs._handler.info)
		{
			if (!it.lock()->GetEnable())continue;

			auto object = it.lock()->GetOwner().lock();
			auto collider = it.lock()->GetCollider().lock();

			std::list<Math::KdCollider::CollisionResult> results;
			if (collider->Intersects(targetShape, object->GetTransform().lock()->GetWorldMatrix(), &results))
			{
				if (pResults)
				{
					CollisionResult result
					(
						results.front(),
						object,
						obs._handler.rigidbody,
						object->GetTransform()
					);
					pResults->push_back(result);
				}

				flg = true;
			}
		}
	}

	return flg;
}

bool ColliderManager::Intersects(const Math::KdCollider::SphereInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults)
{
	bool flg = false;
	for (auto& obs : m_observers)
	{
		if (m_nowData._info == &obs)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		m_nowData._collider = &obs;

		for (auto& it : obs._handler.info)
		{
			if (it.lock()->GetEnable())
			{
				auto& collider = *it.lock()->GetCollider().lock();
				std::list<Math::KdCollider::CollisionResult> results;
				if (collider.Intersects(targetShape, m_nowData._collider->_handler.object.lock()->GetTransform().lock()->GetWorldMatrix(), &results))
				{
					OnIntersects(results.front(), it.lock()->IsTrigger() || _isTrigger, pResults);
					flg = true;
				}
			}
		}
	}

	return flg;
}
bool ColliderManager::Intersects(const Math::KdCollider::BoxInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults)
{
	bool flg = false;
	for (auto& obs : m_observers)
	{
		if (m_nowData._info == &obs)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		m_nowData._collider = &obs;

		for (auto& it : obs._handler.info)
		{
			if (it.lock()->GetEnable())
			{
				auto& collider = *it.lock()->GetCollider().lock();
				std::list<Math::KdCollider::CollisionResult> results;
				if (collider.Intersects(targetShape, m_nowData._collider->_handler.object.lock()->GetTransform().lock()->GetWorldMatrix(), &results))
				{
					OnIntersects(results.front(), it.lock()->IsTrigger() || _isTrigger, pResults);
					flg = true;
				}
			}
		}
	}

	return flg;
}
bool ColliderManager::Intersects(const Math::KdCollider::RayInfo& targetShape, const bool _isTrigger, std::list<CollisionResult>* pResults)
{
	bool flg = false;
	for (auto& obs : m_observers)
	{
		if (m_nowData._info == &obs)continue;
		if (!obs._handler.object.lock()->GetEnable())continue;

		m_nowData._collider = &obs;

		for (auto& it : obs._handler.info)
		{
			if (it.lock()->GetEnable())
			{
				auto& collider = *it.lock()->GetCollider().lock();
				std::list<Math::KdCollider::CollisionResult> results;
				if (collider.Intersects(targetShape, m_nowData._collider->_handler.object.lock()->GetTransform().lock()->GetWorldMatrix(), &results))
				{
					OnIntersects(results.front(), it.lock()->IsTrigger() || _isTrigger, pResults);
					flg = true;
				}
			}
		}
	}

	return flg;
}
void ColliderManager::OnIntersects(const Math::KdCollider::CollisionResult& _collision, const bool _isTrigger, std::list<CollisionResult>* _pResults)
{
	{
		auto& handler = m_nowData._collider->_handler;
		CollisionResult temp(_collision, handler.object, handler.rigidbody, handler.object.lock()->GetTransform());

		OnHit(m_nowData._info->_lisner, *m_nowData._collider, temp, _isTrigger);
		if (_pResults)
		{
			_pResults->push_back(temp);
		}
	}

	{
		auto& handler = m_nowData._info->_handler;
		CollisionResult temp(_collision, handler.object, handler.rigidbody, handler.object.lock()->GetTransform());
		temp.m_hitDir *= -1;

		OnHit(m_nowData._collider->_lisner, *m_nowData._info, temp, _isTrigger);
	}
}
void ColliderManager::OnHit(Observer::Lisner& _lisner, Observer& _handler, CollisionResult& _result, const bool _isTrigger) const
{
	auto it = std::ranges::find(_lisner._enteredHandler, &_handler);
	bool isEnter = it == _lisner._enteredHandler.end();

	if (isEnter)
	{
		_lisner._enteredHandler.push_back(&_handler);

		if (_isTrigger)
		{
			for (auto& event : _lisner._events)
			{
				event->OnTrrigerEnter(&_result);
			}
		}
		else
		{
			for (auto& event : _lisner._events)
			{
				event->OnCollisionEnter(&_result);
			}
		}
	}
	else
	{
		if (_isTrigger)
		{
			for (auto& event : _lisner._events)
			{
				event->OnTrrigerStay(&_result);
			}
		}
		else
		{
			for (auto& event : _lisner._events)
			{
				event->OnCollisionStay(&_result);
			}
		}
	}

	_lisner._calledHandler.push_back(&_handler);
}
void ColliderManager::OnExit(Observer::Lisner& _lisner, const CollisionResult& _result) const
{
	for (auto& event : _lisner._events)
	{
		event->OnExit(&_result);
	}
}

void ColliderManager::Update()
{
	for (auto& observer : m_infoList)
	{
		if (observer->_handler.object.lock()->GetEnable())
		{
			m_nowData._info = observer;
			std::list<CollisionResult> results;
			for (auto& info : observer->_handler.info)
			{
				if (info.lock()->GetEnable())
				{
					info.lock()->MakeResult(this, observer->_handler.rigidbody, results);
				}
			}
		}
	}
	m_nowData._info = nullptr;

	for (auto& observer : m_observers)
	{
		auto& lisner = observer._lisner;
		for (auto handler = lisner._enteredHandler.begin()
			; handler != lisner._enteredHandler.end();)
		{
			auto it = std::ranges::find(lisner._calledHandler, *handler);
			if (it == lisner._calledHandler.end())
			{
				CollisionResult temp;
				temp.m_hitObject = (**handler)._handler.object;
				temp.m_rigidbody = (**handler)._handler.rigidbody;
				temp.m_transform = temp.m_hitObject.lock()->GetTransform();
				OnExit(observer._lisner, temp);

				handler = lisner._enteredHandler.erase(handler);
				continue;
			}
			handler++;
		}

		lisner._calledHandler.clear();
	}
}

void ColliderManager::Clear()
{
	m_observers.clear();
	m_infoList.clear();
}

bool ColliderManager::Observer::operator==(const Observer& _other)
{
	return _handler.object.lock() == _other._handler.object.lock();
}
bool ColliderManager::Observer::operator==(const std::weak_ptr<RigidbodyComponent>& _other)
{
	return _handler.rigidbody.lock() == _other.lock();
}
bool ColliderManager::Observer::operator==(const  std::weak_ptr<IGameObject>& _other)
{
	return _handler.object.lock() == _other.lock();
}
