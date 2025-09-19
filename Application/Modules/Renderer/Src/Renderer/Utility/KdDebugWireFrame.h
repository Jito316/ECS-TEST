#pragma once

class KdDebugWireFrame :public IDebugWireFrame
{
public:
	KdDebugWireFrame() {}
	~KdDebugWireFrame() override { Release(); }

	void AddDebugLine(const Math::Vector3& start, const Math::Vector3& end, const Math::Color& col = kWhiteColor)override;
	void AddDebugLine(const Math::Vector3& start, const Math::Vector3& dir = Math::Vector3(0,0,1), float length = 1.0f, const Math::Color& col = kWhiteColor)override;
	void AddDebugLineFromMatrix(const Math::Matrix& mat, float scale)override;
	void AddDebugSphere(const Math::Vector3& pos, float radius,const Math::Vector3& scale = Math::Vector3::One, const Math::Color& col = kWhiteColor)override;
	void AddDebugArc(const Math::Vector3& pos,float startAngle, float endAngle, float radius, Math::Vector3 scale,const Math::Color& col = kWhiteColor)override;
	void AddDebugBox(const Math::Matrix& matrix, const Math::Vector3& size, const Math::Vector3& offset = { 0, 0, 0 }, const bool isOriented = false, const Math::Color& col = kWhiteColor)override;

	void Draw()override;
	void Clear();

private:
	void Release();

	std::vector<IPolygon::Vertex>	m_debugVertices;
};
