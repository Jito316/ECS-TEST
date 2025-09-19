#pragma once
// DirectX Tool Kit
#pragma comment(lib, "DirectXTK.lib")
#include <SimpleMath.h>

namespace JTN 
{
	namespace Math
	{
		using Color = DirectX::SimpleMath::Color;
		using Matrix = DirectX::SimpleMath::Matrix;
		using Plane = DirectX::SimpleMath::Plane;
		using Quaternion = DirectX::SimpleMath::Quaternion;
		using Ray = DirectX::SimpleMath::Ray;
		using Rectangle = DirectX::SimpleMath::Rectangle;
		using Vector2 = DirectX::SimpleMath::Vector2;
		using Vector3 = DirectX::SimpleMath::Vector3;
		using Vector4 = DirectX::SimpleMath::Vector4;
		using Viewport = DirectX::SimpleMath::Viewport;
	}
}