#pragma once

class Editor_StyleWindow
	:public IEditorWindow
{
public:
	void UpdateContents()override;
	void Init()override;
	void Release()override;
};
