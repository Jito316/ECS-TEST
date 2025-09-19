#pragma once
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 四角形ポリゴンクラス
// テクスチャの一部分を描画したり、描画する板ポリのサイズを設定できる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class RENDERER_API ITrailPolygon
{
public:
	enum class Trail_Pattern
	{
		eDefault,
		eBillboard,
		eVertices
	};

	ITrailPolygon() {}
	virtual ~ITrailPolygon() {}

	// 先頭のポイントを取得
	virtual const Math::Matrix* GetTopPoint() const = 0;
	// ポイントを追加
	 virtual void AddPoint(const Math::Matrix& mat) = 0;

	// 最後尾のポイントを削除
	virtual void DelPointBack() = 0;
	// 軌跡ポイントを全て削除
	virtual void ClearPoints() = 0;
	// リストの数を取得
	virtual int GetNumPoints() const = 0;
	// パターンを設定
	virtual void SetPattern(Trail_Pattern pattern) = 0;
	// 帯状ポリゴンの長さを設定
	virtual void SetLength(UINT length) = 0;

	virtual const IPolygon* GetPolygon() = 0; 
	virtual IPolygon* WorkPolygon() = 0; 
};
