#include "KdDebugWireFrame.h"
#include "Renderer/RenderManager.h"
#include "Renderer/KdShader/KdShaderManager.h"

void KdDebugWireFrame::AddDebugLine(const Math::Vector3& start, const Math::Vector3& end, const Math::Color& col)
{
	// デバッグラインの始点
	IPolygon::Vertex v1;
	v1.color = col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.pos = start;

	// デバッグラインの終点
	IPolygon::Vertex v2;
	v2.color = col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.pos = end;

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void KdDebugWireFrame::AddDebugLine(const Math::Vector3& start, const Math::Vector3& dir, float length, const Math::Color& col)
{
	// デバッグラインの始点
	IPolygon::Vertex v1;
	v1.color = col.RGBA().v;
	v1.UV = Math::Vector2::Zero;
	v1.pos = start;

	// デバッグラインの終点
	IPolygon::Vertex v2;
	v2.color = col.RGBA().v;
	v2.UV = Math::Vector2::Zero;
	v2.pos = v1.pos + (dir * length);

	m_debugVertices.push_back(v1);
	m_debugVertices.push_back(v2);
}

void KdDebugWireFrame::AddDebugLineFromMatrix(const Math::Matrix& mat, float scale)
{
	Math::Vector3 start = mat.Translation();
	Math::Vector3 vAxis;

	// X軸描画
	vAxis = mat.Right();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, kRedColor);

	// Y軸描画
	vAxis = mat.Up();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, kGreenColor);

	// Z軸描画
	vAxis = mat.Backward();
	vAxis.Normalize();
	AddDebugLine(start, start + vAxis * scale, kBlueColor);
}

