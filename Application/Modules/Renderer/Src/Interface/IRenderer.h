#pragma once
#ifdef RENDERER_DLL_EXPORTS
#define  RENDERER_API __declspec(dllexport)
#else
#define  RENDERER_API __declspec(dllimport)
#endif

class IModelData;
class IModelWork;
class IPolygon;
class ISquarePolygon;
class ITrailPolygon;
class ITexture;
class IDirect3D;
class ICamera;
class IDebugWireFrame;

class RENDERER_API IRenderer : public JTN::Module::IModule
{
public:
	enum DrawType : UINT
	{
		Lit = 1 << 0,
		UnLit = 1 << 1,
		Bright = 1 << 2,
		DepthOfShadow = 1 << 3
	};

	IRenderer() = default;
	virtual ~IRenderer() = default;

	virtual void Draw(const IModelData* _modelData, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType) = 0;
	virtual void Draw(IModelWork* _modelWork, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType) = 0;
	virtual void Draw(const IPolygon* _polygon, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType) = 0;
	virtual void Draw(const ITexture* _texture, const Math::Matrix& _matrix, const Math::Rectangle& _rectangle, const Math::Color& _color) = 0;

	virtual void SetMainCamera(ICamera* _camera) = 0;
	virtual void SetSubCamera(ICamera* _camera) = 0;
	virtual ICamera* GetMainCamera() = 0;
	virtual std::list<ICamera*>& GetSubCameraList() = 0;

	virtual IDebugWireFrame* GetDebugWireFrame() = 0;

	virtual IDirect3D* GetDirect3D() = 0;

	virtual ITexture* CreateTexture() = 0;
	virtual IModelData* CreateModelData() = 0;
	virtual IModelWork* CreateModelWork() = 0;
	virtual ISquarePolygon* CreateSquarePolygon() = 0;
	virtual ITrailPolygon* CreateTrailPolygon() = 0;
	virtual ICamera* CreateCamera() = 0;
};
