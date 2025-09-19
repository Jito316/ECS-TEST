#include "MoveGameObject.h"
namespace UndoRedoAction {
	MoveGameObject::MoveGameObject(
		std::weak_ptr<IGameObject> _parent
		, std::shared_ptr<IGameObject> _object
		, std::shared_ptr<IGameObject> _afterObject)
		: m_pList(&GETMODULE(IScript)->GetGameObjectManager()->WorkObjectList())
		, Base(m_pList)
		, m_parent(_parent)
		, m_object(_object)
		, m_afterObject(_afterObject)
		, m_objectListindex(std::distance(m_pList->begin(), std::find(m_pList->begin(), m_pList->end(), m_object)))
		, m_childListindex()
	{
		push_child(m_object, m_objects);
	}

	void MoveGameObject::Init()
	{
		Redo();
	}

	void MoveGameObject::Undo()
	{
		auto postParent = m_object->GetParent();

		if (postParent.lock()) {
			auto& list = postParent.lock()->WorkChilds();
			auto eraseIt = std::find_if(list.begin(), list.end(), [&](const auto& _obj) {return _obj.lock() == m_object; });
			list.erase(eraseIt);
		}

		if (m_pList) {
			// 元の位置から削除
			erase_objects(m_pList, m_objects);

			auto insertIt = std::next(m_pList->begin(), m_objectListindex);
			m_pList->insert(insertIt, m_objects.begin(), m_objects.end());
		}

		if (m_parent.lock()) {
			auto& list = m_parent.lock()->WorkChilds();
			auto insertIt = std::next(list.begin(), m_childListindex);
			list.insert(insertIt, m_object);
		}

		m_object->SetParent(m_parent);

		m_parent = postParent;
	}

	void MoveGameObject::Redo()
	{
		auto postParent = m_object->GetParent();

		// 親子関係を変更
		if (postParent.lock()) {
			auto& list = postParent.lock()->WorkChilds();
			auto eraseIt = std::find_if(list.begin(), list.end(), [&](const auto& _obj) {return _obj.lock() == m_object; });
			m_childListindex = std::distance(list.begin(), eraseIt);
			list.erase(eraseIt);
		}

		// オブジェクトリストのリスト移動処理
		if (m_pList) {
			// 元の位置から削除
			erase_objects(m_pList, m_objects);

			// 移動後の位置に挿入
			auto parent = m_parent.lock();
			auto insertIt = m_pList->begin();
			if (parent && !m_afterObject)
			{
				insertIt = std::ranges::find(m_pList->begin(), m_pList->end(), pick_endChild(parent)); 
				insertIt++;
			}
			else
			{
				insertIt = std::ranges::find(m_pList->begin(), m_pList->end(), m_afterObject);
			}
			m_pList->insert(insertIt, m_objects.begin(), m_objects.end());
		}

		if (m_parent.lock()) {
			auto& list = m_parent.lock()->WorkChilds();
			auto insertIt = std::find_if(list.begin(), list.end(), [&](const auto& _obj) {return _obj.lock() == m_afterObject; });
			list.insert(insertIt, m_object);
		}

		m_object->SetParent(m_parent);

		m_parent = postParent;
	}

	void MoveGameObject::push_child(std::shared_ptr<IGameObject> _object, std::list<std::shared_ptr<IGameObject>>& _result)
	{
		_result.push_back(_object);
		for (auto& child : _object->GetChilds())
		{
			push_child(child.lock(), _result);
		}
	}

	void MoveGameObject::erase_objects(std::list<std::shared_ptr<IGameObject>>* _pList, const std::list<std::shared_ptr<IGameObject>>& _eraseList)
	{
		for (auto& object : _eraseList)
		{
			auto it = std::ranges::find(*_pList, object);
			if (it != _pList->end())
			{
				_pList->erase(it);
			}
		}
	}

	std::shared_ptr<IGameObject> MoveGameObject::pick_endChild(std::shared_ptr<IGameObject> _object)
	{
		if (_object->GetChilds().size())
		{
			auto& child = _object->GetChilds().back();
			return pick_endChild(child.lock());
		}
		return _object;
	}
}