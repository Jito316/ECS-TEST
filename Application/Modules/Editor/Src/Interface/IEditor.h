#pragma once
#ifdef EDITOR_DLL_EXPORTS
#define EDITOR_API __declspec(dllexport)
#define IMGUI_API __declspec(dllexport)
#else
#define EDITOR_API __declspec(dllimport)
#define IMGUI_API __declspec(dllimport)
#endif

#define USE_IMGUI_API

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_stdlib.h"
#include "ImGuizmo/ImGuizmo.h"

class IEditorWindow;
class IGameObject;

class EDITOR_API IEditor : public JTN::Module::IModule
{
public:
	virtual void SetActive(bool _flg) = 0;
	virtual bool GetActive() const = 0;

	virtual void SetEditObject(std::weak_ptr<IGameObject> _obj) = 0;
	virtual std::weak_ptr<IGameObject> GetEditObject() = 0;

	virtual ImGuiIO& GetIO() = 0;
	virtual ImGuiStyle& GetStyle() = 0;
	virtual ImGuiContext& GetContext() = 0;

	virtual void AddEditorWindow(std::string_view _name,std::shared_ptr<IEditorWindow> _editorWindow) = 0;
	template<class T> std::weak_ptr<T> GetEditorWindow() {
		return std::static_pointer_cast<T>(GetEditorWindow(typeid(T)));
	}

protected:
	virtual std::shared_ptr<IEditorWindow> GetEditorWindow(const std::type_info& _info) = 0;
};
