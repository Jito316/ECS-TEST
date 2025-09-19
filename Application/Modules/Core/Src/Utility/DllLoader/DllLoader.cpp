#include "Utility/DllLoader/DllLoader.h"
#include <iostream>
#include <libloaderapi.h>

struct DllLoader::hModuleImple
{
	HMODULE hModule = nullptr;
};

bool DllLoader::Load(const char* dllPath)
{
	m_phModuleImple = new hModuleImple;

	m_phModuleImple->hModule = LoadLibraryA(dllPath);
	if (!m_phModuleImple->hModule) {
		std::cerr << "Failed to load DLL: " << dllPath << std::endl;
		return false;
	}
	return true;
}

void DllLoader::Release()
{
	FreeLibrary(m_phModuleImple->hModule);
	delete m_phModuleImple;
}

void* DllLoader::GetFunction(const char* dllPath)
{
	return GetProcAddress(m_phModuleImple->hModule, dllPath);
}