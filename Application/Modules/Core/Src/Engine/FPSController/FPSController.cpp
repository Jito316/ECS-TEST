#include "FPSController.h"
namespace JTN
{
	// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
	// FPSの制御コントローラー
	// ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### ##### #####
	void FPSController::Init()
	{
		m_fpsMonitorBeginTime = timeGetTime();
	}

	void FPSController::UpdateStartTime()
	{
		m_frameStartTime = timeGetTime();
	}

	void FPSController::Update()
	{
		Control();

		Monitoring();
	}

	// FPS制御
	void FPSController::Control()
	{
		// 処理終了時間Get
		DWORD frameProcessEndTime = timeGetTime();

		// 1フレームで経過すべき時間
		DWORD timePerFrame = Second / m_maxFps;

		m_deltaTime = frameProcessEndTime - m_frameStartTime;

		if (m_deltaTime < timePerFrame)
		{
			// 1秒間にMaxFPS回数以上処理が回らないように待機する
			Sleep(timePerFrame - m_deltaTime);
			m_deltaTime = timePerFrame;
		}
	}

	// 現在のFPS計測
	void FPSController::Monitoring()
	{
		// FPS計測のタイミング　0.5秒おき
		constexpr float FpsRefreshFrame = 500;

		m_fpsCnt++;

		// 0.5秒おきに FPS計測
		if (m_frameStartTime - m_fpsMonitorBeginTime >= FpsRefreshFrame)
		{
			// 現在のFPS算出
			m_nowFps = (m_fpsCnt * Second) / (m_frameStartTime - m_fpsMonitorBeginTime);

			m_fpsMonitorBeginTime = m_frameStartTime;

			m_fpsCnt = 0;
		}
	}

	const int FPSController::GetNowFPS() const
	{
		return m_nowFps;
	}

	const int FPSController::GetMaxFPS() const
	{
		return m_maxFps;
	}

	const float FPSController::GetDeltaTime() const
	{
		return m_deltaTime / (float)Second;
	}

	void FPSController::SetGetMaxFPS(const int _maxFps)
	{
		m_maxFps = _maxFps;
	}

}