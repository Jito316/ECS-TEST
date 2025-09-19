#pragma once

template<class T>
class StateMachine;

template<class T>
class StateBase
{
public:
	StateBase() = default;
	virtual ~StateBase() = default;

	virtual void PreUpdate() {}
	virtual void Update() {}
	virtual void PostUpdate() {}
	virtual void RenderUpdate() {}

	virtual void Init() {}
	virtual void Release() {}
	virtual void Enter() {}
	virtual void Exit() {}

	virtual void ImGuiUpdate() {}

	virtual void Serialize(nlohmann::json&) {}
	virtual void Deserialize(const nlohmann::json&) {}

protected:
	friend StateMachine<T>;
	StateMachine<T>* m_machine = nullptr;
	T* m_owner = nullptr;

};

template<class T>
class StateMachine
{
public:
	typedef std::list<std::pair<std::string, std::shared_ptr<StateBase<T>>>> StateList;

	StateMachine() = default;
	virtual ~StateMachine() = default;

	virtual bool ShihfState(std::string_view _name)
	{
		if (m_nextState)return false;

		auto it = std::ranges::find_if(m_stateList, [&](auto& it) { return it.first == _name.data(); });
		if (it == m_stateList.end() || m_nextState == it->second)return false;

		ShihfState(it->second);
		return true;
	}
	virtual void ShihfState(std::shared_ptr<StateBase<T>> _nextState)
	{
		if (m_nowState != _nextState)
		{
			m_nextState = _nextState;
		}
	}

	virtual void Register(std::string_view _name, std::shared_ptr<StateBase<T>> _instance)	final
	{
		_instance->m_machine = this;
		_instance->m_owner = m_owner;
		m_stateList.push_back({ _name.data(), _instance });
	}

private:
	friend T;
	virtual void Init()
	{
		for (auto& [name, state] : m_stateList)
		{
			state->Init();
		}
	}
	virtual void Release()
	{
		for (auto& [name, state] : m_stateList)
		{
			state->Release();
		}
	}

	virtual void PreUpdate()
	{
		if (m_nextState) {
			if (m_nowState)m_nowState->Exit();
			m_nextState->Enter();

			m_nowState = m_nextState;
			m_nextState = nullptr;
		}
		if (m_nowState)m_nowState->PreUpdate();
	}
	virtual void Update()
	{
		if (m_nowState)
		{
			m_nowState->Update();
		}
	}
	virtual void PostUpdate()
	{
		if (m_nowState)
		{
			m_nowState->PostUpdate();
		}
	}

	virtual void ImGuiUpdate()
	{
		std::string nowStateName = "empty";
		for (auto& [name, state] : m_stateList)
		{
			if (m_nowState == state)
			{
				nowStateName = name;
				break;
			}
		}

		EDITOR_COMMAND("InputText", "nowState", &nowStateName);
	}

	virtual void Serialize(std::string_view _key, nlohmann::json& _data)
	{
		for (auto& [name, state] : m_stateList)
		{
			state->Serialize(_data[_key.data()][name.data()]);
		}
	}
	virtual void Deserialize(std::string_view _key, const nlohmann::json& _data)
	{
		if (!_data.contains(_key.data()))return;
		auto& statesData = _data[_key.data()];
		for (auto& [key, value] : m_stateList)
		{
			if (statesData.contains(key.data()))
			{
				value->Deserialize(statesData[key.data()]);
			}
		}

	}

	std::shared_ptr<StateBase<T>> m_nowState = nullptr;
	std::shared_ptr<StateBase<T>> m_nextState = nullptr;

	StateList m_stateList;

	T* m_owner = nullptr;
};