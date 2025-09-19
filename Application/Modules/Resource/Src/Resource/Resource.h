#pragma once

class AssetManager : public IResource
{
public:


	std::shared_ptr<IModelData> GetModelData(std::string_view _assetPath)override;
	std::shared_ptr<IModelWork> GetModelWork(std::string_view _assetPath)override;

	std::shared_ptr<ITexture> GetTexture(std::string_view _assetPath)override;
	std::shared_ptr<ISquarePolygon> GetSquarePolygon(std::string_view _assetPath)override;
	std::shared_ptr<ITrailPolygon> GetTrailPolygon(std::string_view _assetPath)override;

private:
	bool Entry()override;
	void SetUp();
	void Shutdown();

	std::map<std::string, std::shared_ptr<ITexture>>			m_texList;
	std::map<std::string, std::shared_ptr<IModelData>>			m_modelDataList;

	IRenderer* m_pIRenderer = nullptr;
};

extern"C" RESOUCE_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern"C" RESOUCE_API void DeleteModule(JTN::Module::IModule* _module);