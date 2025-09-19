#pragma once

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 描画に使用する視野のデータをアプリケーション上で管理するクラス
// 3D描画に使用する各種行列を保持
// 被写界深度（目の焦点をどの距離に合わせるのか：焦点が合っていない距離はぼやける）に必要な情報を保持
// シェーダーへの情報の送信を行う（重要機能）SetToShader関数をもつ
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class RENDERER_API ICamera
{
public:
	enum class Type {
		Main
		, Sub
	};

	// デフォルトの射影行列を作成
	ICamera() = default;
	virtual ~ICamera() = default;

	// ※重要機能：カメラ情報(ビュー・射影行列など)をシェーダへ転送
	virtual void SetToShader() const = 0;

	// カメラ行列の設定・カメラ行列からビュー行列を生成
	virtual void SetCameraMatrix(const Math::Matrix& mCam) = 0;

	// 射影行列の設定
	virtual void SetProjectionMatrix(float fov, float maxRange = 2000, float minRange = 0.01f, float aspectRatio = 0.0f) = 0;
	virtual void SetProjectionMatrix(const Math::Matrix& rProj) = 0;

	// 焦点距離を設定
	virtual void SetFocus(float focusDist, float focusForeRange, float focusBackRange) = 0;

	// カメラ行列取得
	virtual const DirectX::SimpleMath::Matrix& GetCameraMatrix() const = 0;

	// カメラ逆行列取得
	virtual const DirectX::SimpleMath::Matrix& GetCameraViewMatrix() const = 0;

	// 射影行列取得
	virtual const DirectX::SimpleMath::Matrix& GetProjMatrix() const = 0;

	// クライアント座標（2D）から3Dワールド座標を求める用のレイ情報を生成
	virtual void GenerateRayInfoFromClientPos(const POINT& clientPos, Math::Vector3& rayPos, Math::Vector3& rayDir, float& rayRange) = 0;

	// ワールド座標(3D座標)をスクリーン座標(2D座標)に変換する
	virtual void ConvertWorldToScreenDetail(const Math::Vector3& pos, Math::Vector3& result) = 0;

	virtual void Register(Type _type = Type::Main) const = 0;
	virtual std::shared_ptr<ITexture> GetViewTexture() = 0;
	virtual void SetViewTexture(std::shared_ptr<ITexture> _spTex) = 0;
};