#pragma once
#include "IWindow.h"

/// <summary>
/// ���|�I�A���ˑ��C���^�[�t�F�C�X
/// �E�B���h�E���󂯎�������b�Z�[�W�ɑ΂��鏈�����g���o����
/// </summary>
class WINDOW_API IWindow_Event 
{
public:
	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
};