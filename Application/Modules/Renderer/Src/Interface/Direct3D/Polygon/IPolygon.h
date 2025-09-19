#pragma once
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンの共通情報を扱う基底クラス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// それぞれの派生クラスにおいて描画の際にポリゴンを形成する頂点情報を作成する機能を持たせる必要がある
// ポリゴンに反映させるマテリアルの情報を保持している
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class RENDERER_API IPolygon
{
public:
	struct Vertex
	{
		Math::Vector3 pos;
		Math::Vector2 UV;
		unsigned int  color = 0xFFFFFFFF;
		Math::Vector3 normal = Math::Vector3::Backward;
		Math::Vector3 tangent = Math::Vector3::Left;

		std::array<short, 4> black1 = {};
		std::array<float, 4> black2 = {};
	};

	IPolygon() = default;
	virtual ~IPolygon() = default;

	// 描画時に使用する頂点リストの取得
	virtual const std::vector<Vertex>& GetVertices() const = 0;
	// 判定用ポリゴン頂点座標のコピー
	virtual void GetPositions(std::vector<Math::Vector3>& res) const = 0;

	// マテリアルの設定
	virtual void SetMaterial(const std::shared_ptr<IMaterial>& spMaterial) = 0;
	// ベースカラーテクスチャから
	virtual void SetMaterial(const std::shared_ptr<ITexture>& spBaseColTex) = 0;
	// ベースカラーファイルパスから
	virtual void SetMaterial(const std::string& baseColTexName) = 0;

	virtual void SetColor(const Math::Color& col) = 0;

	// マテリアルを取得
	virtual const std::shared_ptr<IMaterial>& GetMaterial() const = 0;

	virtual bool IsEnable() const = 0;
	virtual void SetEnable(bool enable) = 0;

	virtual bool Is2DObject() const = 0;
	virtual void Set2DObject(bool is2DObject) = 0;
};