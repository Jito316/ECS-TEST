#include "GameScreen.h"
#include "FreeCamera/FreeCamera.h"
#include "Editor/UndoRedo/UndoRedo.h"
#include "Editor/UndoRedo/Action/EditeGizmo/EditeGizmo.h"

void GameScreen::UpdateContents()
{
	auto iScript = GETMODULE(IScript);
	bool cameraEnableSate = iScript && !iScript->isPlay();
	if (!m_buildCamera && cameraEnableSate)
	{
		m_buildCamera = std::make_shared<FreeCamera>();
	}
	else if (!cameraEnableSate)
	{
		m_buildCamera = nullptr;
	}

	ImVec2 windowSize(1280.0f, 720.0f);
	m_imageSize = ImGui::GetContentRegionAvail() / windowSize;
	m_imageSize = windowSize * (m_imageSize.x < m_imageSize.y ? m_imageSize.x : m_imageSize.y); //ゲームスクリーンのサイズ決定

	auto  temp = GETMODULE(IRenderer);
	//ゲームビュー表示
	auto camera = m_buildCamera ? (m_buildCamera->m_pCamera) : GETMODULE(IRenderer)->GetMainCamera();
	if (camera && (m_spTexture = camera->GetViewTexture())) {
		Utility::ImGuiHelper::ImageWindowCenter(m_spTexture->WorkSRView(), m_imageSize);
	}
	ImRect rect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMax() };

	//ゲームスクリーンへの操作
	UpdateCamera();
	UpdateGizmo();
	PickObject(rect, ImGui::GetMousePos());

	//ギズモの編集対象の変更
	ImGui::Begin("##zmoOption");
	{
		Utility::ImGuiHelper::ComboEnum("MouseLButtonState", m_mouseLButtonState);
		if (m_mouseLButtonState == MouseLButtonState::Gizmo)
		{
			Utility::ImGuizmoHelper::OperationChanger(m_zmoOperation);
			Utility::ImGuizmoHelper::ModeChanger(m_zmoMode);
		}
	}
	ImGui::End();
}

void GameScreen::BeginChild()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
	IEditorWindow::BeginChild();
	ImGui::PopStyleVar();
}

bool InBox(float* _pos, float* _min, float* _max) {
	if (_pos[0] < _min[0])return false;
	if (_pos[0] > _max[0])return false;
	if (_pos[1] < _min[1])return false;
	if (_pos[1] > _max[1])return false;
	return true;
}

void GameScreen::UpdateCamera()
{
	//ビルドカメラ
	if (!m_buildCamera)return;

	ImVec2 windowMin = ImGui::GetItemRectMin();
	ImVec2 windowMax = ImGui::GetItemRectMax();
	ImVec2 mousePos = ImGui::GetMousePos();

	if (InBox(&mousePos.x, &windowMin.x, &windowMax.x))
	{
		m_itemActive = true && !ImGui::IsDragDropActive();
	}
	if (m_itemActive)
	{
		m_buildCamera->DragMove(m_mouseLButtonState == MouseLButtonState::DragMove && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_MouseLeft));
		m_buildCamera->AngleShift(ImGui::IsKeyDown(ImGuiKey::ImGuiKey_MouseRight));
		m_buildCamera->WASDMove(ImGui::IsKeyDown(ImGuiKey::ImGuiKey_MouseRight));
		m_buildCamera->ZoomIOMove(true);
		m_itemActive = ImGui::IsKeyDown(ImGuiKey::ImGuiKey_MouseRight) || ImGui::IsKeyDown(ImGuiKey::ImGuiKey_MouseLeft);
	}

	m_buildCamera->Update();
}
void GameScreen::UpdateGizmo()
{
	if (m_mouseLButtonState != MouseLButtonState::Gizmo)return;
	//============================================
	// ギズモ処理
	//============================================
	std::weak_ptr<IGameObject> editObject = GetOwner()->GetEditObject();
	if (editObject.expired() || (!m_buildCamera))return;
	std::weak_ptr<ITransform> trans = editObject.lock()->GetTransform();
	Math::Matrix transform = trans.lock()->GetWorldMatrix();
	Math::Matrix prevMat = transform;
	//ギズモ更新処理
	if (Utility::ImGuizmoHelper::Update(
		m_buildCamera->m_pCamera->GetCameraViewMatrix(),
		m_buildCamera->m_pCamera->GetProjMatrix(),
		transform,
		m_zmoOperation,
		m_zmoMode
	))
	{
		trans.lock()->SetWorldMatrix(transform);
		if (!m_pPrevTrans) 
		{
			m_pPrevTrans = std::make_shared<Math::Matrix>(prevMat);
		}
	}

	if (m_pPrevTrans && !ImGuizmo::IsOver())
	{
		GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::EditeGizmo>(trans, *m_pPrevTrans));
		m_pPrevTrans.reset();
	}
}

void GameScreen::PickObject(ImRect itemSize, ImVec2 mousePos)
{
	//ギズモ使用中もしくはカメラが存在していなかったときリターン
	if (ImGuizmo::IsUsing())return;
	if (m_buildCamera == nullptr)return;

	//左クリックがないもしくはマウスがスクリーン上にない場合リターン
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left) == false)return;
	if (std::max(itemSize.Max.x, mousePos.x) == mousePos.x)return;
	if (std::max(itemSize.Max.y, mousePos.y) == mousePos.y)return;
	if (std::min(itemSize.Min.x, mousePos.x) == mousePos.x)return;
	if (std::min(itemSize.Min.y, mousePos.y) == mousePos.y)return;

	// ゲームスクリーンからマウスの相対位置をとって
	// ゲームスクリーンのサイズが(1280,720)の時に補正する
	mousePos -= itemSize.Min;
	mousePos /= itemSize.Max - itemSize.Min;
	auto& size = GETMODULE(IWindow)->GetWinodwSize();
	mousePos *= {(float)size[0], (float)size[1]};
	POINT mouse((LONG)(int)mousePos.x, (LONG)(int)mousePos.y);

	Math::KdCollider::RayInfo ray;
	{
		m_buildCamera->m_pCamera->GenerateRayInfoFromClientPos(
			mouse
			, ray.m_pos
			, ray.m_dir
			, ray.m_range
		);
	}

	std::weak_ptr<IGameObject> _object;
	if (SCRIPT_COMMAND("SelectGameObject", &ray, &_object)) {
		GETEDITOR.SetEditObject(_object);
	}
}

