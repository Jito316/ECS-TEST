#pragma once

class FreeCamera
{
public:
	FreeCamera();
	~FreeCamera();

	void Update();
	const Math::Vector3& GetPos() const { return m_trans.pos; }
	ICamera* m_pCamera = nullptr;
private:

	struct 
	{
		Math::Vector3 pos;
		Math::Vector3 angle;
		Math::Matrix mat;
	}m_trans;

	Math::Vector2 m_mousePos;
	Math::Vector2 m_mouseDelta;
public:
	void ZoomIOMove(bool _flg);
	void AngleShift(bool _flg);
	void WASDMove(bool _flg);
	void DragMove(bool _flg);
};
