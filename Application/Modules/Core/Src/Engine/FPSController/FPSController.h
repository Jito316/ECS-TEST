#pragma once
#include "Interface/IFPSController.h"

namespace JTN
{
	class FPSController : public IFPSController
	{
	public:
		void Init();

		void UpdateStartTime();

		void Update();

		const int GetNowFPS() const override;
		const int GetMaxFPS() const override;
		const float GetDeltaTime() const override;
		void SetGetMaxFPS(const int _maxFps) override;
	private:
		// FPS制御
		int		m_nowFps = 0;		// 現在のFPS値
		int		m_maxFps = 60;		// 最大FPS
		float	m_deltaTine = 0.f;		// 最大FPS

		void Control();

		void Monitoring();

		DWORD		m_frameStartTime = 0;		// フレームの開始時間

		int			m_fpsCnt = 0;				// FPS計測用カウント
		DWORD		m_fpsMonitorBeginTime = 0;	// FPS計測開始時間

		const int	Second = 1000;				// １秒のミリ秒

		DWORD m_deltaTime = 0;
	};

}