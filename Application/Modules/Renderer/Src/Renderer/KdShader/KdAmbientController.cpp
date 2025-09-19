#include "KdAmbientController.h"
#include "Renderer/RenderManager.h"	
#include "KdShaderManager.h"	

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// シェーダーマネージャで設定したシェーダーの初期値を取得してくる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Init()
{
	auto& renderer = *GETRENDERER;
	// 光源のパラメータ
	{
		const KdShaderManager::cbLight& cbLight = renderer.m_kdShaderManager->GetLightCB();

		m_parameter.m_ambientLightColor = cbLight.AmbientLight;

		m_parameter.m_directionalLightDir = cbLight.DirLight_Dir;
		m_parameter.m_directionalLightDir.Normalize();

		m_parameter.m_directionalLightColor = cbLight.DirLight_Color;
	}

	// フォグのパラメータ
	{
		const KdShaderManager::cbFog& cbFog = renderer.m_kdShaderManager->GetFogCB();

		m_parameter.m_distanceFogColor = cbFog.DistanceFogColor;
		m_parameter.m_distanceFogDensity = cbFog.DistanceFogDensity;
		
		m_parameter.m_heightFogColor = cbFog.HeightFogColor;
		m_parameter.m_heightFogTopValue= cbFog.HeightFogTopValue;
		m_parameter.m_heightFogBottomValue = cbFog.HeightFogBottomValue;
		m_parameter.m_heightFogBeginDistance = cbFog.HeightFogBeginDistance;
	}

	// 影用深度バッファ生成情報の初期化
	SetDirLightShadowArea({ 25.0f, 25.0f }, 30.0f);

	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 点光源は毎フレーム登録する形なので0クリアする
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Update()
{
	m_pointLights.clear();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画の直前の更新が終わったタイミングで実施
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::Draw()
{
	// 光のパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteLightParams();

	// フォグのパラメータの反映
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	WriteFogParams();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポイントライトの追加
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::AddPointLight(const Math::Vector3& Color, float Radius, const Math::Vector3& Pos, bool IsBright)
{
	m_pointLights.push_back(PointLight(Color, Radius, Pos, IsBright));
}

void KdAmbientController::AddPointLight(const PointLight& pointLight)
{
	m_pointLights.push_back(pointLight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光影の生成エリアの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLightShadowArea(const Math::Vector2& lightingArea, float dirLightHeight)
{
	m_shadowProj = DirectX::XMMatrixOrthographicLH(lightingArea.x, lightingArea.x, 0, dirLightHeight * 2.0f);

	m_dirLightHeight = dirLightHeight;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 平行光の設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDirLight(const Math::Vector3& dir, const Math::Vector3& col)
{
	m_parameter.m_directionalLightDir = dir;
	m_parameter.m_directionalLightDir.Normalize();

	m_parameter.m_directionalLightColor = col;

	m_dirtyLightDir = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 環境光の設定
// アルファ値の変更でデフォルトの暗さを指定できる。0にした場合は点光源の周囲以外は真っ暗
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetAmbientLight(const Math::Vector4& col)
{
	m_parameter.m_ambientLightColor = col;

	m_dirtyLightAmb = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグの有効/無効をそれぞれ切り替える
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetFogEnable(bool distance, bool height)
{
	GETRENDERER->m_kdShaderManager->WriteCBFogEnable(distance, height);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 距離フォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetDistanceFog(const Math::Vector3& col, float density)
{
	m_parameter.m_distanceFogColor = col;

	m_parameter.m_distanceFogDensity = density;

	m_dirtyFogDist = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 高さフォグの設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::SetheightFog(const Math::Vector3& col, float topValue, float bottomValue, float distance)
{
	m_parameter.m_heightFogColor = col;

	m_parameter.m_heightFogTopValue = topValue;

	m_parameter.m_heightFogBottomValue = bottomValue;
	
	m_parameter.m_heightFogBeginDistance = distance;

	m_dirtyFogHeight = true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 光関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteLightParams()
{
	auto& renderer = *GETRENDERER;
	// 環境光
	if (m_dirtyLightAmb)
	{
		renderer.m_kdShaderManager->WriteCBAmbientLight(m_parameter.m_ambientLightColor);

		m_dirtyLightAmb = false;
	}

	// 平行光
	if (m_dirtyLightDir)
	{
		renderer.m_kdShaderManager->WriteCBDirectionalLight(
			m_parameter.m_directionalLightDir, m_parameter.m_directionalLightColor);

		m_dirtyLightDir = false;
	}

	// 点光源
	if (m_pointLights.size())
	{
		renderer.m_kdShaderManager->WriteCBPointLight(m_pointLights);
	}

	// 影描画エリアの更新
	renderer.m_kdShaderManager->WriteCBShadowArea(m_shadowProj, m_dirLightHeight);
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// フォグ関連パラメータの書き込み
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void KdAmbientController::WriteFogParams()
{
	auto& renderer = *GETRENDERER;
	// 距離フォグ
	if (m_dirtyFogDist)
	{
		renderer.m_kdShaderManager->WriteCBDistanceFog(m_parameter.m_distanceFogColor,
			m_parameter.m_distanceFogDensity);

		m_dirtyFogDist = false;
	}

	// 高さフォグ
	if (m_dirtyFogHeight)
	{
		renderer.m_kdShaderManager->WriteCBHeightFog(m_parameter.m_heightFogColor,
			m_parameter.m_heightFogTopValue, m_parameter.m_heightFogBottomValue,
			m_parameter.m_heightFogBeginDistance);

		m_dirtyFogHeight = false;
	}
}
