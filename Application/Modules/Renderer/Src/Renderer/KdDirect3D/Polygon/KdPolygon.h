#pragma once
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// ポリゴンの共通情報を扱う基底クラス
// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
// それぞれの派生クラスにおいて描画の際にポリゴンを形成する頂点情報を作成する機能を持たせる必要がある
// ポリゴンに反映させるマテリアルの情報を保持している
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class ITexture;

class KdPolygon : public IPolygon
{
public:
	KdPolygon() {}
	KdPolygon(const std::shared_ptr<ITexture>& spBaseColTex) { SetMaterial(spBaseColTex); }
	KdPolygon(const std::string& baseColTexName) { SetMaterial(baseColTexName); }
	virtual ~KdPolygon() override {}

	// 描画時に使用する頂点リストの取得
	const std::vector<Vertex>& GetVertices() const override { return m_vertices; };
	// 判定用ポリゴン頂点座標のコピー
	void GetPositions(std::vector<Math::Vector3>& res) const override;

	// マテリアルの設定
	void SetMaterial(const std::shared_ptr<IMaterial>& spMaterial) override { m_spMaterial = spMaterial; }
	// ベースカラーテクスチャから
	void SetMaterial(const std::shared_ptr<ITexture>& spBaseColTex)override;
	// ベースカラーファイルパスから
	void SetMaterial(const std::string& baseColTexName)override;

	virtual void SetColor(const Math::Color& col) override { if (m_spMaterial) { m_spMaterial->SetBaseColorRate(static_cast<const Math::Vector4>(col)); } }

	// マテリアルを取得
	const std::shared_ptr<IMaterial>& GetMaterial() const override { return m_spMaterial; }

	bool IsEnable() const override { return (m_enable); }
	void SetEnable(bool enable) override { m_enable = enable; }

	bool Is2DObject() const override { return m_2DObject; }
	void Set2DObject(bool is2DObject) override { m_2DObject = is2DObject; }

protected:

	// ポリゴンに描画するテクスチャ
	std::shared_ptr<IMaterial> m_spMaterial = nullptr;

	// 頂点リスト
	std::vector<Vertex>		m_vertices;

	// 描画有効フラグ
	bool m_enable = true;

	// 2Dオブジェクトかどうか
	bool m_2DObject = true;
};
