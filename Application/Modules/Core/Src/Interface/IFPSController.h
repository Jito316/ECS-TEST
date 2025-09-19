#pragma once
#include "Core.h"

namespace JTN
{
	//============================================================
	// アプリケーションのFPS制御 + 測定
	//============================================================
	class CORE_API IFPSController
	{
	public:
		virtual const int GetNowFPS() const  = 0;
		virtual const int GetMaxFPS() const  = 0;
		virtual const float GetDeltaTime() const  = 0;
		virtual void SetGetMaxFPS(const int _maxFps) = 0;
	};

}