// デバッグスフィアの描画
void KdDebugWireFrame::AddDebugSphere(const Math::Vector3& pos, float radius, const Math::Vector3& scale, const Math::Color& col)
{
	IPolygon::Vertex v;
	v.UV = Math::Vector2::Zero;
	v.color = col.RGBA().v;

	int detail = 16;
	for (int i = 0; i < detail + 1; ++i)
	{
		// XZ面
		v.pos = pos;
		v.pos.z += sin((float)i * (360 / detail) * KdToRadians) * radius * scale.z;
		v.pos.x += cos((float)i * (360 / detail) * KdToRadians) * radius * scale.x;
		m_debugVertices.push_back(v);

		v.pos = pos;
		v.pos.x += cos((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.x;
		v.pos.z += sin((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.z;
		m_debugVertices.push_back(v);

		// XY面
		v.pos = pos;
		v.pos.x += cos((float)i * (360 / detail) * KdToRadians) * radius * scale.x;
		v.pos.y += sin((float)i * (360 / detail) * KdToRadians) * radius * scale.y;
		m_debugVertices.push_back(v);

		v.pos = pos;
		v.pos.x += cos((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.x;
		v.pos.y += sin((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.y;
		m_debugVertices.push_back(v);

		// YZ面
		v.pos = pos;
		v.pos.y += cos((float)i * (360 / detail) * KdToRadians) * radius * scale.y;
		v.pos.z += sin((float)i * (360 / detail) * KdToRadians) * radius * scale.z;
		m_debugVertices.push_back(v);

		v.pos = pos;
		v.pos.y += cos((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.y;
		v.pos.z += sin((float)(i + 1) * (360 / detail) * KdToRadians) * radius * scale.z;
		m_debugVertices.push_back(v);
	}
}

void KdDebugWireFrame::AddDebugArc(const Math::Vector3& pos, float startAngle, float endAngle, float radius, Math::Vector3 scale, const Math::Color& col)
{
	IPolygon::Vertex v;
	v.UV = Math::Vector2::Zero;
	v.color = col.RGBA().v;
	//中心最初のPointへの線
	{
		v.pos = pos;
		m_debugVertices.push_back(v);
		float angle = -DirectX::XMConvertToRadians(std::lerp(startAngle, endAngle, 0.0f) - 90.0f);
		v.pos = pos;
		v.pos.x += cos(angle) * radius;
		v.pos.z += sin(angle) * radius;
		m_debugVertices.push_back(v);
	}

	int detail = 16;
	for (int i = 0; i < detail; ++i)
	{
		float angle = -DirectX::XMConvertToRadians(std::lerp(startAngle, endAngle, (float)i / detail) - 90.0f);
		v.pos = pos;
		v.pos.x += cos(angle) * radius;
		v.pos.z += sin(angle) * radius;
		m_debugVertices.push_back(v);

		angle = -DirectX::XMConvertToRadians(std::lerp(startAngle, endAngle, (float)(i + 1) / detail) - 90.0f);
		v.pos = pos;
		v.pos.x += cos(angle) * radius;
		v.pos.z += sin(angle) * radius;
		m_debugVertices.push_back(v);
	}

	//最後のPointから中心への線
	{
		float angle = -DirectX::XMConvertToRadians(std::lerp(startAngle, endAngle, 1.0f) - 90.0f);
		v.pos = pos;
		v.pos.x += cos(angle) * radius;
		v.pos.z += sin(angle) * radius;
		m_debugVertices.push_back(v);
		v.pos = pos;
		m_debugVertices.push_back(v);
	}
}

// デバッグボックスの描画
void KdDebugWireFrame::AddDebugBox(const Math::Matrix& matrix, const Math::Vector3& size, const Math::Vector3& offset, const bool isOriented, const Math::Color& col)
{
	IPolygon::Vertex v;
	v.UV = Math::Vector2::Zero;
	v.color = col.RGBA().v;

	Math::Vector3					corners[8];
	DirectX::BoundingBox			aabb;
	DirectX::BoundingOrientedBox	obb;
	// ボックス(AABB)
	if (!isOriented)
	{
		aabb.Center = matrix.Translation() + offset;
		aabb.Extents = size;

		aabb.GetCorners(corners);
	}
	// ボックス(OBB)
	else
	{
		Math::Matrix createMat = matrix;
		createMat.Translation(createMat.Translation() + offset);
		obb.Transform(obb, createMat);
		obb.Extents = size;

		obb.GetCorners(corners);
	}

	// 底辺定義
	{
		//底辺1
		v.pos = corners[0];
		m_debugVertices.push_back(v);

		v.pos = corners[1];
		m_debugVertices.push_back(v);

		//底辺2
		v.pos = corners[0];
		m_debugVertices.push_back(v);

		v.pos = corners[4];
		m_debugVertices.push_back(v);

		//底辺3
		v.pos = corners[5];
		m_debugVertices.push_back(v);

		v.pos = corners[1];
		m_debugVertices.push_back(v);

		//底辺4
		v.pos = corners[5];
		m_debugVertices.push_back(v);

		v.pos = corners[4];
		m_debugVertices.push_back(v);
	}

	// 中辺定義
	{
		//中辺1
		v.pos = corners[0];
		m_debugVertices.push_back(v);

		v.pos = corners[3];
		m_debugVertices.push_back(v);

		//中辺2
		v.pos = corners[1];
		m_debugVertices.push_back(v);

		v.pos = corners[2];
		m_debugVertices.push_back(v);

		//中辺3
		v.pos = corners[4];
		m_debugVertices.push_back(v);

		v.pos = corners[7];
		m_debugVertices.push_back(v);

		//中辺4
		v.pos = corners[5];
		m_debugVertices.push_back(v);

		v.pos = corners[6];
		m_debugVertices.push_back(v);
	}

	// 上辺定義
	{
		//上辺1
		v.pos = corners[2];
		m_debugVertices.push_back(v);

		v.pos = corners[3];
		m_debugVertices.push_back(v);

		//上辺2
		v.pos = corners[2];
		m_debugVertices.push_back(v);

		v.pos = corners[6];
		m_debugVertices.push_back(v);

		//上辺3
		v.pos = corners[7];
		m_debugVertices.push_back(v);

		v.pos = corners[3];
		m_debugVertices.push_back(v);

		//上辺4
		v.pos = corners[7];
		m_debugVertices.push_back(v);

		v.pos = corners[6];
		m_debugVertices.push_back(v);
	}
}

void KdDebugWireFrame::Draw()
{
	if (m_debugVertices.size() < 2) { return; }

	GETRENDERER->m_kdShaderManager->m_StandardShader.DrawVertices(m_debugVertices, Math::Matrix::Identity);
}

void KdDebugWireFrame::Clear()
{
	m_debugVertices.clear();
}

void KdDebugWireFrame::Release()
{
	m_debugVertices.clear();
}
