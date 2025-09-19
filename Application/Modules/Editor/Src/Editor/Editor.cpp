#include "Editor.h"
#include "UndoRedo/UndoRedo.h"
#include "Interface/EditorWindow/IEditorWindow.h"

#include "EditorWindow/Hierarchy/Hierarchy.h"
#include "EditorWindow/GameScreen/GameScreen.h"
#include "EditorWindow/Inspector/Inspector.h"
#include "EditorWindow/ProjectWindow/ProjectWindow.h"
#include "EditorWindow/DebugLog/DebugLog.h"
#include "EditorWindow/SetUpScene/SetUpScene.h"
#include "EditorTask/Register/EditorTaskRegister.h"

#include "Utility/Config/Config.h"

#include "Interface/IWindowEvent.h"
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
class ImGuiWindowProc : public IWindow_Event
{
public:
	LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
	}
};

void Editor::AddEditorWindow(std::string_view _name, std::shared_ptr<IEditorWindow> _editorWindow)
{
	m_editorWindows[_name.data()] = _editorWindow;
	_editorWindow->SetName(_name.data());
	_editorWindow->SetOwner(this);

	if (m_initialized)
	{
		_editorWindow->Init();
	}
}

bool Editor::Entry()
{
	m_pWindow = GETMODULE(IWindow);
	m_pRenderer = GETMODULE(IRenderer);
	m_pInput = GETMODULE(IInput);
	m_pScript = GETMODULE(IScript);

	ModuleEntry(ModuleEntry_InitializeEditor, &Editor::SetUp);
	ModuleEntry(ModuleEntry_UpdateEditor, &Editor::Update);
	ModuleEntry(ModuleEntry_FinalizeEditor, &Editor::Shutdown);
	ModuleEntry(ModuleEntry_EditorProcess, &Editor::Render);
	return m_pWindow && m_pRenderer;
}

