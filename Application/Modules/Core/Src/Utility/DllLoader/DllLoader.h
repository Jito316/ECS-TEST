#pragma once
#include "Core.h"

class CORE_API DllLoader
{
public:
	bool Load(const char* dllPath);
	void Release();

	void* GetFunction(const char* dllPath);
private:
	struct hModuleImple;
	hModuleImple* m_phModuleImple = nullptr;
};
