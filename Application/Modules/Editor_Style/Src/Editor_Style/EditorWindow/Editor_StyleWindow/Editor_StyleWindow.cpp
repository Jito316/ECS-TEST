#include "Editor_StyleWindow.h"

void SetOrangeStyle(ImGuiStyle* _pImGuiStyle) {
	ImGuiStyle& style = *_pImGuiStyle;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.94f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.30f, 0.30f, 0.30f, 0.54f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.36f, 0.33f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.52f, 0.44f, 0.37f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.83f, 0.54f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.83f, 0.54f, 0.18f, 0.51f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.83f, 0.54f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.83f, 0.54f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.71f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.83f, 0.54f, 0.18f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.83f, 0.54f, 0.18f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.83f, 0.54f, 0.18f, 1.00f);
	style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.68f, 0.69f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.83f, 0.54f, 0.18f, 0.20f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.83f, 0.54f, 0.18f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.95f, 0.58f, 0.08f, 0.95f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.83f, 0.54f, 0.18f, 0.86f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.95f, 0.58f, 0.08f, 0.80f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.69f, 0.45f, 0.15f, 0.97f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.73f, 0.48f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.83f, 0.54f, 0.18f, 0.43f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.95f, 0.58f, 0.08f, 1.00f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void Editor_StyleWindow::UpdateContents()
{
	ImGui::ShowStyleEditor(&GetOwner()->GetStyle());
}

void Editor_StyleWindow::Init()
{
	SetOrangeStyle(&GetOwner()->GetStyle());
	ImGui::SetCurrentContext(&GetOwner()->GetContext());
}

void Editor_StyleWindow::Release()
{

}
