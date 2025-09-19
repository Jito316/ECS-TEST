#pragma once
struct KdGLTFModel;

class KdModelData : public IModelData
{
public:
	KdModelData();
	~KdModelData();

	bool Load(std::string_view filename);

	void CreateNodes(const std::shared_ptr<KdGLTFModel>& spGltfModel);									// ノード作成
	void CreateMaterials(const std::shared_ptr<KdGLTFModel>& spGltfModel, const  std::string& fileDir);	// マテリアル作成
	void CreateAnimations(const std::shared_ptr<KdGLTFModel>& spGltfModel);								// アニメーション作成

	//アクセサ
	const std::shared_ptr<IMesh> GetMesh(UINT index) const { return index < m_originalNodes.size() ? m_originalNodes[index].m_spMesh : nullptr; }

	Node* FindNode(std::string name)
	{
		for (auto&& node : m_originalNodes)
		{
			if (node.m_name == name)
			{
				return &node;
			}
		}

		return nullptr;
	}

	// マテリアル配列取得
	const std::vector<std::shared_ptr<IMaterial>>& GetMaterials() const { return m_materials; }

	// ノード配列取得
	const std::vector<Node>& GetOriginalNodes() const { return m_originalNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(std::string_view animName) const;
	const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(UINT index) const;
	const std::vector<std::shared_ptr<JTN::Math::KdAnimationData>> GetAnimations() const;

	// それぞれのノードのインデックスリスト取得
	const std::vector<int>& GetRootNodeIndices() const { return m_rootNodeIndices; }
	const std::vector<int>& GetBoneNodeIndices() const { return m_boneNodeIndices; }
	const std::vector<int>& GetMeshNodeIndices() const { return m_meshNodeIndices; }

	const std::vector<int>& GetDrawMeshNodeIndices() const { return m_drawMeshNodeIndices; }
	const std::vector<int>& GetCollisionMeshNodeIndices() const { return m_collisionMeshNodeIndices; }

	bool IsSkinMesh();

private:
	// 解放
	void Release();

	//マテリアル配列
	std::vector<std::shared_ptr<IMaterial>> m_materials;

	// アニメーションデータリスト
	std::vector<std::shared_ptr<JTN::Math::KdAnimationData>>	m_spAnimations;

	// 全ノード配列
	std::vector<Node>		m_originalNodes;
	// 全ノード中、RootノードのみのIndex配列
	std::vector<int>		m_rootNodeIndices;
	// 全ノード中、ボーンノードのみのIndex配列
	std::vector<int>		m_boneNodeIndices;
	// 全ノード中、メッシュが存在するノードのみのIndexn配列
	std::vector<int>		m_meshNodeIndices;

	// 全ノード中、コリジョンメッシュが存在するノードのみのIndexn配列
	std::vector<int>		m_collisionMeshNodeIndices;
	// 全ノード中、描画するノードのみのIndexn配列
	std::vector<int>		m_drawMeshNodeIndices;
};

class KdModelWork : public IModelWork
{
public:
	// コンストラクタ
	KdModelWork() {}
	KdModelWork(const std::shared_ptr<KdModelData>& spModel) { SetModelData(spModel); }

	~KdModelWork() override {}

	// 全身のボーンの行列を計算
	void CalcNodeMatrices()override;

	// 有効フラグ
	bool IsEnable() const override { return (m_enable && m_spData); }
	void SetEnable(bool flag) override { m_enable = flag; }

	// ノード検索：文字列
	const KdModelData::Node* FindDataNode(std::string_view name) const override;
	const Node* FindNode(std::string_view name) const override;
	Node* FindWorkNode(std::string_view name) override;

	// アクセサ
	// ----- ----- ----- ----- ----- ----- ----- ----- ----- -----
	inline const std::shared_ptr<IModelData> GetData() const override { return m_spData; }
	// メッシュ取得
	inline const std::shared_ptr<IMesh> GetMesh(UINT index) const override { return index >= m_coppiedNodes.size() ? nullptr : GetDataNodes()[index].m_spMesh; }

	// データノードリスト取得
	const std::vector<IModelData::Node>& GetDataNodes() const override { assert(m_spData && "モデルデータが存在しません"); return m_spData->GetOriginalNodes(); }
	// コピーノードリスト取得
	const std::vector<Node>& GetNodes() const override { return m_coppiedNodes; }
	std::vector<Node>& WorkNodes() override { m_needCalcNode = true; return m_coppiedNodes; }

	// アニメーションデータ取得
	const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(std::string_view animName) const { return !m_spData ? nullptr : m_spData->GetAnimation(animName); }
	const std::shared_ptr<JTN::Math::KdAnimationData> GetAnimation(int index) const { return !m_spData ? nullptr : m_spData->GetAnimation(index); }

	// モデル設定：コピーノードの生成
	void SetModelData(const std::shared_ptr<IModelData>& rModel)override;
	void SetModelData(std::string_view fileName)override;

	bool NeedCalcNodeMatrices() override { return m_needCalcNode; }

private:

	// 再帰呼び出し用計算関数
	void recCalcNodeMatrices(int nodeIdx, int parentNodeIdx = -1);

	// 有効
	bool	m_enable = true;

	// モデルデータへの参照
	std::shared_ptr<IModelData>	m_spData = nullptr;

	// 活動中変化する可能性のある全ノードデータのコピー配列
	std::vector<Node>	m_coppiedNodes;

	bool m_needCalcNode = false;
};
