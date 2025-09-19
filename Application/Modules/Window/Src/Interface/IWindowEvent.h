#pragma once
#include "IWindow.h"

/// <summary>
/// 圧倒的、環境依存インターフェイス
/// ウィンドウが受け取ったメッセージに対する処理を拡張出来る
/// </summary>
class WINDOW_API IWindow_Event 
{
public:
	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};