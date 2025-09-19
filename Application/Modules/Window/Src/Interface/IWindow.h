#pragma once
// DLL エクスポート・インポートマクロ
#ifdef WINDOW_DLL_EXPORTS
#define WINDOW_API __declspec(dllexport)
#else
#define WINDOW_API __declspec(dllimport)
#endif

class IWindow_Event;

class WINDOW_API IWindow : public JTN::Module::IModule
{
public:
	IWindow() = default;
	virtual ~IWindow() = default;

	//===================================
	// 取得・設定
	//===================================

	// ウィンドウハンドル取得
	virtual HWND GetWndHandle() const = 0;
	// ウィンドウが存在する？
	virtual bool IsCreated() const = 0;
	// マウスホイールの変化量を取得
	virtual const int GetMouseWheelVal() const = 0;
	// インスタンスハンドル取得
	virtual HINSTANCE GetInstanceHandle() const = 0;

	// クライアントサイズの取得
	virtual const std::array<int, 2>& GetWinodwSize() = 0;
	// クライアントサイズの設定
	virtual void SetClientSize(int w, int h) = 0;

	virtual void EnableCursor(bool _isShow) = 0;

	// ファイルを開くダイアログボックスを表示
	// ・filepath		… 選択されたファイルパスが入る
	// ・title			… ウィンドウのタイトル文字
	// ・filters		… 指定された拡張子のみ表示されるようになる
	virtual bool OpenFileDialog(std::string& filepath, std::string_view title = "ファイルを開く", const char* filters = "全てのファイル\0*.*\0") = 0;

	// ファイル名をつけて保存ダイアログボックスを表示
	// ・filepath		… 選択されたファイルパスが入る
	// ・title			… ウィンドウのタイトル文字
	// ・filters		… 指定された拡張子のみ表示されるようになる
	// ・defExt			… ユーザーが拡張子を入力しなかった場合、これが自動的に付く
	virtual bool SaveFileDialog(std::string& filepath, std::string_view title = "ファイルを保存", const char* filters = "全てのファイル\0*.*\0", std::string_view defExt = "") = 0;

	//===================================
	// 機能拡張
	//===================================
	virtual void AddWindowProc(IWindow_Event* _pIWindow_Event) = 0;
	virtual void RemoveWindowProc(IWindow_Event* _pIWindow_Event) = 0;
};