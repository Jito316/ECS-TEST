#include "RendererPch.h"
#include "KdSpriteShader.h"
#include "Renderer/KdDirect3D/KdDirect3D.h"
#include "Renderer/KdShader/KdShaderManager.h"

bool KdSpriteShader::Init()
{
	auto& renderer = *GETRENDERER;
	Release();

	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
		#include "KdSpriteShader_VS.shaderInc"

		// 頂点シェーダー作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点インプットレイアウト作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateInputLayout(
			&layout[0],
			layout.size(),
			compiledBuffer,
			sizeof(compiledBuffer),
			&m_VLayout))
		){
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
		#include "KdSpriteShader_PS.shaderInc"

		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0.Create();
	m_cb1.Create();

	return true;
}

void KdSpriteShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_VLayout);
	m_cb0.Release();
	m_cb1.Release();
}

void KdSpriteShader::Begin(bool linear, bool disableZBuffer)
{
	// 既にBeginしている
	if (m_isBegin)return;
	m_isBegin = true;
	auto& renderer = *GETRENDERER;

	//---------------------------------------
	// 2D用正射影行列作成
	//---------------------------------------
	UINT pNumVierports = 1;
	D3D11_VIEWPORT vp;
	renderer.m_kdDirect3D->WorkDevContext()->RSGetViewports(&pNumVierports, &vp);
	m_mProj2D = DirectX::XMMatrixOrthographicLH(vp.Width, vp.Height, 0, 1);

	// 定数バッファ書き込み
	m_cb1.Work().mProj = m_mProj2D;
	m_cb1.Write();

	//---------------------------------------
	// 使用するステートをセット
	//---------------------------------------
	// Z判定、Z書き込み無効のステートをセット
	if (disableZBuffer) {
		renderer.m_kdShaderManager->ChangeDepthStencilState(KdDepthStencilState::ZDisable);
	}
	// Samplerステートをセット
	if (linear) {
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Linear_Clamp);
	}
	else {
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Point_Clamp);
	}
	// Rasterizerステートをセット
	renderer.m_kdShaderManager->ChangeRasterizerState(KdRasterizerState::CullNone);

	//---------------------------------------
	// シェーダ
	//---------------------------------------

	// シェーダをセット
	renderer.m_kdDirect3D->WorkDevContext()->VSSetShader(m_VS, 0, 0);
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// 頂点レイアウトセット
	renderer.m_kdDirect3D->WorkDevContext()->IASetInputLayout(m_VLayout);

	// 定数バッファセット
	renderer.m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	renderer.m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	renderer.m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(1, 1, m_cb1.GetAddress());
	renderer.m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(1, 1, m_cb1.GetAddress());
}

void KdSpriteShader::End()
{
	if (!m_isBegin) { return; }
	m_isBegin = false;
	auto& renderer = *GETRENDERER;

	//---------------------------------------
	// 記憶してたステートに戻す
	//---------------------------------------
	renderer.m_kdShaderManager->UndoDepthStencilState();
	renderer.m_kdShaderManager->UndoSamplerState();
	renderer.m_kdShaderManager->UndoRasterizerState();
}

