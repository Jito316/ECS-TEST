#include "Resource.h"

std::shared_ptr<ITexture> AssetManager::GetTexture(std::string_view _assetPath)
{
	auto it = m_texList.find(_assetPath.data());
	if (it == m_texList.end()) {
		auto tex = std::shared_ptr<ITexture>(m_pIRenderer->CreateTexture());
		if (tex->Load(_assetPath)) {
			m_texList[_assetPath.data()] = tex;
			return tex;
		}

		return nullptr;
	}
	return it->second;
}

std::shared_ptr<ISquarePolygon> AssetManager::GetSquarePolygon(std::string_view _assetPath)
{
	std::shared_ptr<ITexture> tex = GetTexture(_assetPath);
	auto poly = std::shared_ptr<ISquarePolygon>(m_pIRenderer->CreateSquarePolygon());
	poly->WorkPolygon()->SetMaterial(tex);
	if (poly->GetPolygon()->IsEnable())
	{
		return poly;
	}

	return nullptr;
}

std::shared_ptr<ITrailPolygon> AssetManager::GetTrailPolygon(std::string_view _assetPath)
{
	std::shared_ptr<ITexture> tex = GetTexture(_assetPath);
	auto poly = std::shared_ptr<ITrailPolygon>(m_pIRenderer->CreateTrailPolygon());
	poly->WorkPolygon()->SetMaterial(tex);
	if (poly->GetPolygon()->IsEnable())
	{
		return poly;
	}

	return nullptr;
}

std::shared_ptr<IModelData> AssetManager::GetModelData(std::string_view _assetPath)
{
	std::map<std::string, std::shared_ptr<IModelData>>::iterator it = m_modelDataList.find(_assetPath.data());
	if (it == m_modelDataList.end())
	{
		auto modelData = std::shared_ptr<IModelData>(m_pIRenderer->CreateModelData());
		if (modelData->Load(_assetPath))m_modelDataList[_assetPath.data()] = modelData;
		else
		{
			return nullptr;
		}

		return modelData;
	}

	return  it->second;
}

std::shared_ptr<IModelWork> AssetManager::GetModelWork(std::string_view _assetPath)
{
	std::shared_ptr<IModelData> data = GetModelData(_assetPath.data());
	if (!data)return nullptr;
	std::shared_ptr<IModelWork> work = std::shared_ptr<IModelWork>(m_pIRenderer->CreateModelWork());
	work->SetModelData(data);
	if (work->IsEnable())
	{
		return work;
	}

	return nullptr;
}

bool AssetManager::Entry()
{
	m_pIRenderer = GETMODULE(IRenderer);
	return m_pIRenderer;
}

void AssetManager::SetUp()
{

}

void AssetManager::Shutdown()
{
	m_texList.clear();
	m_modelDataList.clear();
}


extern"C" RESOUCE_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress)
{
	*_pModuleAdress = new AssetManager;
	*_pInterfaceInfoAdress = &typeid(IResource);
}

extern"C" RESOUCE_API void DeleteModule(JTN::Module::IModule* _module)
{
	delete _module;
}