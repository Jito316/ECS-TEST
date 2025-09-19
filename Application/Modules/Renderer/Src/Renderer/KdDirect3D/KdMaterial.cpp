#include "KdMaterial.h"
#include "Renderer/Utility/KdDataStorage.h"
#include "KdTexture.h"

void KdMaterial::SetTextures(const std::shared_ptr<ITexture>& spBaseColTex, const std::shared_ptr<ITexture>& spMtRfColTex,
	const std::shared_ptr<ITexture>& spEmiColTex, const std::shared_ptr<ITexture>& spNmlColTex)
{
	m_baseColorTex = spBaseColTex;
	m_metallicRoughnessTex = spMtRfColTex;
	m_emissiveTex = spEmiColTex;
	m_normalTex = spNmlColTex;

	if (spMtRfColTex)
	{
		m_metallicRate = 1.0f;
		m_roughnessRate = 1.0f;
	}
}

void KdMaterial::SetTextures(const std::string& fileDir, const std::string& baseColName,
	const std::string& mtRfColName, const std::string& emiColName, const std::string& nmlColName)
{
	std::shared_ptr<ITexture>	BaseColorTex = nullptr;
	std::shared_ptr<ITexture>	MetallicRoughnessTex = nullptr;
	std::shared_ptr<ITexture>	EmissiveTex = nullptr;
	std::shared_ptr<ITexture>	NormalTex = nullptr;

	auto& renderer = *GETRENDERER;
	// 基本色テクスチャ
	if (!baseColName.empty() && KdFileExistence(fileDir + baseColName))
	{
		BaseColorTex = renderer.m_kdAssets->m_textures.GetData(fileDir + baseColName);
	}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 金属性・粗さマップ
	if (!mtRfColName.empty() && KdFileExistence(fileDir + mtRfColName))
	{
		MetallicRoughnessTex = renderer.m_kdAssets->m_textures.GetData(fileDir + mtRfColName);

	}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 自己発光・エミッシブマップ
	if (!emiColName.empty() && KdFileExistence(fileDir + emiColName))
	{
		EmissiveTex = renderer.m_kdAssets->m_textures.GetData(fileDir + emiColName);
	}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 法線マップ
	if (!nmlColName.empty() && KdFileExistence(fileDir + nmlColName))
	{
		NormalTex = renderer.m_kdAssets->m_textures.GetData(fileDir + nmlColName);
	}

	SetTextures(BaseColorTex, MetallicRoughnessTex, EmissiveTex, NormalTex);
}