void KdSpriteShader::DrawTex(const ITexture* tex, int x, int y, int w, int h, const Math::Rectangle* srcRect, const Math::Color* color, const Math::Vector2& pivot)
{
	if (tex == nullptr)return;
	auto& renderer = *GETRENDERER;

	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = this->m_isBegin;
	if (!bBgn)Begin();

	// テクスチャ(ShaderResourceView)セット
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, tex->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	// UV
	Math::Vector2 uvMin = { 0, 0 };
	Math::Vector2 uvMax = { 1, 1 };
	if (srcRect)
	{
		uvMin.x = srcRect->x / (float)tex->GetInfo()->Width;
		uvMin.y = srcRect->y / (float)tex->GetInfo()->Height;

		uvMax.x = (srcRect->x + srcRect->width) / (float)tex->GetInfo()->Width;
		uvMax.y = (srcRect->y + srcRect->height) / (float)tex->GetInfo()->Height;
	}

	// 頂点作成
	float x1 = (float)x;
	float y1 = (float)y;
	float x2 = (float)(x + w);
	float y2 = (float)(y + h);

	// 基準点(Pivot)ぶんずらす
	x1 -= pivot.x * w;
	x2 -= pivot.x * w;
	y1 -= pivot.y * h;
	y2 -= pivot.y * h;

	Vertex vertex[] = {
		{ {x1, y1, 0},	{uvMin.x, uvMax.y} },
		{ {x1, y2, 0},	{uvMin.x, uvMin.y} },
		{ {x2, y1, 0},	{uvMax.x, uvMax.y} },
		{ {x2, y2, 0},	{uvMax.x, uvMin.y} }

	};

	// 描画
	renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));

	// セットしたテクスチャを解除しておく
	ID3D11ShaderResourceView* srv = nullptr;
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, &srv);

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawPoint(int x, int y, const Math::Color* color)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();
	auto& renderer = *GETRENDERER;

	// 白テクスチャ
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, renderer.m_kdDirect3D->GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// 描画
	Vertex vertex[] = {
		{ {(float)x, (float)y, 0},	{0, 0} },
	};
	renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_POINTLIST, 1, vertex, sizeof(Vertex));

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawLine(int x1, int y1, int x2, int y2, const Math::Color* color)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();
	auto& renderer = *GETRENDERER;

	// 白テクスチャ
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, renderer.m_kdDirect3D->GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// 描画
	Vertex vertex[] = {
		{ {(float)x1, (float)y1, 0},	{0, 0} },
		{ {(float)x2, (float)y2, 0},	{1, 0} },
	};
	renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 2, vertex, sizeof(Vertex));

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Math::Color* color, bool fill)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();
	auto& renderer = *GETRENDERER;

	// 白テクスチャ
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, renderer.m_kdDirect3D->GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();


	// 描画
	Vertex vertex[] = {
		{ {(float)x1, (float)y1, 0},	{0, 0} },
		{ {(float)x2, (float)y2, 0},	{1, 0} },
		{ {(float)x3, (float)y3, 0},	{0, 0} },
		{ {(float)x1, (float)y1, 0},	{1, 0} },
	};
	renderer.m_kdDirect3D->DrawVertices(
		fill ? D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP : D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
		4, vertex, sizeof(Vertex));


	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawCircle(int x, int y, int radius, const Math::Color* color, bool fill)
{
	if (radius <= 0)return;

	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();
	auto& renderer = *GETRENDERER;

	// 白テクスチャ
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, renderer.m_kdDirect3D->GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	// 頂点
	if (fill)
	{
		int faceNum = radius + 1;
		if (faceNum > 300)faceNum = 300;
		std::vector<Vertex> vertex(faceNum * 3);		// 半径により頂点数を調整

		// 描画
		for (int i = 0; i < faceNum; i++)
		{
			int idx = i * 3;
			vertex[idx].Pos.x = (float)x;
			vertex[idx].Pos.y = (float)y;

			vertex[idx+1].Pos.x = x + cos(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+1].Pos.y = y + sin(DirectX::XMConvertToRadians(i * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+1].Pos.z = 0;

			vertex[idx+2].Pos.x = x + cos(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+2].Pos.y = y + sin(DirectX::XMConvertToRadians((i+1) * (360.0f / (faceNum - 1)))) * (float)radius;
			vertex[idx+2].Pos.z = 0;
		}

		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (int)vertex.size(), &vertex[0], sizeof(Vertex));
	}
	else
	{
		int numVertex = radius + 1;
		if (numVertex > 300)numVertex = 300;
		std::vector<Vertex> vertex(numVertex);		// 半径により頂点数を調整

		// 描画
		for (int i = 0; i < numVertex; i++)
		{
			vertex[i].Pos.x = x + cos(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)radius;
			vertex[i].Pos.y = y + sin(DirectX::XMConvertToRadians(i * (360.0f / (numVertex - 1)))) * (float)radius;
			vertex[i].Pos.z = 0;
		}

		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, numVertex, &vertex[0], sizeof(Vertex));
	}

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

void KdSpriteShader::DrawBox(int x, int y, int extentX, int extentY, const Math::Color* color, bool fill)
{
	// もし開始していない場合は開始する(最後にEnd())
	bool bBgn = m_isBegin;
	if (!bBgn)Begin();
	auto& renderer = *GETRENDERER;

	// 白テクスチャ
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, 1, renderer.m_kdDirect3D->GetWhiteTex()->WorkSRViewAddress());

	// 色
	if (color) {
		m_cb0.Work().Color = *color;
	}
	m_cb0.Write();

	Math::Vector3 p1 = { (float)x - extentX, (float)y - extentY, 0 };
	Math::Vector3 p2 = { (float)x - extentX, (float)y + extentY, 0 };
	Math::Vector3 p3 = { (float)x + extentX, (float)y + extentY, 0 };
	Math::Vector3 p4 = { (float)x + extentX, (float)y - extentY, 0 };

	// 描画
	if (fill)
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p4, {0,0}},
			{ p3, {0,0}}
		};

		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, 4, vertex, sizeof(Vertex));
	}
	else
	{
		Vertex vertex[] = {
			{ p1, {0,0}},
			{ p2, {0,0}},
			{ p3, {0,0}},
			{ p4, {0,0}},
			{ p1, {0,0}}
		};

		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 5, vertex, sizeof(Vertex));
	}

	// この関数でBeginした場合は、Endしておく
	if (!bBgn)End();
}

// 切り抜き範囲を設定する
// ・rect			… 範囲

void KdSpriteShader::SetScissorRect(const Math::Rectangle& rect)
{
	auto& renderer = *GETRENDERER;

	// ラスタライザステート作成・セット
	ID3D11RasterizerState* rs = renderer.m_kdDirect3D->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, true);
	renderer.m_kdDirect3D->WorkDevContext()->RSSetState(rs);
	rs->Release();

	D3D11_RECT rc{};
	rc.left = rect.x;
	rc.top = rect.y;
	rc.right = rect.x + rect.width;
	rc.bottom = rect.y + rect.height;
	renderer.m_kdDirect3D->WorkDevContext()->RSSetScissorRects(1, &rc);
}
