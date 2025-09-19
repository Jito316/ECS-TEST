#include "RenderManager.h"
#include "Interface/IWindow.h"
#include "KdShader/KdShaderManager.h"
#include "KdDirect3D/KdDirect3D.h"
#include "Utility/KdDebugWireFrame.h"
#include "Utility/KdDataStorage.h"

struct Asset3DPack_ModelData : RenderManager::Asset3DPackBase
{
	void Draw(KdStandardShader* _kdStandardShader, RenderManager::DrawType _drawType)override
	{
		if (drawType & _drawType)
		{
			_kdStandardShader->DrawModel(*asset, *matrix, *color);
		}
	}

	const IModelData* asset = nullptr;
};
struct Asset3DPack_ModelWork : RenderManager::Asset3DPackBase
{
	void Draw(KdStandardShader* _kdStandardShader, RenderManager::DrawType _drawType)override
	{
		if (drawType & _drawType)
		{
			_kdStandardShader->DrawModel(*asset, *matrix, *color);
		}
	}

	IModelWork* asset = nullptr;
};
struct Asset3DPack_Polygon : RenderManager::Asset3DPackBase
{
	void Draw(KdStandardShader* _kdStandardShader, RenderManager::DrawType _drawType)override
	{
		if (drawType & _drawType)
		{
			_kdStandardShader->DrawPolygon(*asset, *matrix, *color);
		}
	}

	const IPolygon* asset = nullptr;
};
struct TexturePack : RenderManager::Asset2DPackBase
{
	void Draw(KdSpriteShader* _kdSpriteShader)override
	{
		_kdSpriteShader->SetMatrix(*mat);
		_kdSpriteShader->DrawTex(asset, 0, 0, (int)rect->width, (int)rect->height, rect, color);
		_kdSpriteShader->SetMatrix(Math::Matrix::Identity);
	}

	const ITexture* asset = nullptr;
	const Math::Matrix* mat = nullptr;
	const Math::Rectangle* rect = nullptr;
	const Math::Color* color = nullptr;
};

bool RenderManager::Entry()
{	
	ModuleEntry(ModuleEntry_InitializeRenderer,&RenderManager::SetUp);
	ModuleEntry(ModuleEntry_UpdateRenderer,&RenderManager::Update);
	ModuleEntry(ModuleEntry_FinalizeRenderer,&RenderManager::Shutdown);

	ModuleEntry(ModuleEntry_BeginDraw,&RenderManager::BeginDraw);
	ModuleEntry(ModuleEntry_DrawProcess,&RenderManager::DrawProcess);
	ModuleEntry(ModuleEntry_EndDraw,&RenderManager::EndDraw);

	return GETMODULE(IWindow);
}

