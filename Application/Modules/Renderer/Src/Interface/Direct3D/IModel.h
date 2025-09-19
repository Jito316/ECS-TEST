#pragma once
class RENDERER_API IModelData
{
public:

	// ノード：モデルを形成するメッシュを扱うための最小単位
	struct Node
	{
		std::string		m_name;				// ノード名

		std::shared_ptr<IMesh>	m_spMesh;	// メッシュ

		Math::Matrix	m_localTransform;			// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;			// 原点からの行列
		Math::Matrix	m_boneInverseWorldMatrix;	// 原点からの逆行列

		int					m_parent = -1;	// 親インデックス
		std::vector<int>	m_children;		// 子供へのインデックスリスト

		//int		m_nodeIndex = -1;		// 先頭から何番目のノードか？
		int		m_boneIndex = -1;			// ボーンノードの時、先頭から何番目のボーンか？

		bool	m_isSkinMesh = false;
	};

	IModelData() = default;
	virtual ~IModelData() = default;

	virtual bool Load(std::string_view filename) = 0;

	//アクセサ
	virtual const std::shared_ptr<IMesh> GetMesh(UINT index) const = 0;

	virtual Node* FindNode(std::string name) = 0;

	// マテリアル配列取得
	virtual const std::vector<std::shared_ptr<IMaterial>>& GetMaterials() const = 0;

	// ノード配列取得
	virtual const std::vector<Node>& GetOriginalNodes() const = 0;

	// アニメーションデータ取得
	virtual const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(std::string_view animName) const = 0;
	virtual const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(UINT index) const = 0;
	virtual const std::vector<std::shared_ptr<JTN::Math::KdAnimationData>> GetAnimations() const = 0;

	// それぞれのノードのインデックスリスト取得
	virtual const std::vector<int>& GetRootNodeIndices() const = 0;
	virtual const std::vector<int>& GetBoneNodeIndices() const = 0;
	virtual const std::vector<int>& GetMeshNodeIndices() const = 0;

	virtual const std::vector<int>& GetDrawMeshNodeIndices() const = 0;
	virtual const std::vector<int>& GetCollisionMeshNodeIndices() const = 0;

	virtual bool IsSkinMesh() = 0;
};

class RENDERER_API IModelWork
{
public:

	// ノード：活動中変化する可能性のあるデータ、検索用の名前
	struct Node
	{
		std::string		m_name;				// ノード名

		Math::Matrix	m_localTransform;	// 直属の親ボーンからの行列
		Math::Matrix	m_worldTransform;	// 原点からの行列

		void copy(const IModelData::Node& rNode)
		{
			m_name = rNode.m_name;

			m_localTransform = rNode.m_localTransform;
			m_worldTransform = rNode.m_worldTransform;
		}
	};

	// コンストラクタ
	IModelWork() = default;
	virtual ~IModelWork() = default;

	// 全身のボーンの行列を計算
	virtual void CalcNodeMatrices() = 0;

	// 有効フラグ
	virtual bool IsEnable() const = 0;
	virtual void SetEnable(bool flag) = 0;

	// ノード検索：文字列
	virtual const IModelData::Node* FindDataNode(std::string_view name) const = 0;
	virtual const Node* FindNode(std::string_view name) const = 0;
	virtual Node* FindWorkNode(std::string_view name) = 0;

	// アクセサ
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	virtual const std::shared_ptr<IModelData> GetData() const = 0;
	// メッシュ取得
	virtual const std::shared_ptr<IMesh> GetMesh(UINT index) const = 0;

	// データノードリスト取得
	virtual const std::vector<IModelData::Node>& GetDataNodes() const = 0;
	// コピーノードリスト取得
	virtual const std::vector<Node>& GetNodes() const = 0;
	virtual std::vector<Node>& WorkNodes() = 0;

	// アニメーションデータ取得
	virtual const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(std::string_view animName) const = 0;
	virtual const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(int index) const = 0;

	// モデル設定：コピーノードの生成
	virtual void SetModelData(const std::shared_ptr<IModelData>& rModel) = 0;
	virtual void SetModelData(std::string_view fileName) = 0;

	virtual bool NeedCalcNodeMatrices() = 0;
};
