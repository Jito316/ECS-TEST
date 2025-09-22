#pragma once

namespace Command {
	class SelectGameObject : public JTN::Message::ITask {
	public:
		bool Task(void** value = nullptr);
	private:
		struct Coll {
			std::unique_ptr<Math::KdCollider> coll;
			Entity object;
		};
		struct Result {
			Math::KdCollider::CollisionResult result;
			Entity object;
		};

		bool CreateColls();
		bool CreateResults();
		bool PickUpObject(Entity& _object);
		
		std::list<Coll>				m_colls;
		Math::KdCollider::RayInfo*  m_pInfo = nullptr;
		std::list<Result>			m_results;
	};
}
