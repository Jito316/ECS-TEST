#pragma once
class KdDebugWireFrame;
class KdDirect3D;
class KdShaderManager;
struct KdRenderTargetChanger;
struct KdRenderTargetPack;
class KdAssets;

class RenderManager :public IRenderer
{
public:
	struct Asset2DPackBase;
	struct Asset3DPackBase;

	void Draw(const IModelData* _modelData, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)override;
	void Draw(IModelWork* _modelWork, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)override;
	void Draw(const IPolygon* _polygon, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)override;
	void Draw(const ITexture* _texture, const Math::Matrix& _matrix, const Math::Rectangle& _rectangle, const Math::Color& _color)override;

	IDirect3D* GetDirect3D()override;

	void SetMainCamera(ICamera* _camera)override;
	void SetSubCamera(ICamera* _camera)override;
	ICamera* GetMainCamera()override;
	std::list<ICamera*>& GetSubCameraList()override;

	IDebugWireFrame* GetDebugWireFrame()override;

	ITexture* CreateTexture()override;
	IModelData* CreateModelData()override;
	IModelWork* CreateModelWork()override;
	ISquarePolygon* CreateSquarePolygon()override;
	ITrailPolygon* CreateTrailPolygon()override;
	ICamera* CreateCamera()override;

	KdDirect3D* m_kdDirect3D = nullptr;
	KdShaderManager* m_kdShaderManager = nullptr;
	KdAssets* m_kdAssets = nullptr;
private:

	bool Entry()override;

	void SetUp();
	void Shutdown();
	void Update();

	void BeginDraw();
	void DrawProcess();
	void EndDraw();


	void Draw(ICamera* _camera,const std::array<int,2>& _size);

	void DrawDebug();
	void DrawSprite();

	void Draw3D(DrawType _type);


	std::list<std::shared_ptr<Asset2DPackBase>>	m_draw2DList;
	std::list<std::shared_ptr<Asset3DPackBase>>	m_draw3DList;

	KdDebugWireFrame*			m_debugWireFrame = nullptr;

	KdRenderTargetChanger*		m_rtc = nullptr;

	bool m_bActiveRenderTarget = false;

	ICamera* m_pMainCamera = nullptr;
	std::list<ICamera*> m_pSubCamera;
public:
	// 2D描画用の情報体の基底
	struct Asset2DPackBase
	{
		virtual void Draw(class KdSpriteShader* _kdSpriteShader) = 0;
	};
	// 3D描画用の情報体の基底
	struct Asset3DPackBase
	{
		virtual void Draw(class KdStandardShader* _kdStandardShader,DrawType _drawType) = 0;
		const Math::Matrix* matrix = nullptr;
		const Math::Color* color = nullptr;
		DrawType			drawType = DrawType::Lit;
	};
};

extern"C" RENDERER_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress);
extern"C" RENDERER_API void DeleteModule(JTN::Module::IModule* _module);

#define GETRENDERER static_cast<RenderManager*>(JTN::GetEngine()->GetModule<IRenderer>())