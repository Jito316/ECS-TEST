#include "KdShaderManager.h"
#include "Renderer/RenderManager.h"
#include "Renderer/KdDirect3D/KdDirect3D.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 全てのシェーダーオブジェクトの生成
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 描画するシェーダー本体の作成（メモリ確保）
// 全ての描画に必要であろうカメラ・ライトの定数バッファの作成
// 描画に関する様々なステートの作成（実行中に切り替える用：切り替えるたびに新しく作るのは無駄
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::Init()
{
	auto renderer = GETRENDERER;

	//============================================
	// シェーダ
	//============================================
	m_spriteShader.Init();
	m_StandardShader.Init();
	m_postProcessShader.Init();

	//============================================
	// 定数バッファ
	//============================================

	// カメラ：ビュー行列・射影行列・フォグのパラメータ
	m_cb7_Camera.Create();
	renderer->m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(7, 1, m_cb7_Camera.GetAddress());
	renderer->m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(7, 1, m_cb7_Camera.GetAddress());

	// ライト：平行光源・点光源・環境光
	m_cb8_Fog.Create();
	m_cb8_Fog.Write();
	renderer->m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(8, 1, m_cb8_Fog.GetAddress());
	renderer->m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(8, 1, m_cb8_Fog.GetAddress());

	// ライト：平行光源・点光源・環境光
	m_cb9_Light.Create();
	m_cb9_Light.Write();
	renderer->m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(9, 1, m_cb9_Light.GetAddress());
	renderer->m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(9, 1, m_cb9_Light.GetAddress());

	//============================================
	// パイプラインステート関係
	//============================================
	//深度ステンシルステート作成（奥行情報の使い方・手前にあるものを無視して描画したりできる
	m_depthStencilStates[(int)KdDepthStencilState::ZEnable] = renderer->m_kdDirect3D->CreateDepthStencilState(true, true);
	m_depthStencilStates[(int)KdDepthStencilState::ZWriteDisable] = renderer->m_kdDirect3D->CreateDepthStencilState(true, false);
	m_depthStencilStates[(int)KdDepthStencilState::ZDisable] = renderer->m_kdDirect3D->CreateDepthStencilState(false, false);

	// 初期深度ステートの設定
	renderer->m_kdDirect3D->WorkDevContext()->OMSetDepthStencilState(m_depthStencilStates[(int)KdDepthStencilState::ZEnable], 0);

	// ラスタライザステート作成（ポリゴンの任意の面の描画を省略できる、処理を軽減する目的で運用する事が多い
	m_rasterizerStates[(int)KdRasterizerState::CullNone] = renderer->m_kdDirect3D->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, true, false);
	m_rasterizerStates[(int)KdRasterizerState::CullFront] = renderer->m_kdDirect3D->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, true, false);
	m_rasterizerStates[(int)KdRasterizerState::CullBack] = renderer->m_kdDirect3D->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, false);

	// 初期ラスタライザステートの設定
	renderer->m_kdDirect3D->WorkDevContext()->RSSetState(m_rasterizerStates[(int)KdRasterizerState::CullNone]);

	// ブレンドステート作成（ピクセルの最終色を決めるときに既に塗られている色と、どう合成するのかの選択ができる
	m_blendStates[(int)KdBlendState::Alpha] = renderer->m_kdDirect3D->CreateBlendState(KdBlendMode::Alpha);
	m_blendStates[(int)KdBlendState::Add] = renderer->m_kdDirect3D->CreateBlendState(KdBlendMode::Add);

	// 初期ブレンドステートの設定
	renderer->m_kdDirect3D->WorkDevContext()->OMSetBlendState(m_blendStates[(int)KdBlendState::Alpha], Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	// サンプラーステート作成（テクスチャのピクセル色を取得する際にどのような補間や扱いで取得するかを選択できる
	m_samplerStates[(int)KdSamplerState::Anisotropic_Wrap] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Wrap, false);
	m_samplerStates[(int)KdSamplerState::Anisotropic_Clamp] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Clamp, false);
	m_samplerStates[(int)KdSamplerState::Linear_Clamp] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Linear, 0, KdSamplerAddressingMode::Clamp, false);
	m_samplerStates[(int)KdSamplerState::Linear_Clamp_Cmp] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Linear, 0, KdSamplerAddressingMode::Clamp, true);
	m_samplerStates[(int)KdSamplerState::Point_Wrap] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Point, 0, KdSamplerAddressingMode::Wrap, false);
	m_samplerStates[(int)KdSamplerState::Point_Clamp] = renderer->m_kdDirect3D->CreateSamplerState(KdSamplerFilterMode::Point, 0, KdSamplerAddressingMode::Clamp, false);

	// 初期サンプラーステートの設定
	if (m_pixelArtStyle)
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetSamplers(0, 1, &m_samplerStates[(int)KdSamplerState::Point_Wrap]);
	}
	else
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetSamplers(0, 1, &m_samplerStates[(int)KdSamplerState::Anisotropic_Wrap]);
	}

	m_ambientController.Init();
}


