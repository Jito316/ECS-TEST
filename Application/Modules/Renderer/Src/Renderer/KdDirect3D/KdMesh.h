#pragma once
#include "KdBuffer.h"
//==========================================================
//
// メッシュクラス
//
//==========================================================
class KdMesh : public IMesh
{
public:

	//=================================================
	// 取得・設定
	//=================================================

	// サブセット情報配列を取得
	const std::vector<KdMeshSubset>& GetSubsets() const override { return m_subsets; };

	// 頂点の座標配列を取得
	const std::vector<Math::Vector3>& GetVertexPositions() const override { return m_positions; }
	// 面の配列を取得
	const std::vector<KdMeshFace>& GetFaces() const override { return m_faces; }

	// 軸平行境界ボックス取得
	const DirectX::BoundingBox* GetBoundingBox() const override { return &m_aabb; }
	// 境界球取得
	const DirectX::BoundingSphere* GetBoundingSphere() const override { return &m_bs; }

	// メッシュデータをデバイスへセットする
	void SetToDevice() const override;

	// スキンメッシュ？
	bool IsSkinMesh() const override { return m_isSkinMesh; }

	//=================================================
	// 作成・解放
	//=================================================

	// メッシュ作成
	// ・vertices		… 頂点配列
	// ・faces			… 面インデックス情報配列
	// ・subsets		… サブセット情報配列
	// 戻り値			… 成功：true
	bool Create(const std::vector <KdMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets, bool isSkinMesh);

	// 解放
	void Release();

	KdMesh() {}
	~KdMesh() override {
		Release();
	}

	//=================================================
	// 処理
	//=================================================

	// 指定サブセットを描画
	void DrawSubset(int subsetNo) const override;


private:

	// 頂点バッファ
	KdBuffer					m_vertBuf;
	// インデックスバッファ
	KdBuffer					m_indxBuf;

	// サブセット情報
	std::vector<KdMeshSubset>	m_subsets;

	// 境界データ
	DirectX::BoundingBox		m_aabb;	// 軸平行境界ボックス
	DirectX::BoundingSphere		m_bs;	// 境界球

	// 座標のみの配列(複製)
	std::vector<Math::Vector3>	m_positions;
	// 面情報のみの配列(複製)
	std::vector<KdMeshFace>		m_faces;

	bool						m_isSkinMesh = false;

private:
	// コピー禁止用
	KdMesh(const KdMesh& src) = delete;
	void operator=(const KdMesh& src) = delete;
};
