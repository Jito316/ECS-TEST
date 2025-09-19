#include "FreeCamera.h"

FreeCamera::FreeCamera()
{
	m_pCamera = GETMODULE(IRenderer)->CreateCamera();
}

FreeCamera::~FreeCamera()
{
	delete m_pCamera;
}

void FreeCamera::Update()
{
	m_trans.mat = {
		Math::Matrix::CreateRotationX(Utility::MathHelper::ToRadians(m_trans.angle.x))
		* Math::Matrix::CreateRotationY(Utility::MathHelper::ToRadians(m_trans.angle.y))
		* Math::Matrix::CreateRotationZ(Utility::MathHelper::ToRadians(m_trans.angle.z))
		* Math::Matrix::CreateTranslation(m_trans.pos)
	};

	m_pCamera->SetCameraMatrix(m_trans.mat);
	m_pCamera->Register(ICamera::Type::Sub);

	POINT mousePos; IWindow* pWIndow;
	if (GetCursorPos(&mousePos) && (pWIndow = GETMODULE(IWindow))) {
		ScreenToClient(pWIndow->GetWndHandle(), &mousePos);
		auto& size = pWIndow->GetWinodwSize();
		mousePos.x -= size[0] / 2;
		mousePos.y -= size[1] / 2;
		Math::Vector2 nowMousePos{ (float)mousePos.x,-(float)mousePos.y };
		m_mouseDelta = m_mousePos - nowMousePos;
		m_mousePos = nowMousePos;
	}
}

void FreeCamera::ZoomIOMove(bool _flg)
{
	if (!_flg)return;
	if (this == nullptr)return;

	Math::Vector3 vec;
	int wheelValue = GETMODULE(IWindow)->GetMouseWheelVal();

	if (wheelValue == 0)return;
	else if (wheelValue < 0)vec = m_trans.mat.Forward();
	else if (wheelValue > 0)vec = m_trans.mat.Backward();

	m_trans.pos += vec * 0.5;
}
void FreeCamera::AngleShift(bool _flg)
{
	if (!_flg)return;
	if (this == nullptr)return;
	Math::Vector3& angle = m_trans.angle;
	angle.x += m_mouseDelta.y;
	angle.y -= m_mouseDelta.x;
	if (angle.x > 90)angle.x = 90;
	if (angle.x < -90)angle.x = -90;
}
void FreeCamera::WASDMove(bool _flg)
{
	if (!_flg)return;
	if (this == nullptr)return;
	const Math::Matrix& transform = m_trans.mat;

	Math::Vector3 move;
	if (ImGui::IsKeyDown(ImGuiKey_W))move += transform.Backward() * 1.5;
	if (ImGui::IsKeyDown(ImGuiKey_A))move += transform.Left() * 1.5;
	if (ImGui::IsKeyDown(ImGuiKey_S))move += transform.Forward() * 1.5;
	if (ImGui::IsKeyDown(ImGuiKey_D))move += transform.Right() * 1.5;

	m_trans.pos += move;
}


void FreeCamera::DragMove(bool _flg)
{
	if (!_flg)return;
	if (this == nullptr)return;

	Math::Vector3& pos = m_trans.pos;
	const Math::Matrix& transform = m_trans.mat;

	Math::Vector2 movePow = { abs(m_mouseDelta.x),abs(m_mouseDelta.y) };
	movePow.Normalize();
	movePow *= 0.05f;

	if (m_mouseDelta.x < 0.f)		pos += transform.Left() * movePow.x;
	else if (m_mouseDelta.x > 0.f)	pos += transform.Right() * movePow.x;

	if (m_mouseDelta.y > 0.f)		pos += transform.Up() * movePow.y;
	else if (m_mouseDelta.y < 0.f)	pos += transform.Down() * movePow.y;
}