//==========================
//
// 描画パイプライン系の設定
//
//==========================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 頂点シェーダのセット（現行のシェーダーと同じ場合はキャンセル）
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdShaderManager::SetVertexShader(ID3D11VertexShader* pSetVS)
{
	if (!pSetVS) { return false; }
	auto renderer = GETRENDERER;

	ID3D11VertexShader* pNowVS = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);

	// セットしようとしているシェーダーが現行と同じならキャンセル
	bool needChange = pNowVS != pSetVS;

	if (needChange)
	{
		renderer->m_kdDirect3D->WorkDevContext()->VSSetShader(pSetVS, nullptr, 0);
	}

	KdSafeRelease(pNowVS);

	return needChange;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ピクセルシェーダのセット（現行のシェーダーと同じ場合はキャンセル）
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdShaderManager::SetPixelShader(ID3D11PixelShader* pSetPS)
{
	if (!pSetPS) { return false; }
	auto renderer = GETRENDERER;

	ID3D11PixelShader* pNowPS = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->PSGetShader(&pNowPS, nullptr, nullptr);

	// セットしようとしているシェーダーが現行と同じならキャンセル
	bool needChange = pNowPS != pSetPS;

	if (needChange)
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetShader(pSetPS, nullptr, 0);
	}

	KdSafeRelease(pNowPS);

	return needChange;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 頂点入力レイアウトのセット（現行のレイアウトと同じ場合はキャンセル）
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdShaderManager::SetInputLayout(ID3D11InputLayout* pSetLayout)
{
	if (!pSetLayout) { return false; }
	auto renderer = GETRENDERER;

	ID3D11InputLayout* pNowLayout = nullptr;

	renderer->m_kdDirect3D->WorkDevContext()->IAGetInputLayout(&pNowLayout);

	// セットしようとしているレイアウトが現行と同じならキャンセル
	bool needChange = pNowLayout != pSetLayout;

	if (needChange)
	{
		renderer->m_kdDirect3D->WorkDevContext()->IASetInputLayout(pSetLayout);
	}

	KdSafeRelease(pNowLayout);

	return needChange;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 頂点シェーダーの定数バッファのセット（現行の定数バッファと同じ場合はキャンセル）
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdShaderManager::SetVSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetVSBuffer)
{
	if (!pSetVSBuffer) { return false; }
	auto renderer = GETRENDERER;

	ID3D11Buffer* pNowVSBuffer = nullptr;

	renderer->m_kdDirect3D->WorkDevContext()->VSGetConstantBuffers(startSlot, 1, &pNowVSBuffer);

	// セットしようとしている定数バッファが現行と同じならキャンセル
	bool needChange = pNowVSBuffer != *pSetVSBuffer;

	if (needChange)
	{
		renderer->m_kdDirect3D->WorkDevContext()->VSSetConstantBuffers(startSlot, 1, pSetVSBuffer);
	}

	KdSafeRelease(pNowVSBuffer);

	return needChange;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ピクセルシェーダーの定数バッファのセット（現行の定数バッファと同じ場合はキャンセル）
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdShaderManager::SetPSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetPSBuffer)
{
	if (!pSetPSBuffer) { return false; }
	auto renderer = GETRENDERER;

	ID3D11Buffer* pNowPSBuffer = nullptr;

	renderer->m_kdDirect3D->WorkDevContext()->PSGetConstantBuffers(startSlot, 1, &pNowPSBuffer);

	bool needChange = pNowPSBuffer != *pSetPSBuffer;

	if (needChange)
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetConstantBuffers(startSlot, 1, pSetPSBuffer);
	}

	KdSafeRelease(pNowPSBuffer);

	return needChange;
}


