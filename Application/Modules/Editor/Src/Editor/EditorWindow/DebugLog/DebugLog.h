#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class DebugLog
	:public IEditorWindow
{
public:
	void Init()override;
	void UpdateContents()override;
	void FilterUpdate();
	const bool BeginLogs()const;
	void Logs();
	void EndLogs();

	const ImVec4 GetStateColor(const JTN::Log::State& _state = JTN::Log::State::Default);
private:
	int CountActiveElement(UINT& _element);

	enum Element : UINT
	{
		Time = 1 << 0,
		Text = 1 << 1,
		File = 1 << 2
	};
	struct
	{
		std::string name;
		UINT state = JTN::Log::State::Default | JTN::Log::State::Warning | JTN::Log::State::Error;
		UINT element = Element::File | Element::Text;
		int elementNum = 0;
	}m_filter;

	struct
	{
		bool active = true;
		int postSize = 0;
	}m_autoScroll;
};
