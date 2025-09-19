#pragma once
#include "Interface/ILog.h"

namespace JTN {
	namespace Log {
		class Data : public IData
		{
		public:
			// コンストラクタ
			Data() = default;
			~Data()override = default;

			// ログ状態の指定
			void operator()(const State& _sate) override;

			const std::string& GetTime()const override;
			const std::string& GetLog()const override;
			const std::string& GetFile()const override;
			const State& GetState()const override;

			std::string m_time;				// 時間
			std::string m_message;			// ログメッセージ
			std::string m_file;				// ファイルパス
			State m_state = State::Default;	// ログの状態
		};

		//==================
		// ログ管理クラス
		//==================
		class System : public ISystem
		{
		public:
			// 状態管理
			void PushState(State state) override;
			void PopState() override;

			// 可変引数マクロを使うバージョン
			IData& AddLog(std::string_view _file, const char* _format, ...) override;

			// ログ取得
			const std::list<IData*>& GetLogs() const override;
			std::list<IData*>& WorkLogs()  override;

			// ログクリア
			void ClearLogs() override;

			// 設定
			void SetMaxLogCount(size_t count) override;

			// コンストラクタ
			System() = default;
			~System() override 
			{
				ClearLogs();
			}
		private:
			std::list<IData*> m_logs;              // ログリスト
			std::vector<State> m_stateStack;     // 状態スタック
			size_t m_maxLogCount = 1000;         // 最大ログ保持数
		};
	}
}