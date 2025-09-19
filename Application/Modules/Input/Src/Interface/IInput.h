#pragma once
#ifdef INPUT_DLL_EXPORTS
#define  INPUT_API __declspec(dllexport)
#else
#define  INPUT_API __declspec(dllimport)
#endif

class IInputCollector;
class IInputButtonBase;
class IInputAxisBase;

class INPUT_API IInput :public JTN::Module::IModule
{
public:
	IInput() = default;
	virtual ~IInput()override = default;

	// 全ての有効な入力装置からのボタン入力状態を取得
	virtual short GetButtonState(std::string_view name) const = 0;

	virtual bool IsFree(std::string_view name) const = 0;
	virtual bool IsPress(std::string_view name) const = 0;
	virtual bool IsHold(std::string_view name) const = 0;
	virtual bool IsRelease(std::string_view name) const = 0;

	// 全ての有効な入力装置からの軸入力状態を取得
	virtual Math::Vector2 GetAxisState(std::string_view name) const = 0;

	// 入力装置の登録
	virtual void AddDevice(std::string_view name, IInputCollector* pDevice) = 0;
	virtual void AddDevice(std::string_view name, std::unique_ptr<IInputCollector>& pDevice) = 0;

	virtual const std::unique_ptr<IInputCollector>& GetDevice(std::string_view name) const = 0;
	virtual std::unique_ptr<IInputCollector>& WorkDevice(std::string_view name) = 0;
};

class INPUT_API IInputCollector
{
public:
	enum class ActiveState
	{
		Disable,	// 無効：完全に停止している状態
		Monitoring,	// 監視：デバイスの入力を更新、アプリに入力の影響はない
		Enable,		// 有効：アプリに入力の影響を与える
	};

	IInputCollector() = default;
	virtual ~IInputCollector() = default;

	virtual void Update() = 0;

	// 何かしらの入力を検知したか
	virtual bool IsSomethingInput() = 0;

	// 任意の入力状況の取得
	virtual short GetButtonState(std::string_view name) const = 0;
	virtual Math::Vector2 GetAxisState(std::string_view name) const = 0;

	// 入力デバイスの状態の取得と設定
	virtual ActiveState GetActiveState() const = 0;
	virtual void SetActiveState(ActiveState state) = 0;

	// アプリケーションボタンの追加：上書き
	virtual void AddButton(std::string_view name, IInputButtonBase* pButton) = 0;
	virtual void AddButton(std::string_view name, std::shared_ptr<IInputButtonBase> spButton) = 0;
	// 入力軸の追加：上書き
	virtual void AddAxis(std::string_view name, IInputAxisBase* pAxis) = 0;
	virtual void AddAxis(std::string_view name, std::shared_ptr<IInputAxisBase> spAxis) = 0;

	virtual const std::shared_ptr<IInputButtonBase> GetButton(std::string_view name) const = 0;

	virtual const std::shared_ptr<IInputAxisBase> GetAxis(std::string_view name) const = 0;

	virtual const std::unordered_map<std::string, std::shared_ptr<IInputButtonBase>>& GetButtons() = 0;

protected:
	virtual void Release() = 0;

};

class INPUT_API IInputButtonBase
{
public:
	enum State
	{
		Free,				// 入力が無い
		Press,				// 押された瞬間
		Hold = Press << 1,	// 押している間
		Release = Press << 2// 離された瞬間
	};

	IInputButtonBase() = default;
	virtual ~IInputButtonBase() = default;

	virtual void PreUpdate() = 0;

	virtual void Update() = 0;

	// 強制的に入力無しの状態にする
	virtual void NoInput() = 0;

	virtual short GetState() const = 0;

	virtual void GetCode(std::vector<int>& ret) const = 0;
};

class INPUT_API IInputAxisBase
{
public:
	IInputAxisBase() {}
	virtual ~IInputAxisBase() {}

	virtual void PreUpdate() = 0;

	virtual void Update() = 0;

	// 強制的に入力無しの状態にする
	virtual void NoInput() = 0;

	virtual void SetValueRate(float rate) = 0;
	virtual void SetLimitValue(float limit) = 0;

	virtual Math::Vector2 GetState() const = 0;
};