void Editor::SetUp()
{
	//===================================================================
	// imgui初期設定
	//===================================================================
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// Setup Dear ImGui style
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // ドッキングを有効にする
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // マルチビューポートを有効にする
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボードナビゲーションを有効にする
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		if (!ImGui_ImplWin32_Init(m_pWindow->GetWndHandle()))return;
		IDirect3D& kdD3D = *m_pRenderer->GetDirect3D();
		if (!ImGui_ImplDX11_Init(kdD3D.WorkDev(), kdD3D.WorkDevContext()))return;
		{
			// 日本語対応
#include "ja_glyph_ranges.h"
			ImFontConfig config;
			config.MergeMode = true;
			io.Fonts->AddFontDefault();
			io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);
		}
	}

	//===================================================================
	// EditorWindow初期設定
	//===================================================================
	{
#define EDITORREGISTER(Tag)AddEditorWindow(#Tag,std::make_shared<Tag>());
		// 登録
		EDITORREGISTER(Hierarchy);
		EDITORREGISTER(GameScreen);
		EDITORREGISTER(Inspector);
		EDITORREGISTER(ProjectWindow);
		EDITORREGISTER(SetUpScene);
		EDITORREGISTER(DebugLog);

		// debug初期化
		m_debugLog = std::make_shared<DebugLog>();
		AddEditorWindow("DebugLog", m_debugLog);

		for (auto& [key, value] : m_editorWindows)
		{
			value->Init();
		}

		// 定数代入
		JTN::File::Config config; nlohmann::json data;
		if (config.Load("Editor", data))
		{
			auto it = data.begin();
			while (it != data.end())
			{
				if (m_editorWindows.find(it.key()) != m_editorWindows.end())
				{
					m_editorWindows[it.key()]->SetActive(*it);
				}
				it++;
			}
		}
		else
		{
			m_editorWindows["Hierarchy"]->SetActive(true);
			m_editorWindows["GameScreen"]->SetActive(true);
			m_editorWindows["Inspector"]->SetActive(true);
			m_editorWindows["ProjectWindow"]->SetActive(true);
		}

		m_initialized = true;
	}

	//===================================================================
	// Editorコマンド初期設定
	//===================================================================
	{
		m_spUndoRedo = std::make_shared<RedoUndoManager>();
		RegisterEditorTask();
	}

	//===================================================================
	// ImGuiのウィンドウプロシージャーの登録
	//===================================================================
	{
		m_pWindow_Event = new ImGuiWindowProc;
		m_pWindow->AddWindowProc(m_pWindow_Event);
	}
}
void Editor::Update()
{
	if (BeginEditor()) {
		UpdateEditor();
	}
	EndEditor();
}
void Editor::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
void Editor::Shutdown()
{
	nlohmann::json data;
	for (auto& [key, value] : m_editorWindows)data[key] = value->GetActive();
	JTN::File::Config().Save("Editor", data);

	m_taskMap.reset();

	while (m_editorWindows.size())
	{
		auto it = m_editorWindows.begin();
		it->second->Release();
		m_editorWindows.erase(it);
	}

	if (m_pScript)
	{
		m_pScript->SetState(RuntimeState::Play);
		m_pScript->LoadScene(m_pScript->GetStartName());
	}

	//===================================================================
	// ImGuiのウィンドウプロシージャーの登録
	//===================================================================
	{
		m_pWindow->RemoveWindowProc(m_pWindow_Event);
		delete m_pWindow_Event;
	}

	//imgui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Editor::MenuBar()
{
	auto iScript = GETMODULE(IScript);
	bool enable = iScript && iScript->isEdit();
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File", enable))
		{
			IEditor* temp = this;
			auto setScence = temp->GetEditorWindow<SetUpScene>();
			if (ImGui::MenuItem("Open"))setScence.lock()->Open();
			if (ImGui::MenuItem("Save"))setScence.lock()->Save();
			if (ImGui::MenuItem("Save as"))setScence.lock()->SaveAs();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Option", enable))
		{
			bool flg = JTN::GetEngine()->is_Debug();
			if (ImGui::Checkbox("Debug", &flg)) JTN::GetEngine()->SetDebug(flg);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window", enable))
		{
			for (auto& item : m_editorWindows)
			{
				if (ImGui::MenuItem(item.first.c_str(), "", item.second->GetActive()))
				{
					item.second->SetActive(!item.second->GetActive());
				}
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit", enable))
		{
			if (ImGui::MenuItem("Undo"))m_spUndoRedo->Undo();
			if (ImGui::MenuItem("Redo"))m_spUndoRedo->Redo();
			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Extensions", enable)) {
			if (std::string _str
				; ImGui::MenuItem("Add")
				&& GETMODULE(IWindow)->OpenFileDialog(_str, "dllを読み込む", "dllファイル\0*.dll*\0")) {
				std::filesystem::path file(_str);
				JTN::GetEngine()->GetModuleSystem()->Load(file.filename().string().substr(0, file.filename().string().size() - 4));
			}
			if (ImGui::MenuItem("Delete")) {
				JTN::GetEngine()->GetModuleSystem()->Remove(this);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}
void Editor::PlayButton()
{
	ImVec2 defaultSpacing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(defaultSpacing.x, 1.0f));

	ImGui::BeginGroup();
	{
		Utility::ImGuiHelper::SetCenterCursorWidth();
		StartButton();
		ImGui::SameLine();
		StopButton();
	}
	ImGui::EndGroup();

	ImGui::PopStyleVar();
}

void Editor::StartButton()
{
	bool edited = false;
	bool playing = m_pScript && m_pScript->isPlay();

	if (edited = ImGui::Checkbox("Start", &playing));
	else if (edited = m_pInput && m_pInput->IsPress("F5")) playing = true;
	edited = edited && ((m_pScript && m_pScript->isPlay()) != playing);

	if (edited || !m_pScript->isEdit() && m_pInput && m_pInput->IsPress("ESC"))
	{
		if (!edited) 
		{
			playing = false;
		}

		m_spUndoRedo->Clear();
		m_pScript->SetState(playing ? RuntimeState::Play : RuntimeState::Edit);
		auto& name = m_pScript->GetStartName();
		m_pScript->LoadScene(name);
	}
}
void Editor::StopButton()
{
	bool playing = m_pScript && m_pScript->isPlay();
	bool stoped = m_pScript && m_pScript->isStop();
	if (!stoped && !playing)return;

	bool edited = false;
	if (edited = m_pInput && m_pInput->IsPress("P"))stoped = !stoped;
	else edited = ImGui::Checkbox("stop", &stoped);

	if (edited) 
	{
		m_pScript->SetState(stoped ? RuntimeState::Stop : RuntimeState::Play);
	}
}

std::shared_ptr<IEditorWindow> Editor::GetEditorWindow(const std::type_info& _info)
{
	for (auto& it : m_editorWindows)
	{
		if (typeid(*it.second) == _info)
		{
			return it.second;
		}
	}
	return nullptr;
}

bool Editor::BeginEditor()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	m_spUndoRedo->Update();
	if (m_pInput && m_pInput->IsPress("F1"))m_isActive = !m_isActive;

	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	bool flg = m_isActive && ImGui::Begin("DockSpace Demo", &m_isActive, window_flags);
	ImGui::PopStyleVar();

	return flg;
}

void Editor::UpdateEditor()
{
	MenuBar();
	PlayButton();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(ImGui::GetID("DockSpace Demo"));
	}

	for (auto& window : m_editorWindows)
	{
		if (window.second->GetActive())
		{
			window.second->Update();
		}
	}
}
void Editor::EndEditor()
{
	ImGui::End();
}

extern"C" EDITOR_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress) {
	*_pModuleAdress = new Editor;
	*_pInterfaceInfoAdress = &typeid(IEditor);
}
extern"C" EDITOR_API void DeleteModule(JTN::Module::IModule* _pModule) {
	delete _pModule;
}