#pragma once
#include "Interface/IEditor.h"

class EDITOR_API IEditorWindow
{
public:
	IEditorWindow();
	virtual ~IEditorWindow();

	virtual void Update()final;

	virtual void SetOwner(IEditor* _editor) final;
	virtual void SetName(std::string _str)  final;
	
	virtual bool GetActive() const final;
	virtual void SetActive(const bool _active)  final;


	virtual void Init() {};
	virtual void Release() {};

	virtual void MenuItem() {};
protected:
	virtual IEditor* GetOwner()final;
	
	virtual void ConfigLoad()final;
	virtual void ConfigSave()final;

	virtual void BeginChild();
	virtual void UpdateContents() = 0;
	virtual void EndChild();

	virtual void ConfigLoadContents(nlohmann::json&) {}
	virtual void ConfigSaveContents(nlohmann::json&) {}

private:
	struct IEditorWindowImpl;
	IEditorWindowImpl* m_Impl = nullptr;
};
