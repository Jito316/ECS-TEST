#include "Log.h"

namespace JTN {
	namespace Log {
		void Data::operator()(const State& _sate)
		{
			m_state = _sate;
		}

		const std::string& Data::GetTime() const
		{
			return m_time;
		}

		const std::string& Data::GetLog() const
		{
			return m_message;
		}

		const std::string& Data::GetFile() const
		{
			return m_file;
		}

		const State& Data::GetState() const
		{
			return m_state;
		}

		void System::PushState(State state)
		{
			m_stateStack.push_back(state);
		}
		void System::PopState()
		{
			if (!m_stateStack.empty()) {
				m_stateStack.pop_back();
			}
		}


		IData& System::AddLog(std::string_view _file, const char* _format, ...)
		{
			// 現在時刻を取得
			std::time_t now = std::time(nullptr);
			std::tm localTime; localtime_s(&localTime, &now);

			// 文字列ストリームを使用してフォーマット
			std::stringstream ss;
			ss << std::put_time(&localTime, "%H:%M:%S");
			std::string time = ss.str();

			char buffer[1024];
			va_list args;
			va_start(args, _format);
			vsnprintf(buffer, sizeof(buffer), _format, args);
			va_end(args);

			auto log = new Data;
			log->m_file = _file;
			log->m_message = buffer;
			log->m_state = m_stateStack.empty() ? State::Default : m_stateStack.back();
			log->m_time = time;
			m_logs.push_back(log);

			// 最大ログ数の管理
			while (m_logs.size() > m_maxLogCount) {
				delete m_logs.front();
				m_logs.pop_front();
			}

			return *log;
		}

		const std::list<IData*>& System::GetLogs() const
		{
			return m_logs;
		}

		std::list<IData*>& System::WorkLogs()
		{
			return m_logs;
		}



		void System::ClearLogs()
		{
			while (m_logs.size())
			{
				delete m_logs.front();
				m_logs.erase(m_logs.begin());
			}
			m_stateStack.clear();
		}

		void System::SetMaxLogCount(size_t count)
		{
			m_maxLogCount = count;
		}
	};
};