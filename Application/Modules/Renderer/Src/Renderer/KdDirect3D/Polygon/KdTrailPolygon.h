#pragma once
#include "KdPolygon.h"
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 帯状ポリゴンクラス
// AddPointによって登録された行列のリストを結ぶように帯状のポリゴンを生成する
// 登録された行列そのままにポリゴンを生成したり、ビルボード処理を施して生成したりできる
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
class KdTrailPolygon
	: public KdPolygon
	, public ITrailPolygon
{
public:
	KdTrailPolygon() {}
	KdTrailPolygon(const std::shared_ptr<ITexture>& spBaseColTex) : KdPolygon(spBaseColTex) {}
	KdTrailPolygon(const std::string& baseColTexName) : KdPolygon(baseColTexName) {}

	// 先頭のポイントを取得
	inline const Math::Matrix* GetTopPoint() const override
	{
		if (m_pointList.size() == 0) { return nullptr; }

		return &m_pointList.front();
	}

	// ポイントを追加
	inline void AddPoint(const Math::Matrix& mat)override
	{
		m_pointList.push_front(mat);

		// トレイルが指定の長さを超えたら昔のポイントから削除していく
		if (m_pointList.size() > m_length)
		{
			m_pointList.pop_back();
		}

		GenerateVertices();
	}

	// 最後尾のポイントを削除
	inline void DelPointBack()override
	{
		m_pointList.pop_back();

		GenerateVertices();
	}

	// 軌跡ポイントを全て削除
	inline void ClearPoints()override
	{
		m_pointList.clear();
	}

	// リストの数を取得
	inline int GetNumPoints() const override
	{
		return (int)m_pointList.size();
	}

	// パターンを設定
	inline void SetPattern(Trail_Pattern pattern) override
	{
		if (m_pattern == pattern) { return; }

		m_pattern = pattern;

		GenerateVertices();
	}

	// 帯状ポリゴンの長さを設定
	inline void SetLength(UINT length) override { m_length = length; }

	const IPolygon* GetPolygon() override { return this; }
	IPolygon* WorkPolygon() override { return this; }
private:

	// 頂点生成関数
	void GenerateVertices();

	// 通常描画頂点リストの作成
	void CreateVerticesWithDefaultPattern();

	// ビルボード描画
	void CreateVerticesWithBillboardPattern();

	// 頂点情報をそのまま繋げてポリゴンを作成
	void CreateVerticesWithVerticesPattern();

	// ポリゴンの生成パターン
	Trail_Pattern m_pattern = Trail_Pattern::eDefault;

	// 軌跡の行列情報
	std::deque<Math::Matrix>	m_pointList;

	// 軌跡の長さ
	UINT			m_length = 20;
};
