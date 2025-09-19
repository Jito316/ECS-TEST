#pragma once

class BaseEditorTask :public JTN::Message::ITask {
public:
	virtual bool Task(void** values) final;
protected:
	virtual bool TaskContents(void** values) = 0;
};