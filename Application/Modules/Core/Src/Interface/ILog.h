#pragma once
#include "Core.h"

namespace JTN {
	namespace Log
	{
		//==================
		// ログステート
		//==================
		enum CORE_API State
		{
			Default = 1 << 0,
			Warning = 1 << 1,
			Error = 1 << 2
		};

		//==================
		// ログデータクラス
		//==================
		class CORE_API IData
		{
		public:
			// コンストラクタ
			IData() = default;
			virtual ~IData() = default;

			// ログ状態の指定
			virtual void operator()(const State& _sate) = 0;

			virtual const std::string& GetTime()const = 0;
			virtual const std::string& GetLog()const = 0;
			virtual const std::string& GetFile()const = 0;
			virtual const State& GetState()const = 0;
		};

		//==================
		// ログ管理クラス
		//==================
		class CORE_API ISystem
		{
		public:
			// 状態管理
			virtual void PushState(State state) = 0;
			virtual void PopState() = 0;

			// 可変引数マクロを使うバージョン
			virtual IData& AddLog(std::string_view _file, const char* _format, ...) = 0;

			// ログ取得
			virtual const std::list<IData*>& GetLogs() const = 0;
			virtual std::list<IData*>& WorkLogs() = 0;

			// ログクリア
			virtual void ClearLogs() = 0;

			// 設定
			virtual void SetMaxLogCount(size_t count) = 0;

			// コンストラクタ
			ISystem() = default;
			virtual ~ISystem() = default;
		};
	}
}