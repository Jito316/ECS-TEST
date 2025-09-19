#pragma once

//==========================================================
// メッシュ用 頂点
//==========================================================
struct KdMeshVertex
{
	Math::Vector3	Pos;		// 座標
	Math::Vector2	UV;			// UV
	unsigned int	Color = 0xFFFFFFFF;			// RGBA色(容量削減のため、各色0～255のUINT型)
	Math::Vector3	Normal;		// 法線
	Math::Vector3	Tangent;	// 接線

	std::array<short, 4>	SkinIndexList;		// スキニングIndexリスト
	std::array<float, 4>	SkinWeightList;		// スキニングウェイトリスト
};

//==========================================================
// メッシュ用 面情報
//==========================================================
struct RENDERER_API KdMeshFace
{
	UINT Idx[3];				// 三角形を構成する頂点のIndex
};

//==========================================================
// メッシュ用 サブセット情報
//==========================================================
struct RENDERER_API KdMeshSubset
{
	UINT		MaterialNo = 0;		// マテリアルNo

	UINT		FaceStart = 0;		// 面Index　このマテリアルで使用されている最初の面のIndex
	UINT		FaceCount = 0;		// 面数　FaceStartから、何枚の面が使用されているかの
};

//==========================================================
//
// メッシュクラス
//
//==========================================================
class RENDERER_API IMesh
{
public:
	IMesh() = default;
	virtual ~IMesh() = default;

	//=================================================
	// 取得・設定
	//=================================================

	// サブセット情報配列を取得
	virtual const std::vector<KdMeshSubset>& GetSubsets() const = 0;

	// 頂点の座標配列を取得
	virtual const std::vector<Math::Vector3>& GetVertexPositions() const = 0;
	// 面の配列を取得
	virtual const std::vector<KdMeshFace>& GetFaces() const = 0;

	// 軸平行境界ボックス取得
	virtual const DirectX::BoundingBox* GetBoundingBox() const = 0;
	// 境界球取得
	virtual const DirectX::BoundingSphere* GetBoundingSphere() const = 0;

	// メッシュデータをデバイスへセットする
	virtual void SetToDevice() const = 0;

	// スキンメッシュ？
	virtual bool IsSkinMesh() const = 0;

	/*
	//=================================================
	// 作成・解放
	//=================================================

	// メッシュ作成
	// ・vertices		… 頂点配列
	// ・faces			… 面インデックス情報配列
	// ・subsets		… サブセット情報配列
	// 戻り値			… 成功：true
	virtual bool Create(const std::vector<IMeshVertex>& vertices, const std::vector<KdMeshFace>& faces, const std::vector<KdMeshSubset>& subsets, bool isSkinMesh) = 0;
	*/

	// 解放
	virtual void Release() = 0;

	//=================================================
	// 処理
	//=================================================

	// 指定サブセットを描画
	virtual void DrawSubset(int subsetNo) const = 0;
private:

	// コピー禁止用
	IMesh(const IMesh& src) = delete;
	void operator=(const IMesh& src) = delete;
};
