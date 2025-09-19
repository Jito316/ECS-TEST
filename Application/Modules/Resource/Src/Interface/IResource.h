#pragma once
#ifdef RESOUCE_DLL_EXPORTS
#define RESOUCE_API __declspec(dllexport)
#else
#define RESOUCE_API __declspec(dllimport)
#endif

class RESOUCE_API IResource : public JTN::Module::IModule
{
public:
	virtual std::shared_ptr<IModelData> GetModelData(std::string_view _assetPath) = 0;
	virtual std::shared_ptr<IModelWork> GetModelWork(std::string_view _assetPath) = 0;

	virtual std::shared_ptr<ITexture> GetTexture(std::string_view _assetPath) = 0;
	virtual std::shared_ptr<ISquarePolygon> GetSquarePolygon(std::string_view _assetPath) = 0;
	virtual std::shared_ptr<ITrailPolygon> GetTrailPolygon(std::string_view _assetPath) = 0;
};