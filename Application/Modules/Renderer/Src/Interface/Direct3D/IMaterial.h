#pragma once
class ITexture;
//==========================================================
// シェーダー描画用マテリアル
//==========================================================
class RENDERER_API IMaterial
{
public:
	virtual void SetTextures(const std::shared_ptr<ITexture>& spBaseColTex,
		const std::shared_ptr<ITexture>& spMtRfColTex, const std::shared_ptr<ITexture>& spEmiColTex,
		const std::shared_ptr<ITexture>& spNmlColTex) = 0;

	virtual void SetTextures(const std::string& fileDir, const std::string& baseColName,
		const std::string& mtRfColName, const std::string& emiColName, const std::string& nmlColName) = 0;

	//---------------------------------------
	// 材質データ
	//---------------------------------------
	// 名前
	virtual const std::string& GetName() const = 0;
	virtual void SetName(const std::string& _str) = 0;

	// 基本色
	virtual const std::shared_ptr<ITexture> GetBaseColorTex() const = 0;
	virtual const Math::Vector4& GetBaseColorRate() const = 0;
	virtual void SetBaseColorRate(const Math::Vector4& _baseColorRate) = 0;

	// B:金属性 G:粗さ
	virtual const std::shared_ptr<ITexture>	GetMetallicRoughnessTex() const = 0;
	virtual const float						GetMetallicRate() const = 0;
	virtual void							SetMetallicRate(const float _metallicRate) = 0;
	virtual const float						GetRoughnessRate() const = 0;
	virtual void							SetRoughnessRate(const float _roughnessRate) = 0;

	// 自己発光
	virtual const std::shared_ptr<ITexture>	GetEmissiveTex() const = 0;
	virtual const Math::Vector3& GetEmissiveRate() const = 0;
	virtual void							SetEmissiveRate(const Math::Vector3& _emissiveRate) = 0;

	// 法線マップ
	virtual const std::shared_ptr<ITexture>	GetNormalTex() const = 0;
};
