#pragma once
#include "KdPolygon.h"
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 四角形ポリゴンクラス
// テクスチャの一部分を描画したり、描画する板ポリのサイズを設定できる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdSquarePolygon 
	: public KdPolygon
	, public ISquarePolygon
{
public:

	KdSquarePolygon() { InitVertices(); }
	KdSquarePolygon(const std::shared_ptr<ITexture>& spBaseColTex) : KdPolygon(spBaseColTex) { InitVertices(); SetLocalScale(1.0f); }
	KdSquarePolygon(const std::string& baseColTexName) : KdPolygon(baseColTexName) { InitVertices(); SetLocalScale(1.0f); }

	~KdSquarePolygon() override {}

	void SetPivot(PivotType pivot)override;

	// 描画の幅と高さの設定
	void SetLocalScale(const Math::Vector2& scale)override;
	void SetLocalScale(float _scalar)override;

	// 頂点の描画色の設定、それぞれの頂点色を指定
	void SetVertexColor(const std::vector<Math::Color>& _vertCols)override;

	// テクスチャ内の描画エリアの設定
	void SetUVRect(UINT index)override;
	void SetUVRect(UINT x, UINT y)override;
	void SetUVRect(const Math::Rectangle& _rect)override;
	void SetUVRect(const Math::Vector2& _minUV, const Math::Vector2& _maxUV)override;

	// テクスチャの分割数を設定
	inline void SetSplit(UINT splitX, UINT splitY)override
	{
		m_splitX = splitX;
		m_splitY = splitY;
	}
	inline void SetSplit(int* _split)override
	{
		m_splitX = *_split;
		_split++;
		m_splitY = *_split;
	}

	UINT GetSplitX() override { return m_splitX; }
	UINT GetSplitY() override { return m_splitY; }

	const IPolygon* GetPolygon() override { return this; };
	IPolygon* WorkPolygon() override { return this; };
private:

	void InitVertices();

	UINT m_splitX = 1;
	UINT m_splitY = 1;

	PivotType m_pivotType = PivotType::Center_Middle;
};
