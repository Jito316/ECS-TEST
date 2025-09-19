#pragma once
// DLL �G�N�X�|�[�g�E�C���|�[�g�}�N��
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
	// �擾�E�ݒ�
	//===================================

	// �E�B���h�E�n���h���擾
	virtual HWND GetWndHandle() const = 0;
	// �E�B���h�E�����݂���H
	virtual bool IsCreated() const = 0;
	// �}�E�X�z�C�[���̕ω��ʂ��擾
	virtual const int GetMouseWheelVal() const = 0;
	// �C���X�^���X�n���h���擾
	virtual HINSTANCE GetInstanceHandle() const = 0;

	// �N���C�A���g�T�C�Y�̎擾
	virtual const std::array<int, 2>& GetWinodwSize() = 0;
	// �N���C�A���g�T�C�Y�̐ݒ�
	virtual void SetClientSize(int w, int h) = 0;

	virtual void EnableCursor(bool _isShow) = 0;

	// �t�@�C�����J���_�C�A���O�{�b�N�X��\��
	// �Efilepath		�c �I�����ꂽ�t�@�C���p�X������
	// �Etitle			�c �E�B���h�E�̃^�C�g������
	// �Efilters		�c �w�肳�ꂽ�g���q�̂ݕ\�������悤�ɂȂ�
	virtual bool OpenFileDialog(std::string& filepath, std::string_view title = "�t�@�C�����J��", const char* filters = "�S�Ẵt�@�C��\0*.*\0") = 0;

	// �t�@�C���������ĕۑ��_�C�A���O�{�b�N�X��\��
	// �Efilepath		�c �I�����ꂽ�t�@�C���p�X������
	// �Etitle			�c �E�B���h�E�̃^�C�g������
	// �Efilters		�c �w�肳�ꂽ�g���q�̂ݕ\�������悤�ɂȂ�
	// �EdefExt			�c ���[�U�[���g���q����͂��Ȃ������ꍇ�A���ꂪ�����I�ɕt��
	virtual bool SaveFileDialog(std::string& filepath, std::string_view title = "�t�@�C����ۑ�", const char* filters = "�S�Ẵt�@�C��\0*.*\0", std::string_view defExt = "") = 0;

	//===================================
	// �@�\�g��
	//===================================
	virtual void AddWindowProc(IWindow_Event* _pIWindow_Event) = 0;
	virtual void RemoveWindowProc(IWindow_Event* _pIWindow_Event) = 0;
};