#pragma once
#ifdef SCRIPT_DLL_EXPORTS
#define  SCRIPT_API __declspec(dllexport)
#else
#define  SCRIPT_API __declspec(dllimport)
#endif

enum RuntimeState 
{
	Play = 1,
	Stop,
	Edit
};

// シーンの管理と各マネージャーの提供
class SCRIPT_API IScript :public JTN::Module::IModule
{
public:
	virtual void ImGuiUpdate() = 0;

	virtual void LoadScene(const std::string& _name,const bool _bSave = false) = 0;

	virtual const std::string& GetNowSceneName() const = 0;
	virtual std::string& WorkNowSceneName() = 0;

	virtual const std::map<std::string, std::string>& GetSceneList() = 0;
	virtual std::map<std::string, std::string>& WorkSceneList() = 0;

	virtual const std::string& GetStartName() const = 0;
	virtual std::string& WorkStartName() = 0;

	virtual bool SetState(const RuntimeState _state) = 0;
	virtual bool isPlay() = 0;
	virtual bool isStop() = 0;
	virtual bool isEdit() = 0;
};
