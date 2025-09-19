#pragma once
#include "Interface/Manager/ComponentFactory/IComponentFactory.h"

// トランスフォーム
#include "Script/Object/Component/TransformComponent/TransformComponent.h"
// カメラ
#include "Script/Object/Component/CameraComponent/CameraComponent.h"
// サウンド
#include "Script/Object/Component/Audio/AudioListenerComponent/AudioListenerComponent.h"
#include "Script/Object/Component/Audio/AudioPlayerComponent/AudioPlayerComponent.h"
// レンダー
#include "Script/Object/Component/RendererComponent/ModelRendererComponent/ModelRendererComponent.h"
#include "Script/Object/Component/RendererComponent/SquarePolygonRendererComponent/SquarePolygonRendererComponent.h"
#include "Script/Object/Component/RendererComponent/TextureRendererComponent/TextureRendererComponent.h"

// 算術
#include "Script/Object/Component/RigidbodyComponent/RigidbodyComponent.h"
#include "Script/Object/Component/ModelAnimatorComponent/ModelAnimatorComponent.h"
#include "Script/Object/Component/ColliderComponent/BoxColliderComponent/BoxColliderComponent.h"
#include "Script/Object/Component/ColliderComponent/MeshColliderComponent/MeshColliderComponent.h"
#include "Script/Object/Component/ColliderComponent/SphereColliderComponent/SphereColliderComponent.h"

inline void BuildInComponentRegister(IComponentFactory& _factory) {
	{// カメラ
		_factory.RegisterComponent<CameraComponent>();
	}
	{// 描画
		_factory.RegisterComponent<ModelRendererComponent>();
		_factory.RegisterComponent<SquarePolygonRendererComponent>();
		_factory.RegisterComponent<TextureRendererComponent>();
	}
	{// 算術
		_factory.RegisterComponent<RigidbodyComponent>();
		_factory.RegisterComponent<ModelAnimatorComponent>();

		_factory.RegisterComponent<BoxColliderComponent>();
		_factory.RegisterComponent<MeshColliderComponent>();
		_factory.RegisterComponent<SphereColliderComponent>();
	}
}