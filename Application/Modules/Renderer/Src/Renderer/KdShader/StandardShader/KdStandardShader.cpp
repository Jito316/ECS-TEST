#include "KdStandardShader.h"
#include "Renderer/RenderManager.h"
#include "Renderer/KdDirect3D/KdDirect3D.h"
#include "Renderer/KdShader/KdShaderManager.h"
#include "Renderer/KdDirect3D/KdMaterial.h"
#include "Renderer/KdDirect3D/KdTexture.h"

void KdStandardShader::SetDissolveTexture(ITexture& dissolveMask)
{
	GETRENDERER->m_kdDirect3D->WorkDevContext()->PSSetShaderResources(11, 1, dissolveMask.WorkSRViewAddress());
}

//================================================
// 描画準備
//================================================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影をつけるオブジェクトの描画の直前処理（不透明な物体やキャラクタの板ポリゴン）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// LitShaderで使用するリソースのバッファー設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::BeginLit()
{
	auto& renderer = *GETRENDERER;
	// 頂点シェーダーのパイプライン変更
	if (renderer.m_kdShaderManager->SetVertexShader(m_VS_Lit))
	{
		renderer.m_kdShaderManager->SetInputLayout(m_inputLayout);

		renderer.m_kdShaderManager->SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		renderer.m_kdShaderManager->SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	// ピクセルシェーダーのパイプライン変更
	if (renderer.m_kdShaderManager->SetPixelShader(m_PS_Lit))
	{
		renderer.m_kdShaderManager->SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		renderer.m_kdShaderManager->SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}

	// ボーン情報をセット(スキンメッシュ対応)
	renderer.m_kdShaderManager->SetVSConstantBuffer(3, m_cb3_Bone.GetAddress());

	// シャドウマップのテクスチャをセット
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(10, 1, m_depthMapFromLightRTPack.m_RTTexture->WorkSRViewAddress());

	// 通常テクスチャ用サンプラーのセット
	renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Anisotropic_Wrap, 0);

	// 影ぼかし用の比較機能付きサンプラーのセット
	renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Linear_Clamp_Cmp, 1);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影ありオブジェクトの描画修了
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// 影を書き込む用に使用していたGenDepthFromLightで生成した深度SRVの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::EndLit()
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GETRENDERER->m_kdDirect3D->WorkDevContext()->PSSetShaderResources(10, 1, &pNullSRV);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影をつけないオブジェクトの描画の直前処理（エフェクトや半透明物）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// UnLitShaderで使用するリソースのバッファー設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::BeginUnLit()
{
	auto& renderer = *GETRENDERER;
	if (renderer.m_kdShaderManager->SetVertexShader(m_VS_UnLit))
	{
		renderer.m_kdShaderManager->SetInputLayout(m_inputLayout);

		renderer.m_kdShaderManager->SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		renderer.m_kdShaderManager->SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	if (renderer.m_kdShaderManager->SetPixelShader(m_PS_UnLit))
	{
		renderer.m_kdShaderManager->SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
		renderer.m_kdShaderManager->SetPSConstantBuffer(2, m_cb2_Material.GetAddress());
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 陰影なしオブジェクトの描画終了
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::EndUnLit()
{
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 影を生み出すオブジェクトの情報描画（光を遮る物体）
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダーのパイプライン変更
// GenDepthMapFromLightShaderで使用するリソースのバッファー設定
// 書き込むテクスチャーを深度用の赤一色のテクスチャに切り替え
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::BeginGenerateDepthMapFromLight()
{
	auto& renderer = *GETRENDERER;
	if (renderer.m_kdShaderManager->SetVertexShader(m_VS_GenDepthFromLight))
	{
		renderer.m_kdShaderManager->SetInputLayout(m_inputLayout);

		renderer.m_kdShaderManager->SetVSConstantBuffer(0, m_cb0_Obj.GetAddress());
		renderer.m_kdShaderManager->SetVSConstantBuffer(1, m_cb1_Mesh.GetAddress());
	}

	// ボーン情報をセット(スキンメッシュ対応)
	renderer.m_kdShaderManager->SetVSConstantBuffer(3, m_cb3_Bone.GetAddress());

	if (renderer.m_kdShaderManager->SetPixelShader(m_PS_GenDepthFromLight))
	{
		renderer.m_kdShaderManager->SetPSConstantBuffer(0, m_cb0_Obj.GetAddress());
	}

	m_depthMapFromLightRTPack.ClearTexture(kRedColor);
	m_depthMapFromLightRTChanger.ChangeRenderTarget(m_depthMapFromLightRTPack);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 影を生み出すオブジェクトの描画終了
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// レンダーターゲットを元に戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::EndGenerateDepthMapFromLight()
{
	m_depthMapFromLightRTChanger.UndoRenderTarget();
}


//================================================
// 描画関数
//================================================

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// メッシュを描画
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// メッシュの頂点データや3Dワールド情報・マテリアル情報をシェーダー(GPU)に転送する
// サブセットごとに描画命令を呼び出す：サブセットの個数分処理が重くなる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::DrawMesh(const IMesh* mesh, const Math::Matrix& mWorld, 
	const std::vector<std::shared_ptr<IMaterial>>& materials, const Math::Vector4& colRate, const Math::Vector3& emissive)
{
	if (mesh == nullptr) { return; }

	// メッシュの頂点情報転送
	mesh->SetToDevice();

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = mWorld;
	m_cb1_Mesh.Write();

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルデータの転送
		const IMaterial& material = *materials[mesh->GetSubsets()[subi].MaterialNo];
		WriteMaterial(material, colRate, emissive);

		//-----------------------
		// サブセット描画
		//-----------------------
		mesh->DrawSubset(subi);
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// モデルデータを描画（スタティック(アニメーションをしない)なモデル専用
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュを描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::DrawModel(const IModelData& rModel, const Math::Matrix& mWorld,
	const Math::Color& colRate, const Math::Vector3& emissive)
{
	// オブジェクト単位の情報転送
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	auto& dataNodes = rModel.GetOriginalNodes();

	// 全描画用メッシュノードを描画
	for (auto& nodeIdx : rModel.GetDrawMeshNodeIndices())
	{
		// 描画
		DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), dataNodes[nodeIdx].m_worldTransform * mWorld, 
			rModel.GetMaterials(), colRate, emissive);
	}

	// 定数に変更があった場合は自動的に初期状態に戻す
	if(m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// モデルワークを描画（ダイナミック(アニメーションをする)なモデルに対応
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュをワークの3D行列に従って描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::DrawModel(IModelWork& rModel, const Math::Matrix& mWorld, const Math::Color& colRate, const Math::Vector3& emissive)
{
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<IModelData>& data = rModel.GetData();

	// データがないときはスキップ
	if (data == nullptr) { return; }

	if (rModel.NeedCalcNodeMatrices())
	{
		rModel.CalcNodeMatrices();
	}

	// オブジェクト単位の情報転送
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	// オブジェクト単位の情報転送(スキンメッシュ対応)
	SetIsSkinMeshObj(data->IsSkinMesh());
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	auto& workNodes = rModel.GetNodes();
	auto& dataNodes = data->GetOriginalNodes();

	// スキンメッシュモデルの場合:ボーン情報を書き込み(スキンメッシュ対応)
	if (data->IsSkinMesh())
	{
		// ノード内からボーン情報を取得
		for (auto&& nodeIdx : data->GetBoneNodeIndices())
		{
			if (nodeIdx >= KdStandardShader::maxBoneBufferSize)
			{
				assert(0 && "転送できるボーンの上限数を超えました");
				return;
			}

			auto& dataNode = dataNodes[nodeIdx];
			auto& workNode = workNodes[nodeIdx];

			// ボーン情報からGPUに渡す行列の計算
			m_cb3_Bone.Work().mBones[dataNode.m_boneIndex] =
				dataNode.m_boneInverseWorldMatrix * workNode.m_worldTransform;
			m_cb3_Bone.Write();
		}
	}

	// 全描画用メッシュノードを描画
	for (auto& nodeIdx : data->GetDrawMeshNodeIndices())
	{
		// 描画
		DrawMesh(dataNodes[nodeIdx].m_spMesh.get(), workNodes[nodeIdx].m_worldTransform * mWorld,
			data->GetMaterials(), colRate, emissive);
	}

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンを描画（モデル以外のプログラム上で生成された頂点の集合体
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// データに所属する全ての描画用メッシュをワークの3D行列に従って描画する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::DrawPolygon(const IPolygon& rPolygon, const Math::Matrix& mWorld, const Math::Color& colRate, const Math::Vector3& emissive)
{
	if (!rPolygon.IsEnable()) { return; }

	// ポリゴン描画用の頂点取得
	auto& vertices = rPolygon.GetVertices();

	// 頂点数が3より少なければポリゴンが形成できないので描画不能
	if (vertices.size() < 3) { return; }

	// オブジェクト単位の定数バッファで変更があった場合のみ情報転送
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = mWorld;
	m_cb1_Mesh.Write();

	// マテリアルの転送
	if (rPolygon.GetMaterial())
	{
		WriteMaterial(*rPolygon.GetMaterial(), colRate, emissive);
	}
	else
	{
		WriteMaterial(KdMaterial(), colRate, emissive);
	}

	auto& renderer = *GETRENDERER;
	renderer.m_kdShaderManager->ChangeRasterizerState(KdRasterizerState::CullNone);

	// サンプラーステートの変更:ポリゴンの描画なので、テクスチャの末端が繰り返されると不自然な描画になるため変更が必要
	if (renderer.m_kdShaderManager->IsPixelArtStyle())
	{
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Point_Clamp);
	}
	else
	{
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Anisotropic_Clamp);
	}

	// 描画パイプラインのチェック
	ID3D11VertexShader* pNowVS = nullptr;
	renderer.m_kdDirect3D->WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);
	bool isLitShader = m_VS_Lit == pNowVS;
	KdSafeRelease(pNowVS);

	// 陰影ありのシェーダーで2Dオブジェクトを描画する時
	if (isLitShader && rPolygon.Is2DObject())
	{
		std::vector<IPolygon::Vertex> _2DVertices = vertices;

		// ポリゴンの法線を光に向ける処理：どの方向に向いていても光の影響を正面からに受けるように変換
		ConvertNormalsFor2D(_2DVertices, mWorld);

		// 2DObject用に変換した頂点配列を描画
		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, _2DVertices.size(), &_2DVertices[0], sizeof(IPolygon::Vertex));
	}
	else
	{
		// 頂点配列を描画
		renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertices.size(), &vertices[0], sizeof(IPolygon::Vertex));
	}

	renderer.m_kdShaderManager->UndoSamplerState();

	renderer.m_kdShaderManager->UndoRasterizerState();

	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

void KdStandardShader::DrawVertices(const std::vector<IPolygon::Vertex>& vertices, const Math::Matrix& mWorld,
	const Math::Color& colRate)
{
	// 頂点数が2より少なければポリゴンが形成できないので描画不能
	if (vertices.size() < 2) { return; }

	// オブジェクト単位の定数バッファで変更があった場合のみ情報転送
	if (m_dirtyCBObj)
	{
		m_cb0_Obj.Write();
	}

	// 3Dワールド行列転送
	m_cb1_Mesh.Work().mW = mWorld;
	m_cb1_Mesh.Write();

	// マテリアルの転送
	WriteMaterial(KdMaterial(), colRate, Math::Vector3::Zero);

	auto& renderer = *GETRENDERER;
	renderer.m_kdShaderManager->ChangeRasterizerState(KdRasterizerState::CullNone);
	renderer.m_kdShaderManager->ChangeDepthStencilState(KdDepthStencilState::ZDisable);

	// サンプラーステートの変更:ポリゴンの描画なので、テクスチャの末端が繰り返されると不自然な描画になるため変更が必要
	if (renderer.m_kdShaderManager->IsPixelArtStyle())
	{
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Point_Clamp);
	}
	else
	{
		renderer.m_kdShaderManager->ChangeSamplerState(KdSamplerState::Anisotropic_Clamp);
	}

	// 描画パイプラインのチェック
	ID3D11VertexShader* pNowVS = nullptr;
	renderer.m_kdDirect3D->WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);

	KdSafeRelease(pNowVS);

	// 頂点配列を描画
	renderer.m_kdDirect3D->DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINELIST, vertices.size(), &vertices[0], sizeof(IPolygon::Vertex));

	renderer.m_kdShaderManager->UndoSamplerState();

	renderer.m_kdShaderManager->UndoDepthStencilState();

	renderer.m_kdShaderManager->UndoRasterizerState();
	// 定数に変更があった場合は自動的に初期状態に戻す
	if (m_dirtyCBObj)
	{
		ResetCBObject();
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// KdShaderManagerの初期化時に呼び出される
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// シェーダー本体の生成
// シェーダーで利用する定数バッファの生成
// 影用の光からの深度情報テクスチャを生成
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool KdStandardShader::Init()
{
	auto& renderer = *GETRENDERER;
	//-------------------------------------
	// 頂点シェーダ(スキンメッシュ対応)
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_VS_Lit.shaderInc"

		// 頂点シェーダー作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_Lit))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKININDEX",0, DXGI_FORMAT_R16G16B16A16_UINT,	    0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKINWEIGHT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点入力レイアウト作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateInputLayout(
			&layout[0],				// 入力エレメント先頭アドレス
			layout.size(),			// 入力エレメント数
			&compiledBuffer[0],		// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),	// 上記のバッファサイズ
			&m_inputLayout))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}

	{
#include "KdStandardShader_VS_GenDepthMapFromLight.shaderInc"

		// 頂点シェーダー作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_GenDepthFromLight))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}
	}

	{
#include "KdStandardShader_VS_UnLit.shaderInc"

		// 頂点シェーダー作成
		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_UnLit))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
