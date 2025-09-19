#pragma once
#ifdef AUDIO_DLL_EXPORTS
#define  AUDIO_API __declspec(dllexport)
#else
#define  AUDIO_API __declspec(dllimport)
#endif

class ISoundInstance;
class ISoundInstance3D;

class AUDIO_API IAudio :public JTN::Module::IModule
{
public:
	IAudio() = default;
	virtual ~IAudio()override = default;

	virtual void SetListnerMatrix(const Math::Matrix& mWorld) = 0;

	// サウンド再生
	virtual std::shared_ptr<ISoundInstance3D> Play3D(std::string_view rName, const Math::Vector3& rPos, bool loop = false) = 0;
	virtual std::shared_ptr<ISoundInstance>   Play(std::string_view rName, bool loop = false) = 0;

	virtual void AddPlayList(const std::shared_ptr<ISoundInstance>& rSound) = 0;

	// 再生中の音をすべて停止する
	virtual void StopAllSound() = 0;
	// 再生中の音をすべて一時停止する
	virtual void PauseAllSound() = 0;
	// 再生中の音をすべて停止する
	virtual void ResumeAllSound() = 0;

	// 再生中の音をすべて停止・サウンドアセットの解放を行う
	virtual void SoundReset() = 0;

	// サウンドアセットの一括読込
	virtual void LoadSoundAssets(std::initializer_list<std::string_view>& fileName) = 0;
};

class AUDIO_API ISoundInstance
{
public:
	ISoundInstance() = default;
	virtual ~ISoundInstance() = default;


	virtual bool CreateInstance() = 0;

	virtual void Play(bool loop = false) = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	// ・vol	… ボリューム設定(1.0が100%)
	virtual void SetVolume(float vol) = 0;

	// ・pitch	… 振動設定(低音-1.0～1.0高音)
	virtual void SetPitch(float pitch) = 0;

	// 再生状態の取得
	virtual bool IsPlaying() = 0;
	virtual bool IsPause() = 0;
	virtual bool IsStopped() = 0;
};

class AUDIO_API ISoundInstance3D : public ISoundInstance
{
public:
	ISoundInstance3D() = default;
	virtual ~ISoundInstance3D() = default;

	virtual bool CreateInstance()override = 0;

	virtual void Play(bool loop = false)override = 0;

	virtual void SetPos(const Math::Vector3& rPos) = 0;

	// 減衰倍率設定 1:通常 FLT_MIN～FLT_MAX
	virtual void SetCurveDistanceScaler(float val) = 0;
};
