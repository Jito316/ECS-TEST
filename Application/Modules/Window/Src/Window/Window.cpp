#include "Window.h"
#include <strconv.h>
#include <filesystem>

bool Window::Create(int clientWidth, int clientHeight, std::string_view titleName, std::string_view windowClassName)
{
	// m_nowScreenSize = { (uint32_t)clientWidth ,(uint32_t)clientHeight };

	// インスタンスハンドル取得
	HINSTANCE hInst = GetInstanceHandle();

	//===================================================================
	// メインウィンドウ作成
	//===================================================================
	m_windowClassName = windowClassName;
	std::wstring wndClsName = sjis_to_wide(windowClassName.data() + std::to_string((int)&windowClassName));

	//ウィンドウクラスの定義
	WNDCLASSEX wc;											// ウィンドウクラスの定義用
	wc.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
	wc.style = 0;											// スタイル
	wc.lpfnWndProc = &Window::callWindowProc;				// ウインドウ関数
	wc.cbClsExtra = 0;										// エキストラクラス情報 
	wc.cbWndExtra = 0;										// エキストラウィンドウ情報
	wc.hInstance = hInst;									// インスタンスハンドル
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);			// ラージアイコン
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);		// スモールアイコン 
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);			// マウスカーソル
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 背景色 
	wc.lpszMenuName = nullptr;								// メインメニュー名
	wc.lpszClassName = (LPCSTR)wndClsName.data();					// ウィンドウクラス名

	//ウィンドウクラスの登録
	if (!RegisterClassEx(&wc)) {
		return false;
	}

	//ウィンドウの作成
	m_hWnd = CreateWindow(
		(LPCSTR)wndClsName.c_str(),							// ウィンドウクラス名
		(LPCSTR)sjis_to_wide(titleName.data()).c_str(),		// ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW,								// ウィンドウタイプを標準タイプに	
		0,													// ウィンドウの位置（Ｘ座標）
		0,													// ウィンドウの位置（Ｙ座標）						
		clientWidth,										// ウィンドウの幅
		clientHeight,										// ウィンドウの高さ			
		nullptr,											// 親ウィンドウのハンドル
		nullptr,											// メニューのハンドル
		hInst,												// インスタンスハンドル 
		this);												// 追加情報

	if (m_hWnd == nullptr) {
		return false;
	}

	// クライアントのサイズを設定
	SetClientSize(clientWidth, clientHeight);

	//===================================================================
	//ウィンドウの表示
	//===================================================================
	ShowWindow(m_hWnd, SW_SHOW);
	//ウィンドウの更新
	UpdateWindow(m_hWnd);

	// timeGetTime関数の精度を1msに設定
	timeBeginPeriod(1);

	return true;
}


