#pragma once

#include "KdUtility/KdUtility.h"
#include "Utility/KdUtility/KdCSVData.h"
#include "Utility/KdUtility/KdFPSController.h"

namespace Utility
{
	namespace TemplateHepler
	{
		template<typename T>concept NumberClass = std::is_floating_point_v<T> || std::is_integral_v<T>;
	}

	namespace StringHelper
	{
		CORE_API inline void DeleteWord(std::string& _str, const std::string& _word) {
			size_t pos = 0;
			while ((pos = _str.find(_word, pos)) != std::string::npos) {
				_str.erase(pos, _word.length());
			}
		}
		CORE_API inline std::string PickName(std::string str, const char separator = ' ')
		{
			std::vector<std::string> out;
			std::stringstream ss(str);
			std::string buf;
			while (std::getline(ss, buf, separator))
			{
				out.push_back(buf);
			}

			return out[1];
		}

	}

	namespace FileHelper
	{
		CORE_API inline bool CopyFileUsingStreams(const std::string& sourcePath, const std::string& destinationPath) {
			// 入力ファイルを開く（バイナリモード）
			std::ifstream inputFile(sourcePath, std::ios::binary);
			if (!inputFile.is_open()) {
				std::cerr << "Failed to open source file: " << sourcePath << std::endl;
				return false;
			}

			// 出力ファイルを開く（バイナリモード）
			std::ofstream outputFile(destinationPath, std::ios::binary);
			if (!outputFile.is_open()) {
				std::cerr << "Failed to open destination file: " << destinationPath << std::endl;
				return false;
			}

			// ストリームを使ってデータをコピー
			outputFile << inputFile.rdbuf();

			// ストリームを閉じる
			inputFile.close();
			outputFile.close();

			// コピー成功
			return true;
		}
	}

	namespace TypeIDHelper
	{
		// 非テンプレート基底クラス（DLLでエクスポート可能）
		class CORE_API TypeInfoBase {
		public:
			virtual const std::type_info& GetTypeInfo() const = 0;
			virtual ~TypeInfoBase() = default;
		};

		// テンプレートクラス（ヘッダーオンリー）
		template<typename T>
		class TypeInfo : public TypeInfoBase {
		public:
			const std::type_info& GetTypeInfo() const override {
				return typeid(T);
			}

			// ヘルパー関数
			static const std::type_info& Create() {
				return typeid(T);
			}
		};
	};

	namespace MathHelper
	{
		CORE_API inline float ToRadians(const float _deg)
		{
			return DirectX::XMConvertToRadians(_deg);
		}
		CORE_API inline void ToRadians(const JTN::Math::Vector3& _deg, JTN::Math::Vector3& _result)
		{
			_result.x = DirectX::XMConvertToRadians(_deg.x);
			_result.y = DirectX::XMConvertToRadians(_deg.y);
			_result.z = DirectX::XMConvertToRadians(_deg.z);
		}
		CORE_API inline JTN::Math::Vector3 ToRadians(const JTN::Math::Vector3& _deg)
		{
			JTN::Math::Vector3 result;
			ToRadians(_deg, result);
			return result;
		}

		CORE_API inline float ToDegrees(float _rad)
		{
			return DirectX::XMConvertToDegrees(_rad);
		}
		CORE_API inline void ToDegrees(const DirectX::SimpleMath::Vector3& _rad, DirectX::SimpleMath::Vector3& _result)
		{
			_result.x = ToDegrees(_rad.x);
			_result.y = ToDegrees(_rad.y);
			_result.z = ToDegrees(_rad.z);
		}
		CORE_API inline DirectX::SimpleMath::Vector3 ToDegrees(const DirectX::SimpleMath::Vector3& _rad)
		{
			DirectX::SimpleMath::Vector3 degrees3;
			ToDegrees(_rad, degrees3);
			return degrees3;
		}
		CORE_API inline DirectX::SimpleMath::Vector3 ToDegrees(const DirectX::SimpleMath::Quaternion& _Quaternion)
		{
			DirectX::SimpleMath::Vector3 result;
			DirectX::SimpleMath::Vector3 radians = _Quaternion.ToEuler();
			result.x = ToDegrees(radians.x);
			result.y = ToDegrees(radians.y);
			result.z = ToDegrees(radians.z);
			return result;
		}
		CORE_API inline void ToDegrees(const DirectX::SimpleMath::Quaternion& _Quaternion, DirectX::SimpleMath::Vector3& _result)
		{
			DirectX::SimpleMath::Vector3 radians = _Quaternion.ToEuler();
			_result.x = ToDegrees(radians.y);
			_result.y = ToDegrees(radians.x);
			_result.z = ToDegrees(radians.z);
		}

		template<TemplateHepler::NumberClass T>
		CORE_API inline void MaxBorder(T& _value, const T& _max)
		{
			if (_value > _max)_value = _max;
		}

		template<TemplateHepler::NumberClass T>
		CORE_API inline void MinBorder(T& _value, const T& _min)
		{
			if (_value < _min)_value = _min;
		}

		template<TemplateHepler::NumberClass T>
		CORE_API inline void Clamp(T& _value, const T& _min, const T& _max)
		{
			MaxBorder(_value, _max);
			MinBorder(_value, _min);
		}

		CORE_API inline JTN::Math::Matrix ToRotationMatrix(const JTN::Math::Vector3& _degees)
		{
			JTN::Math::Vector3 radian = ToRadians(_degees);
			return
				JTN::Math::Matrix::CreateRotationX(radian.x)
				* JTN::Math::Matrix::CreateRotationY(radian.y)
				* JTN::Math::Matrix::CreateRotationZ(radian.z);
		}

		template<TemplateHepler::NumberClass T = int>
		CORE_API inline T BoolToSign(bool _flg)
		{
			return (_flg * 2) - 1;
		}

		template<TemplateHepler::NumberClass Is_Num>
		CORE_API inline bool InBox(const Is_Num* _point, const Is_Num* _min, const Is_Num* _max)
		{
			if (_point[0] < _min[0])return false;
			if (_point[0] > _max[0])return false;
			if (_point[1] < _min[1])return false;
			if (_point[1] > _max[1])return false;
			return true;
		}
	};

	namespace EnumHelper
	{
		enum Direction
		{
			Up,
			Down,
			Right,
			Left,
			Forward,
			Backward
		};

	};

	namespace JsonHelper
	{
#include "Helper/JsonHelper.h"
	};

}