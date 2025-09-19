#pragma once
//===================================
// 前提外部ライブラリ
//===================================
// json
#include "nlohmann/json.hpp"

//===================================
// 自作システム
//===================================
// APIマクロ
#include "Core.h"

// 算術
#include "Math/Math.h"
#include "Math/KdAnimation.h"
#include "Math/KdUVAnimation.h"
#include "Math/KdEasing.h"
#include "Math/KdCollider.h"

// 便利関数
#include "Utility/Utility.h"

// エンジン
#include "Engine/Engine.h"

#include "Interface/IModuleSystem.h"
#include "Interface/IModule.h"
#include "Interface/ILog.h"

#include "Interface/ICommand.h"