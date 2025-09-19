#pragma once
class ITexture;

//==========================================================
// シェーダー描画用マテリアル
//==========================================================
class KdMaterial : public IMaterial
{
public:
	virtual void SetTextures(const std::shared_ptr<ITexture>& spBaseColTex,
		const std::shared_ptr<ITexture>& spMtRfColTex, const std::shared_ptr<ITexture>& spEmiColTex,
		const std::shared_ptr<ITexture>& spNmlColTex)override;

	virtual void SetTextures(const std::string& fileDir, const std::string& baseColName,
		const std::string& mtRfColName, const std::string& emiColName, const std::string& nmlColName)override;

	//---------------------------------------
	// 取得
	//---------------------------------------
	// 名前
	const std::string& GetName() const override { return m_name; };
	void SetName(const std::string& _str) override { m_name = _str; };

	// 基本色
	const std::shared_ptr<ITexture> GetBaseColorTex() const override { return m_baseColorTex; };
	const Math::Vector4& GetBaseColorRate() const override { return m_baseColorRate; };
	void SetBaseColorRate(const Math::Vector4& _baseColorRate)override { m_baseColorRate = _baseColorRate; };

	// B:金属性 G:粗さ
	const std::shared_ptr<ITexture> GetMetallicRoughnessTex() const override { return m_metallicRoughnessTex; };
	const float						GetMetallicRate() const override { return m_metallicRate; };
	void						SetMetallicRate(const float _metallicRate) override { m_metallicRate = _metallicRate; }
	const float						GetRoughnessRate() const override { return m_roughnessRate; };
	void						SetRoughnessRate(const float _roughnessRate) override { m_roughnessRate = _roughnessRate; }

	// 自己発光
	const std::shared_ptr<ITexture>	GetEmissiveTex() const override { return m_emissiveTex; };
	const Math::Vector3& GetEmissiveRate() const override { return m_emissiveRate; };

	// 法線マップ
	const std::shared_ptr<ITexture>	GetNormalTex() const override { return m_normalTex; }



	// 自己発光
	void						SetEmissiveRate(const Math::Vector3& _emissiveRate) { m_emissiveRate = _emissiveRate; };

	//---------------------------------------
	// 材質データ
	//---------------------------------------
	// 名前
	std::string					m_name;

	// 基本色
	std::shared_ptr<ITexture>	m_baseColorTex = nullptr;
	Math::Vector4				m_baseColorRate = kWhiteColor;

	// B:金属性 G:粗さ
	std::shared_ptr<ITexture>	m_metallicRoughnessTex = nullptr;
	float						m_metallicRate = 0.0f;
	float						m_roughnessRate = 1.0f;

	// 自己発光
	std::shared_ptr<ITexture>	m_emissiveTex = nullptr;
	Math::Vector3				m_emissiveRate = Math::Vector3::One;

	// 法線マップ
	std::shared_ptr<ITexture>	m_normalTex = nullptr;
};
