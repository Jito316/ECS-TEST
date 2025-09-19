#pragma once
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 四角形ポリゴンクラス
// テクスチャの一部分を描画したり、描画する板ポリのサイズを設定できる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class RENDERER_API ISquarePolygon
{
public:
	// ポリゴンの原点の位置
	enum class PivotType
	{
		Right_Top,		// 右上
		Right_Middle,	// 右中段
		Right_Bottom,	// 右下
		Center_Top = 10,// 中央上
		Center_Middle,	// 中央中段
		Center_Bottom,	// 中央下
		Left_Top = 20,	// 左上
		Left_Middle,	// 左中段
		Left_Bottom,	// 左下
	};

	ISquarePolygon() = default;
	virtual ~ISquarePolygon() = default;

	virtual void SetPivot(PivotType pivot) = 0;

	// 描画の幅と高さの設定
	virtual void SetLocalScale(const Math::Vector2& scale) = 0;
	virtual void SetLocalScale(float _scalar) = 0;

	// 頂点の描画色の設定、それぞれの頂点色を指定
	virtual void SetVertexColor(const std::vector<Math::Color>& _vertCols) = 0;

	// テクスチャ内の描画エリアの設定
	virtual void SetUVRect(UINT index) = 0;
	virtual void SetUVRect(UINT x, UINT y) = 0;
	virtual void SetUVRect(const Math::Rectangle& _rect) = 0;
	virtual void SetUVRect(const Math::Vector2& _minUV, const Math::Vector2& _maxUV) = 0;

	// テクスチャの分割数を設定
	virtual void SetSplit(UINT splitX, UINT splitY) = 0;
	virtual void SetSplit(int* _split) = 0;

	virtual UINT GetSplitX() = 0;
	virtual UINT GetSplitY() = 0;

	virtual const IPolygon* GetPolygon() = 0;
	virtual IPolygon* WorkPolygon() = 0;
};
