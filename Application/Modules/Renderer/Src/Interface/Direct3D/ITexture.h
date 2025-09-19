#pragma once
//====================================================
//
// テクスチャクラス
//
//====================================================
class RENDERER_API ITexture
{
public:
	ITexture() = default;
	virtual ~ITexture() = default;

	//====================================================
	//
	// 取得
	//
	//====================================================

	// 画像のアスペクト比取得
	virtual float						GetAspectRatio() const = 0;
	// 画像の幅を取得
	virtual UINT						GetWidth() const = 0;
	// 画像の高さを取得
	virtual UINT						GetHeight() const = 0;
	// 画像の全情報を取得
	virtual const D3D11_TEXTURE2D_DESC* GetInfo() const = 0;
	// ファイルパス取得(Load時のみ)
	virtual const std::string& GetFilepath() const = 0;

	// 画像リソースを取得
	virtual const ID3D11Texture2D* GetResource() const = 0;
	virtual ID3D11Texture2D* WorkResource() const = 0;

	// シェーダリソースビュー取得
	virtual const ID3D11ShaderResourceView* GetSRView() const = 0;
	virtual ID3D11ShaderResourceView* WorkSRView() const = 0;
	// シェーダリソースビューのアドレス取得
	virtual const ID3D11ShaderResourceView* const* GetSRViewAddress() const = 0;
	virtual ID3D11ShaderResourceView* const* WorkSRViewAddress() const = 0;

	// レンダーターゲットビュー取得
	virtual const ID3D11RenderTargetView* GetRTView() const = 0;
	virtual ID3D11RenderTargetView* WorkRTView() const = 0;
	// レンダーターゲットビューのアドレス取得
	virtual const ID3D11RenderTargetView* const* GetRTViewAddress() const = 0;
	virtual ID3D11RenderTargetView* const* WorkRTViewAddress() const = 0;

	// 深度ステンシルビュー取得
	virtual const ID3D11DepthStencilView* GetDSView() const = 0;
	virtual ID3D11DepthStencilView* WorkDSView() const = 0;

	//====================================================
	//
	// 画像ファイルからテクスチャ作成
	//
	//====================================================

	// 画像ファイルを読み込む
	// ・filename		… 画像ファイル名
	// ・renderTarget	… レンダーターゲットビューを生成する(レンダーターゲットにする)
	// ・depthStencil	… 深度ステンシルビューを生成する(Zバッファにする)
	// ・generateMipmap	… ミップマップ生成する？
	virtual bool Load(std::string_view filename, bool renderTarget = false, bool depthStencil = false, bool generateMipmap = true) = 0;

	//====================================================
	//
	// テクスチャ作成
	//
	//====================================================

	// リソースから作成
	// ・pTexture2D	… 画像リソース
	// 戻り値：true … 成功
	virtual bool Create(ID3D11Texture2D* pTexture2D) = 0;

	// desc情報からテクスチャリソースを作成する
	// ・desc		… 作成するテクスチャリソースの情報
	// ・fillData	… バッファに書き込むデータ　nullptrだと書き込みなし
	// 戻り値：true … 成功
	virtual bool Create(const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* fillData = nullptr) = 0;

	// 通常テクスチャとして作成
	// ※テクスチャリソースを作成し、ShaderResourceViewのみを作成します
	// ・w			… 画像の幅(ピクセル)
	// ・h			… 画像の高さ(ピクセル)
	// ・format		… 画像の形式　DXGI_FORMATを使用
	// ・arrayCnt	… 「テクスチャ配列」を使用する場合、その数。1で通常の1枚テクスチャ
	// ・fillData	… バッファに書き込むデータ　nullptrだと書き込みなし
	// 戻り値：true … 成功
	virtual bool Create(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const D3D11_SUBRESOURCE_DATA* fillData = nullptr) = 0;

	// レンダーターゲットテクスチャとして作成
	// ※テクスチャリソースを作成し、ShaderResourceViewのみを作成します
	// ・w            … 画像の幅(ピクセル)
	// ・h            … 画像の高さ(ピクセル)
	// ・format        … 画像の形式　DXGI_FORMATを使用
	// ・arrayCnt    … 「テクスチャ配列」を使用する場合、その数。1で通常の1枚テクスチャ
	// ・fillData    … バッファに書き込むデータ　nullptrだと書き込みなし
	// ・miscFlags    … その他フラグ(キューブマップを作成したい時に必要)
	// 戻り値：true … 成功
	virtual bool CreateRenderTarget(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const D3D11_SUBRESOURCE_DATA* fillData = nullptr, UINT miscFlags = 0) = 0;

	// 深度ステンシルテクスチャ(Zバッファ)として作成
	// ・w            … 画像の幅(ピクセル)
	// ・h            … 画像の高さ(ピクセル)
	// ・format        … 画像の形式　DXGI_FORMATを使用
	// ・arrayCnt    … 「テクスチャ配列」を使用する場合、その数。1で通常の1枚テクスチャ
	// ・fillData    … バッファに書き込むデータ　nullptrだと書き込みなし
	// ・miscFlags    … その他フラグ(キューブマップを作成したい時に必要)
	// 戻り値：true … 成功
	virtual bool CreateDepthStencil(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS, UINT arrayCnt = 1, const D3D11_SUBRESOURCE_DATA* fillData = nullptr, UINT miscFlags = 0) = 0;

	//====================================================
	//
	// ビューから作成
	//
	//====================================================
	// ShaderResourceViewをセットする
	virtual void SetSRView(ID3D11ShaderResourceView* srv) = 0;

	//====================================================
	// 解放
	//====================================================
	virtual void Release() = 0;

private:
	// コピー禁止用
	ITexture(const ITexture& src) = delete;
	void operator=(const ITexture& src) = delete;
};