void RenderManager::SetUp()
{
	m_kdDirect3D = new KdDirect3D;
	m_kdShaderManager = new KdShaderManager;

	m_debugWireFrame = new KdDebugWireFrame;

	m_rtc = new KdRenderTargetChanger;

	m_kdAssets = new KdAssets;

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	auto window = GETMODULE(IWindow);
	const auto& windowSize = window->GetWinodwSize();

	std::string errorMsg;
	if (m_kdDirect3D->Init(window->GetWndHandle(), windowSize[0], windowSize[1], deviceDebugMode, errorMsg) == false) {
		MessageBoxA(window->GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return;
	}

	m_kdShaderManager->Init();
	return;
}

void RenderManager::Shutdown()
{
	m_kdShaderManager->Release();

	KdSafeDelete(m_rtc);
	KdSafeDelete(m_kdAssets);

	KdSafeDelete(m_debugWireFrame);
	KdSafeDelete(m_kdDirect3D);
	KdSafeDelete(m_kdShaderManager);

}

void RenderManager::Update()
{
	// 空間環境の更新
	m_kdShaderManager->WorkAmbientController().Update();

	m_pMainCamera = nullptr;
	m_pSubCamera.clear();

	m_draw2DList.clear();
	m_draw3DList.clear();

	m_debugWireFrame->Clear();
}

void RenderManager::Draw(const IModelData* _modelData, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)
{
	auto temp = std::make_shared<Asset3DPack_ModelData>();
	{
		temp->asset = _modelData;
		temp->matrix = &_matrix;
		temp->color = &_color;
		temp->drawType = _drawType;
	}
	m_draw3DList.push_back(temp);
}
void RenderManager::Draw(IModelWork* _modelWork, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)
{
	auto temp = std::make_shared<Asset3DPack_ModelWork>();
	{
		temp->asset = _modelWork;
		temp->matrix = &_matrix;
		temp->color = &_color;
		temp->drawType = _drawType;
	}
	m_draw3DList.push_back(temp);
}
void RenderManager::Draw(const IPolygon* _polygon, const Math::Matrix& _matrix, const Math::Color& _color, const DrawType _drawType)
{
	auto temp = std::make_shared<Asset3DPack_Polygon>();
	{
		temp->asset = _polygon;
		temp->matrix = &_matrix;
		temp->color = &_color;
		temp->drawType = _drawType;
	}
	m_draw3DList.push_back(temp);
}
void RenderManager::Draw(const ITexture* _texture, const Math::Matrix& _matrix, const Math::Rectangle& _rectangle, const Math::Color& _color)
{
	auto temp = std::make_shared<TexturePack>();
	{
		temp->asset = _texture;
		temp->rect = &_rectangle;
		temp->mat = &_matrix;
		temp->color = &_color;
	}
	m_draw2DList.push_back(temp);
}

void RenderManager::BeginDraw()
{
	m_kdDirect3D->ClearBackBuffer();
}

void RenderManager::DrawProcess()
{
	auto window = GETMODULE(IWindow);
	const auto& windowSize = window->GetWinodwSize();
	for (auto& it : m_pSubCamera) {
		Draw(it, windowSize);
	}

	if (m_pMainCamera) {
		Draw(m_pMainCamera, windowSize);
		auto& sprit = m_kdShaderManager->m_spriteShader;
		sprit.Begin();
		{
			sprit.DrawTex(m_pMainCamera->GetViewTexture(), 0, 0, (int)windowSize[0], (int)windowSize[1]);
		}
		sprit.End();
	}
}

void RenderManager::EndDraw()
{
	m_kdDirect3D->SetBackBuffer();
	m_kdDirect3D->WorkSwapChain()->Present(0, 0);
}

IDebugWireFrame* RenderManager::GetDebugWireFrame()
{
	return m_debugWireFrame;
}

IDirect3D* RenderManager::GetDirect3D()
{
	return m_kdDirect3D;
}

void RenderManager::SetMainCamera(ICamera* _camera)
{
	m_pMainCamera = _camera;
}

void RenderManager::SetSubCamera(ICamera* _camera)
{
	m_pSubCamera.push_back(_camera);
}

ICamera* RenderManager::GetMainCamera()
{
	return m_pMainCamera;
}

std::list<ICamera*>& RenderManager::GetSubCameraList()
{
	return m_pSubCamera;
}

#include "KdDirect3D/KdTexture.h"
ITexture* RenderManager::CreateTexture()
{
	return new KdTexture;
}

#include "KdDirect3D/KdModel.h"
IModelData* RenderManager::CreateModelData()
{
	return new KdModelData;
}
IModelWork* RenderManager::CreateModelWork()
{
	return new KdModelWork;
}

#include "KdDirect3D/Polygon/KdSquarePolygon.h"
ISquarePolygon* RenderManager::CreateSquarePolygon()
{
	return new KdSquarePolygon;
}

#include "KdDirect3D/Polygon/KdTrailPolygon.h"
ITrailPolygon* RenderManager::CreateTrailPolygon()
{
	return new KdTrailPolygon;
}

#include "KdDirect3D/KdCamera.h"
ICamera* RenderManager::CreateCamera()
{
	return new KdCamera;
}

void RenderManager::Draw(ICamera* _camera, const std::array<int, 2>& _size)
{
	_camera->SetToShader();

	KdRenderTargetPack pack;
	pack.CreateRenderTarget(_size[0], _size[1]);
	pack.ClearTexture(kBlackColor);
	m_rtc->ChangeRenderTarget(pack);
	
	m_kdShaderManager->WorkAmbientController().Draw();
	m_kdShaderManager->m_postProcessShader.Draw();


	// Draw3D
	{
		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
		m_kdShaderManager->m_StandardShader.BeginGenerateDepthMapFromLight();
		{
			Draw3D(DrawType::DepthOfShadow);
		}
		m_kdShaderManager->m_StandardShader.EndGenerateDepthMapFromLight();

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
		m_kdShaderManager->m_StandardShader.BeginLit();
		{
			Draw3D(DrawType::Lit);
		}
		m_kdShaderManager->m_StandardShader.EndLit();

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
		m_kdShaderManager->m_StandardShader.BeginUnLit();
		{
			Draw3D(DrawType::UnLit);
		}
		m_kdShaderManager->m_StandardShader.EndUnLit();

		// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
		// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
		m_kdShaderManager->m_postProcessShader.BeginBright();
		{
			Draw3D(DrawType::Bright);
		}
		m_kdShaderManager->m_postProcessShader.EndBright();
	}


	m_kdShaderManager->m_postProcessShader.PostEffectProcess();
	DrawDebug();
	DrawSprite();

	m_rtc->UndoRenderTarget();
	_camera->SetViewTexture(pack.m_RTTexture);
}

void RenderManager::DrawSprite()
{
	m_kdShaderManager->m_spriteShader.Begin();
	{
		for (auto& draw2D : m_draw2DList)draw2D->Draw(&m_kdShaderManager->m_spriteShader);
	}
	m_kdShaderManager->m_spriteShader.End();
}
void RenderManager::DrawDebug()
{
	if (!JTN::GetEngine()->is_Debug()) return;
	m_kdShaderManager->m_StandardShader.BeginUnLit();
	{
		m_debugWireFrame->Draw();
	}
	m_kdShaderManager->m_StandardShader.EndUnLit();
}

void RenderManager::Draw3D(DrawType _type)
{
	for (auto& draw3D : m_draw3DList)
	{
		draw3D->Draw(&m_kdShaderManager->m_StandardShader, _type);
	}
}

extern"C" RENDERER_API void CreateModule(JTN::Module::IModule** _pModuleAdress, const std::type_info** _pInterfaceInfoAdress)
{
	*_pModuleAdress =  new RenderManager;
	*_pInterfaceInfoAdress = &typeid(IRenderer);
}

extern"C" RENDERER_API void DeleteModule(JTN::Module::IModule* _module)
{
	delete _module;
}