#include "KdStandardShader_PS_Lit.shaderInc"

		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS_Lit))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	{
#include "KdStandardShader_PS_GenDepthMapFromLight.shaderInc"

		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS_GenDepthFromLight))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	} 
	
	{
#include "KdStandardShader_PS_UnLit.shaderInc"

		if (FAILED(renderer.m_kdDirect3D->WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS_UnLit))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}
	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0_Obj.Create();
	m_cb1_Mesh.Create();
	m_cb2_Material.Create();
	// スキンメッシュ対応
	m_cb3_Bone.Create();

	std::shared_ptr<ITexture> ds = std::make_shared<KdTexture>();
	ds->CreateDepthStencil(1024, 1024);
	D3D11_VIEWPORT vp = {
		0.0f, 0.0f,
		static_cast<float>(ds->GetWidth()),
		static_cast<float>(ds->GetHeight()),
		0.0f, 1.0f };

	m_depthMapFromLightRTPack.CreateRenderTarget(1024, 1024, true, DXGI_FORMAT_R32_FLOAT);
	m_depthMapFromLightRTPack.ClearTexture(kRedColor);

	auto temp = std::make_shared<KdTexture>();
	temp->Load("Asset/Textures/System/WhiteNoise.png");
	SetDissolveTexture(*temp);


	return true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// シェーダー本体の解放