bool Window::ProcessMessage()
{
	m_mouseWheelVal = 0;
	POINT mouse;
	GetCursorPos(&mouse);
	//ディスプレイの左上が(0,0)
	ScreenToClient(m_hWnd, &mouse);

	// メッセージ取得
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// 終了メッセージがきた
		if (msg.message == WM_QUIT) {
			JTN::GetEngine()->End();
			return false;
		}

		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

// ウィンドウ関数(Static関数)
LRESULT CALLBACK Window::callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ウィンドウプロパティから、GameWindowクラスのインスタンスを取得
	Window* pThis = (Window*)GetProp(hWnd, (LPCSTR)L"GameWindowInstance");

	// nullptrの場合は、デフォルト処理を実行
	if (pThis == nullptr) {
		switch (message) {
		case WM_CREATE:
		{
			// CreateWindow()で渡したパラメータを取得
			CREATESTRUCT* createStruct = (CREATESTRUCT*)lParam;
			Window* window = (Window*)createStruct->lpCreateParams;

			// ウィンドウプロパティにこのクラスのインスタンスアドレスを埋め込んでおく
			// 次回から、pThis->WindowProcの方へ処理が流れていく
			SetProp(hWnd, (LPCSTR)L"GameWindowInstance", window);
		}
		return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	// インスタンス側のWindow関数を実行する
	return pThis->WindowProc(hWnd, message, wParam, lParam);
}

// ウィンドウ関数
#include "Interface/IWindowEvent.h"
LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for (auto& it : m_pWindowEvents)
	{
		it->WindowProc(hWnd, message, wParam, lParam);
	}

	//===================================
	//メッセージによって処理を選択
	//===================================
	switch (message) {
		// ホイールスクロール時
	case WM_MOUSEWHEEL:
	{
		m_mouseWheelVal = (short)HIWORD(wParam);
	}
	break;
	// Xボタンが押された
	case WM_CLOSE:
		// 破棄
		JTN::GetEngine()->End();
		break;
		// ウィンドウ破棄直前
	case WM_DESTROY:
		RemoveProp(hWnd, (LPCSTR)L"GameWindowInstance");
		PostQuitMessage(0);
		break;
		/*
	case WM_SIZE:
		if (Kernel::GetEngine()->m_kdDirect3D.GetDevContext() != nullptr && wParam != SIZE_MINIMIZED)
		{
			Kernel::GetEngine()->m_kdDirect3D.WindowResize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		}
		break;
		*/
	default:
		// メッセージのデフォルト処理
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

const std::array<int, 2>& Window::GetWinodwSize()
{
	return m_windowSize;
}

// ウィンドウのクライアントのサイズを指定サイズにする。
void Window::SetClientSize(int w, int h)
{
	// m_nowScreenSize = { (uint32_t)w ,(uint32_t)h };

	RECT rcWnd, rcCli;

	GetWindowRect(m_hWnd, &rcWnd); // ウィンドウのRECT取得
	GetClientRect(m_hWnd, &rcCli); // クライアント領域のRECT取得

	// ウィンドウの余白を考えて、クライアントのサイズを指定サイズにする。
	MoveWindow(m_hWnd,
		rcWnd.left,// X座標
		rcWnd.top,// Y座標
		w + (rcWnd.right - rcWnd.left) - (rcCli.right - rcCli.left),
		h + (rcWnd.bottom - rcWnd.top) - (rcCli.bottom - rcCli.top),
		TRUE);
}

void Window::EnableCursor(bool _isShow)
{
	if (m_isShow != _isShow) {
		ShowCursor(_isShow);
		m_isShow = _isShow;
	}
}

bool Window::OpenFileDialog(std::string& filepath, std::string_view title, const char* filters)
{
	// 現在のカレントディレクトリ保存
	auto current = std::filesystem::current_path();
	// ファイル名のみ
	auto filename = std::filesystem::path(filepath).filename();

	// 結果用
	static char fname[1000];
	strcpy_s(fname, sizeof(fname), filename.string().c_str());

	// デフォルトフォルダ
	std::string dir;
	if (filepath.size() == 0)
	{
		dir = current.string() + "\\";
	}
	else {
		auto path = std::filesystem::absolute(filepath);
		dir = path.parent_path().string() + "\\";
	}

	OPENFILENAMEA o;
	ZeroMemory(&o, sizeof(o));

	o.lStructSize = sizeof(o);									// 構造体サイズ
	o.hwndOwner = nullptr;										// 親ウィンドウのハンドル
	o.lpstrInitialDir = dir.c_str();							// 初期フォルダー
	o.lpstrFile = fname;										// 取得したファイル名を保存するバッファ
	o.nMaxFile = sizeof(fname);									// 取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = filters;									// (例) "TXTファイル(*.TXT)\0*.TXT\0全てのファイル(*.*)\0*.*\0";
	o.lpstrDefExt = "";
	o.lpstrTitle = title.data();
	o.nFilterIndex = 1;
	if (GetOpenFileNameA(&o))
	{
		// カレントディレクトリを元に戻す
		std::filesystem::current_path(current);
		// 相対パスへ変換
		filepath = std::filesystem::relative(fname).string();
		return true;
	}
	std::filesystem::current_path(current);	// カレントディレクトリを元に戻す
	return false;
}

bool Window::SaveFileDialog(std::string& filepath, std::string_view title, const char* filters, std::string_view defExt)
{
	// 現在のカレントディレクトリ保存
	auto current = std::filesystem::current_path();
	// ファイル名のみ
	auto filename = std::filesystem::path(filepath).filename();

	// 結果用
	static char fname[1000];
	strcpy_s(fname, sizeof(fname), filename.string().c_str());

	// デフォルトフォルダ
	std::string dir;
	if (filepath.size() == 0)
	{
		dir = current.string() + "\\";
	}
	else {
		auto path = std::filesystem::absolute(filepath);
		dir = path.parent_path().string() + "\\";
	}

	OPENFILENAMEA o;
	ZeroMemory(&o, sizeof(o));

	o.lStructSize = sizeof(o);								// 構造体サイズ
	o.hwndOwner = nullptr;									// 親ウィンドウのハンドル
	o.lpstrInitialDir = dir.c_str();						// 初期フォルダー
	o.lpstrFile = fname;									// 取得したファイル名を保存するバッファ
	o.nMaxFile = sizeof(fname);								// 取得したファイル名を保存するバッファサイズ
	o.lpstrFilter = filters;								// (例) "TXTファイル(*.TXT)\0*.TXT\0全てのファイル(*.*)\0*.*\0";
	o.lpstrDefExt = defExt.data();
	o.lpstrTitle = title.data();
	o.nFilterIndex = 1;
	o.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	if (GetSaveFileNameA(&o))
	{
		// カレントディレクトリを元に戻す
		std::filesystem::current_path(current);
		// 相対パスへ変換
		filepath = std::filesystem::relative(fname).string();
		return true;
	}
	std::filesystem::current_path(current);	// カレントディレクトリを元に戻す
	return false;
}

void Window::AddWindowProc(IWindow_Event* _pIWindow_Event)
{
	auto it = std::find(m_pWindowEvents.begin(), m_pWindowEvents.end(), _pIWindow_Event);
	if (it == m_pWindowEvents.end())
	{
		m_pWindowEvents.push_back(_pIWindow_Event);
	}
}

void Window::RemoveWindowProc(IWindow_Event* _pIWindow_Event)
{
	auto it = std::find(m_pWindowEvents.begin(), m_pWindowEvents.end(), _pIWindow_Event);
	if (it != m_pWindowEvents.end())
	{
		m_pWindowEvents.erase(it);
	}
}

bool Window::Entry()
{
	ModuleEntry(ModuleEntry_InitializeWindow, &Window::SetUp);
	ModuleEntry(ModuleEntry_UpdateWindow, &Window::Update);
	ModuleEntry(ModuleEntry_FinalizeWindow, &Window::Shutdown);
	return true;
}

#include "Utility/Config/Config.h"
void Window::SetUp()
{
	JTN::File::Config config;
	nlohmann::json data;
	if (config.Load("Windows_Window", data)) {
		m_windowSize[0] = data["w"];
		m_windowSize[1] = data["h"];
	}
	else {
		m_windowSize[0] = 1280;
		m_windowSize[1] = 720;
	}

	Create(m_windowSize[0], m_windowSize[1], "JTNEngine", "window");
}

void Window::Update()
{
	std::string titlebar = "JTN Engine fps : " + std::to_string(JTN::GetEngine()->GetFPSController()->GetNowFPS());
	SetWindowTextA(GetWndHandle(), titlebar.c_str());
	if (!ProcessMessage()) 
	{
		JTN::GetEngine()->End();
	}
}

void Window::Shutdown()
{
	if (m_hWnd) {
		std::wstring wndClsName = sjis_to_wide(m_windowClassName.data());
		UnregisterClass((LPCSTR)wndClsName.c_str(), GetInstanceHandle());
		DestroyWindow(m_hWnd);
		m_hWnd = nullptr;
	}
}

extern "C" WINDOW_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress)
{
	*_pModuleAdress =  new Window;
	*_pInterfaceInfoAdress = &typeid(IWindow);
};

extern "C" WINDOW_API void DeleteModule(JTN::Module::IModule* _module)
{
	delete _module;
}