//==========================
//
// パイプラインステートの変更
//
//==========================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 深度ステンシルステートの変更（現行と同じステートの場合はキャンセル
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::ChangeDepthStencilState(KdDepthStencilState stateId)
{
	auto renderer = GETRENDERER;

	ID3D11DepthStencilState* pNowDs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->OMGetDepthStencilState(&pNowDs, 0);

	if (pNowDs != m_depthStencilStates[(int)stateId])
	{
		renderer->m_kdDirect3D->WorkDevContext()->OMSetDepthStencilState(m_depthStencilStates[(int)stateId], 0);
	}

	m_ds_Undo.push(pNowDs);

	KdSafeRelease(pNowDs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 深度ステンシルステートを変更直前のステートに戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::UndoDepthStencilState()
{
	if (!m_ds_Undo.size()) { return; }
	auto renderer = GETRENDERER;

	ID3D11DepthStencilState* pNowDs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->OMGetDepthStencilState(&pNowDs, 0);

	if (pNowDs != m_ds_Undo.top())
	{
		renderer->m_kdDirect3D->WorkDevContext()->OMSetDepthStencilState(m_ds_Undo.top(), 0);
	}

	m_ds_Undo.pop();

	KdSafeRelease(pNowDs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ラスタライザステートの変更（現行と同じステートの場合はキャンセル
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::ChangeRasterizerState(KdRasterizerState stateId)
{
	auto renderer = GETRENDERER;

	ID3D11RasterizerState* pNowRs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->RSGetState(&pNowRs);

	if (pNowRs != m_rasterizerStates[(int)stateId])
	{
		renderer->m_kdDirect3D->WorkDevContext()->RSSetState(m_rasterizerStates[(int)stateId]);
	}

	m_rs_Undo.push(pNowRs);

	KdSafeRelease(pNowRs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ラスタライザステンシルステートを変更直前のステートに戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::UndoRasterizerState()
{
	if (!m_rs_Undo.size()) { return; }
	auto renderer = GETRENDERER;

	ID3D11RasterizerState* pNowRs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->RSGetState(&pNowRs);

	if (pNowRs != m_rs_Undo.top())
	{
		renderer->m_kdDirect3D->WorkDevContext()->RSSetState(m_rs_Undo.top());
	}

	m_rs_Undo.pop();

	KdSafeRelease(pNowRs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ブレンドステートの変更（現行と同じステートの場合はキャンセル
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::ChangeBlendState(KdBlendState stateId)
{
	auto renderer = GETRENDERER;

	ID3D11BlendState* pNowBs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->OMGetBlendState(&pNowBs, nullptr, nullptr);

	if (pNowBs != m_blendStates[(int)stateId])
	{
		renderer->m_kdDirect3D->WorkDevContext()->OMSetBlendState(m_blendStates[(int)stateId], Math::Color(1, 0, 0, 1), 0xFFFFFFFF);
	}

	m_bs_Undo.push(pNowBs);

	KdSafeRelease(pNowBs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ブレンドステートを変更直前のステートに戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::UndoBlendState()
{
	if (!m_bs_Undo.size()) { return; }
	auto renderer = GETRENDERER;

	ID3D11BlendState* pNowBs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->OMGetBlendState(&pNowBs, nullptr, nullptr);

	if (pNowBs != m_bs_Undo.top())
	{
		renderer->m_kdDirect3D->WorkDevContext()->OMSetBlendState(m_bs_Undo.top(), Math::Color(0, 0, 0, 0), 0xFFFFFFFF);
	}

	m_bs_Undo.pop();

	KdSafeRelease(pNowBs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// サンプラーステートの変更（現行と同じステートの場合はキャンセル
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::ChangeSamplerState(KdSamplerState stateId, int slot)
{
	auto renderer = GETRENDERER;

	ID3D11SamplerState* pNowSs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->PSGetSamplers(slot, 1, &pNowSs);

	if (pNowSs != m_samplerStates[(int)stateId])
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetSamplers(slot, 1, &m_samplerStates[(int)stateId]);
	}

	m_ss_Undo.push(pNowSs);

	KdSafeRelease(pNowSs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// サンプラーステートを変更直前のステートに戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::UndoSamplerState(int slot)
{
	if (!m_ss_Undo.size()) { return; }
	auto renderer = GETRENDERER;

	ID3D11SamplerState* pNowSs = nullptr;
	renderer->m_kdDirect3D->WorkDevContext()->PSGetSamplers(0, 1, &pNowSs);

	if (pNowSs != m_ss_Undo.top())
	{
		renderer->m_kdDirect3D->WorkDevContext()->PSSetSamplers(slot, 1, &m_ss_Undo.top());
	}

	m_ss_Undo.pop();

	KdSafeRelease(pNowSs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// カメラ情報をGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBCamera(const Math::Matrix& cam, const Math::Matrix& proj)
{
	cbCamera& camera = m_cb7_Camera.Work();

	camera.mView = cam.Invert();

	camera.mProj = proj;
	camera.mProjInv = proj.Invert();

	camera.CamPos = cam.Translation();

	m_cb7_Camera.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグの有効フラグをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBFogEnable(bool distance, bool height)
{
	cbFog& fog = m_cb8_Fog.Work();

	fog.DistanceFogEnable = distance;
	fog.HeightFogEnable = height;

	m_cb8_Fog.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 距離フォグのデータをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBDistanceFog(const Math::Vector3& col, float density)
{
	cbFog& fog = m_cb8_Fog.Work();

	fog.DistanceFogColor = col;
	fog.DistanceFogDensity = density;

	m_cb8_Fog.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 高さフォグのデータをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBHeightFog(const Math::Vector3& col, float top, float bottom, float beginDistance)
{
	cbFog& fog = m_cb8_Fog.Work();

	fog.HeightFogColor = col;
	fog.HeightFogTopValue = top;
	fog.HeightFogBottomValue = bottom;
	fog.HeightFogBeginDistance = beginDistance;

	m_cb8_Fog.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 環境光のデータをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBAmbientLight(const Math::Vector4& col)
{
	m_cb9_Light.Work().AmbientLight = col;

	m_cb9_Light.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光のデータをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBDirectionalLight(const Math::Vector3& dir, const Math::Vector3& col)
{
	cbLight& light = m_cb9_Light.Work();

	light.DirLight_Dir = dir;
	light.DirLight_Color = col;

	m_cb9_Light.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 影生成エリアのデータをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBShadowArea(const Math::Matrix& proj, float dirLightHeight)
{
	Math::Vector3 lightDir = m_cb9_Light.Get().DirLight_Dir;
	Math::Vector3 lightPos = m_cb7_Camera.Get().CamPos;
	Math::Vector3 upVec = (lightDir == Math::Vector3::Up) ? Math::Vector3::Right : Math::Vector3::Up;

	Math::Matrix shadowVP = DirectX::XMMatrixLookAtLH(lightPos - lightDir * dirLightHeight, lightPos, upVec);

	shadowVP *= proj;

	m_cb9_Light.Work().DirLight_mVP = shadowVP;

	m_cb9_Light.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 点光源の配列データをGPUに転送
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdShaderManager::WriteCBPointLight(const std::list<PointLight>& pointLights)
{
	cbLight& light = m_cb9_Light.Work();

	light.PointLight_Num = pointLights.size();

	UINT pointIndex = 0;

	for (const PointLight& pointlight : pointLights)
	{
		light.PointLights[pointIndex] = pointlight;

		++pointIndex;
	}

	m_cb9_Light.Write();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// パラメータの解放：シェーダー本体・共通の定数バッファ・各パイプラインステート
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
#pragma warning(disable:4239)
void KdShaderManager::Release()
{
	m_StandardShader.Release();
	m_postProcessShader.Release();
	m_spriteShader.Release();

	m_cb7_Camera.Release();
	m_cb8_Fog.Release();
	m_cb9_Light.Release();

	//深度ステンシルステート開放
	for (auto& state : m_depthStencilStates)
	{
		KdSafeRelease(state);
	}

	auto ds = std::stack<ID3D11DepthStencilState*>();
	m_ds_Undo.swap(ds);

	// ラスタライザステート解放
	for (auto& state : m_rasterizerStates)
	{
		KdSafeRelease(state);
	}

	auto rs = std::stack<ID3D11RasterizerState*>();
	m_rs_Undo.swap(rs);

	// ブレンドステート解放
	for (auto& state : m_blendStates)
	{
		KdSafeRelease(state);
	}

	auto bs = std::stack<ID3D11BlendState*>();
	m_bs_Undo.swap(bs);

	// サンプラーステート解放
	for (auto& state : m_samplerStates)
	{
		KdSafeRelease(state);
	}

	auto ss = std::stack<ID3D11SamplerState*>();
	m_ss_Undo.swap(ss);
}
#pragma warning(default:4239)