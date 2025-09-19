#pragma once
#include "Interface/IWindow.h"
//=====================================================
//
// ウィンドウ
//
//=====================================================
class Window :public IWindow {
public:
	//===================================
	// 処理
	//===================================

	// ウィンドウメッセージを処理する
	//  戻り値：終了メッセージが来たらfalseが返る
	bool ProcessMessage();

	//===================================
	// 初期化・解放
	//===================================

	// ウィンドウ作成
	bool Create(int clientWidth, int clientHeight, std::string_view titleName, std::string_view windowClassName);


	//===================================
	// 取得・設定
	//===================================

	// ウィンドウハンドル取得
	HWND GetWndHandle() const override { return m_hWnd; }
	// ウィンドウが存在する？
	bool IsCreated() const override { return m_hWnd ? true : false; }
	// マウスホイールの変化量を取得
	const int GetMouseWheelVal() const override { return m_mouseWheelVal; }
	// インスタンスハンドル取得
	HINSTANCE GetInstanceHandle() const override { return GetModuleHandle(0); }

	// クライアントサイズの取得
	const std::array<int, 2>& GetWinodwSize()override;
	// クライアントサイズの設定
	void SetClientSize(int w, int h)override;

	void EnableCursor(bool _isShow)override;

	//===================================
	// 便利関数
	//===================================

	// ファイルを開くダイアログボックスを表示
	// ・filepath		… 選択されたファイルパスが入る
	// ・title			… ウィンドウのタイトル文字
	// ・filters		… 指定された拡張子のみ表示されるようになる
	bool OpenFileDialog(std::string& filepath, std::string_view title = "ファイルを開く", const char* filters = "全てのファイル\0*.*\0")override;

	// ファイル名をつけて保存ダイアログボックスを表示
	// ・filepath		… 選択されたファイルパスが入る
	// ・title			… ウィンドウのタイトル文字
	// ・filters		… 指定された拡張子のみ表示されるようになる
	// ・defExt			… ユーザーが拡張子を入力しなかった場合、これが自動的に付く
	bool SaveFileDialog(std::string& filepath, std::string_view title = "ファイルを保存", const char* filters = "全てのファイル\0*.*\0", std::string_view defExt = "")override;

	//===================================
	// 機能拡張
	//===================================
	void AddWindowProc(IWindow_Event* _pIWindow_Event)override;
	void RemoveWindowProc(IWindow_Event* _pIWindow_Event)override;

private:
	bool Entry()override;

	void SetUp();
	void Update();
	void Shutdown();

	bool m_isShow = true;

	// ウィンドウハンドル
	HWND	m_hWnd = nullptr;

	// ウィンドウサイズ
	std::array<int, 2> m_windowSize = {};

	// マウスホイール値
	int		m_mouseWheelVal = 0;

	// ウィンドウ関数
	static LRESULT CALLBACK callWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	std::string_view m_windowClassName;

	std::list<IWindow_Event*> m_pWindowEvents;
};

extern "C" WINDOW_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern "C" WINDOW_API void DeleteModule(JTN::Module::IModule* _module);