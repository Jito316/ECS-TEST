#pragma once

namespace Command {
	class SelectGameObject : public JTN::Message::ITask {
	public:
		bool Task(void** value = nullptr);
	private:
		struct Coll {
			std::unique_ptr<Math::KdCollider> coll;
			std::weak_ptr<IGameObject> object;
		};
		struct Result {
			Math::KdCollider::CollisionResult result;
			std::weak_ptr<IGameObject> object;
		};

		bool CreateColls();
		bool CreateResults();
		bool PickUpObject(std::weak_ptr<IGameObject>& _object);
		
		std::list<Coll>				m_colls;
		Math::KdCollider::RayInfo*  m_pInfo = nullptr;
		std::list<Result>			m_results;
	};
}