// 利用していたコンスタントバッファの解放
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::Release()
{
	KdSafeRelease(m_VS_Lit);
	KdSafeRelease(m_VS_GenDepthFromLight);
	KdSafeRelease(m_VS_UnLit);

	KdSafeRelease(m_inputLayout);
	
	KdSafeRelease(m_PS_Lit);
	KdSafeRelease(m_PS_GenDepthFromLight);
	KdSafeRelease(m_PS_UnLit);

	m_cb0_Obj.Release();
	m_cb1_Mesh.Release();
	m_cb2_Material.Release();
	// スキンメッシュ対応
	m_cb3_Bone.Release();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画用マテリアル情報の転送
// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
// それぞれのマテリアルの影響倍率値とテクスチャを設定
// BaseColor：基本色 / Emissive：自己発光色 / Metalic：金属性(テカテカ) / Roughness：粗さ(材質の色の反映度)
// テクスチャは法線マップ以外は未設定なら白1ピクセルのシステムテクスチャを指定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::WriteMaterial(const IMaterial& material, const Math::Vector4& colRate, const Math::Vector3& emiRate)
{
	//-----------------------
	// マテリアル情報を定数バッファへ書き込む
	//-----------------------
	m_cb2_Material.Work().BaseColor = material.GetBaseColorRate() * colRate;
	m_cb2_Material.Work().Emissive = material.GetEmissiveRate() * emiRate;
	m_cb2_Material.Work().Metallic = material.GetMetallicRate();
	m_cb2_Material.Work().Roughness = material.GetRoughnessRate();
	m_cb2_Material.Write();

	//-----------------------
	// テクスチャセット
	//-----------------------
	ID3D11ShaderResourceView* srvs[4];
	auto& renderer = *GETRENDERER;

	srvs[0] = material.GetBaseColorTex() ? material.GetBaseColorTex()->WorkSRView() : renderer.m_kdDirect3D->GetWhiteTex()->WorkSRView();
	srvs[1] = material.GetMetallicRoughnessTex() ? material.GetMetallicRoughnessTex()->WorkSRView() : renderer.m_kdDirect3D->GetWhiteTex()->WorkSRView();
	srvs[2] = material.GetEmissiveTex() ? material.GetEmissiveTex()->WorkSRView() : renderer.m_kdDirect3D->GetWhiteTex()->WorkSRView();
	srvs[3] = material.GetNormalTex() ? material.GetNormalTex()->WorkSRView() : renderer.m_kdDirect3D->GetNormalTex()->WorkSRView();

	// セット
	renderer.m_kdDirect3D->WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンがどの方向に向いていても光の影響を正面からに受けるように頂点の法線を変換
// 2Dキャラクタを描画する時などは必要
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::ConvertNormalsFor2D(std::vector<IPolygon::Vertex>& target, const Math::Matrix& mWorld)
{
	// 平行光の向き
	const Math::Vector3& dirLight_Dir = GETRENDERER->m_kdShaderManager->GetLightCB().DirLight_Dir;

	// どの角度を向いていても表面は常に光の方向を向いている状態：横向きの板ポリが暗くならない対策
	Math::Vector3 normal = Math::Vector3::TransformNormal(-dirLight_Dir, mWorld.Invert());
	Math::Vector3 tangent = (normal != Math::Vector3::Up) ?
		normal.Cross(Math::Vector3::Up) : normal.Cross(Math::Vector3::Right);

	for (size_t i = 0; i < target.size(); ++i)
	{
		target[i].normal = normal;
		target[i].tangent = tangent;
	}
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// オブジェクト定数バッファを初期状態に戻す
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdStandardShader::ResetCBObject()
{
	m_cb0_Obj.Work() = cbObject();

	m_cb0_Obj.Write();

	m_dirtyCBObj = false;
}
