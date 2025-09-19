#ifdef _DEBUG	
#define DllPath(NAME)  std::string("Modules/") + #NAME  + "/Win32/Debug/" + #NAME + ".dll"
#else			
#define DllPath(NAME) std::string("Modules/") + #NAME  + "/Win32/Release/" + #NAME + ".dll"
#endif		

typedef void (*AppFnc)();

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int)
{

	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	HMODULE hModule;
	hModule = LoadLibraryA((DllPath(Core)).data());
	if (hModule) {
		AppFnc Execute = (AppFnc)GetProcAddress(hModule, "Execute");

		Execute();

		FreeLibrary(hModule);
	}

	// COM解放
	CoUninitialize();
	return 0;
}