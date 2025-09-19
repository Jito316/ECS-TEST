#pragma once
//===============================================
//
// プリコンパイル済みヘッダー
//  ここに書いたものは初回のみ解析されるため、コンパイル時間が高速になる。
//  全てのcppからインクルードされる必要がある。
//
//===============================================


//===============================================
//
// 基本
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>
#include <typeinfo>
#include <typeindex>
#include <variant>


//===============================================
// 文字コード変換
//===============================================
#include <strconv.h>

//===============================================
// Direct3D11
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

//===============================================
// コア
//===============================================
#pragma comment(lib, "Core.lib")
#include "CoreIncluder.h"

//===============================================
// レンダラー
//===============================================
#pragma comment(lib,"Renderer.lib")
#include "Interface/IRenderer.h"
#include "Interface/Direct3D/ITexture.h"
#include "Interface/Direct3D/IMesh.h"
#include "Interface/Direct3D/IMaterial.h"
#include "Interface/Direct3D/IModel.h"
#include "Interface/Direct3D/Polygon/IPolygon.h"
#include "Interface/Direct3D/Polygon/ISquarePolygon.h"
#include "Interface/Direct3D/Polygon/ITrailPolygon.h"

#include "Interface/IResource.h"

