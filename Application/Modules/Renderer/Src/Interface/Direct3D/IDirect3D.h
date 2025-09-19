#pragma once
class ITexture;

// サンプラステート
enum class KdSamplerFilterMode {
	Point,			// ポイントフィルタ
	Linear,			// 線形フィルタ
	Anisotropic,	// 異方性フィルタ
};

enum class KdSamplerAddressingMode {
	Wrap,			// ラップモード
	Clamp,			// クランプモード
};

// ブレンドモード用定数
enum class KdBlendMode {
	NoBlend,		// ブレンドをしない
	Alpha,			// 半透明ブレンド
	Add				// 加算ブレンド
};

class RENDERER_API IDirect3D
{
public:
	IDirect3D() = default;
	virtual ~IDirect3D() = default;

	//==============================================================
	//
	// 取得
	//
	//==============================================================

	// Direct3Dデバイス取得
	virtual const ID3D11Device* GetDev() = 0;
	virtual ID3D11Device* WorkDev() = 0;

	// Direct3Dデバイスコンテキスト取得
	virtual const ID3D11DeviceContext* GetDevContext() = 0;
	virtual ID3D11DeviceContext* WorkDevContext() = 0;

	// SwapChain取得
	virtual const IDXGISwapChain* GetSwapChain() = 0;
	virtual IDXGISwapChain* WorkSwapChain() = 0;

	// バックバッファ取得
	virtual const std::shared_ptr<ITexture> GetBackBuffer() = 0;
	virtual std::shared_ptr<ITexture>		 WorkBackBuffer() = 0;

	// Zバッファ取得
	virtual const std::shared_ptr<ITexture> GetZBuffer() = 0;
	virtual std::shared_ptr<ITexture>		 WorkZBuffer() = 0;

	// ビューポート取得
	virtual void						CopyViewportInfo(Math::Viewport& out) const = 0;

	// 1x1 白テクスチャ取得
	virtual const std::shared_ptr<ITexture>	GetWhiteTex() const = 0;

	// 1x1 Z向き法線マップ取得
	virtual const std::shared_ptr<ITexture>	GetNormalTex() const = 0;

	// スクリーン座標から3D座標への変換
	virtual void ClientToWorld(const POINT& screenPos, float porjZ, Math::Vector3& dst, const Math::Matrix& mCam, const Math::Matrix& mProj) = 0;
	virtual void WorldToClient(const Math::Vector3& srcWorld, POINT& dst, const Math::Matrix& mCam, const Math::Matrix& mProj) = 0;

	//==============================================================
	//
	// 設定
	//
	//==============================================================
	// フルスクリーン設定
	virtual HRESULT SetFullscreenState(bool isFullscreen, IDXGIOutput* pTarget) = 0;

	//==============================================================
	//
	// 初期化・解放
	//
	//==============================================================

	// 初期化
	// ・hWnd			… ウィンドウのハンドル
	// ・w				… X解像度
	// ・h				… Y解像度
	// ・debugDevice	… デバッグモード
	// ・errMsg			… (出力)エラーメッセージが入ってくる
	virtual bool Init(HWND hWnd, int w, int h, bool deviceDebug, std::string& errMsg) = 0;

	// 解放
	virtual void Release() = 0;

	//==============================================================
	//
	// パイプラインステート作成関数
	//
	//==============================================================

	// 深度ステンシルステート作成
	// ・device				… D3Dデバイス
	// ・zEnable			… 深度テスト有効
	// ・zWriteEnable		… 深度書き込み有効
	virtual ID3D11DepthStencilState* CreateDepthStencilState(bool zEnable, bool zWriteEnable) const = 0;

	// ラスタライザーステート作成
	// ・device				… D3Dデバイス
	// ・cullMode			… カリングモード
	// ・fillMode			… 三角形の描画モード
	// ・depthClipEnable	… Zクリッピングを有効にする
	// ・scissorEnable		… 切り抜き範囲を有効にする
	virtual ID3D11RasterizerState* CreateRasterizerState(int cullMode, int fillMode, bool depthClipEnable, bool scissorEnable) const = 0;

	// サンプラーステート作成
	// ・device				… D3Dデバイス
	// ・filterType			… 0:補間なし 1:線形フィルタ 2:異方性フィルタ
	// ・maxAnisotropy		… 異方性フィルタ時の最大値　2, 4, 6, 8, 10, 12, 14, 16 のいずれか
	// ・addressingMode		… テクスチャアドレッシングモード 0:Wrap 1:Clamp
	// ・comparisonModel	… 比較モードON　シャドウマッピングなどで使用する
	virtual ID3D11SamplerState* CreateSamplerState(KdSamplerFilterMode filterType, UINT maxAnisotropy, KdSamplerAddressingMode addressingMode, bool comparisonModel) const = 0;

	// ブレンドステート作成
	// ・device				… D3Dデバイス
	// ・mode				… 合成モード
	virtual ID3D11BlendState* CreateBlendState(KdBlendMode mode) const = 0;

	//==============================================================

	// 頂点を描画する簡易的な関数
	// ・topology		… 頂点をどのような形状で描画するか　D3D_PRIMITIVE_TOPOLOGYマクロを使用
	// ・vertexCount	… 頂点数
	// ・pVertexStream	… 頂点配列の先頭アドレス
	// ・stride			… １頂点のバイトサイズ
	virtual void DrawVertices(int topology, int vertexCount, const void* pVertexStream, UINT stride) = 0;

	//==============================================================

	virtual void SetBackBufferColor(const Math::Color& col) = 0;
	virtual void ClearBackBuffer() = 0;

	//書き込み主を元に戻す奴
	virtual void SetBackBuffer() = 0;
};
