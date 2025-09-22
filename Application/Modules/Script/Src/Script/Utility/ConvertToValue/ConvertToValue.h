#pragma once
namespace JsonHelper
{
	template<class ValueType, size_t Array = 1>
	inline void ToJson(nlohmann::json& _data, std::string_view _key, const ValueType* _value)
	{
		for (size_t i = 0; i < Array; i++)
		{
			_data[_key.data()][i] = _value[i];
		}
	}

	template<class ValueType>
	inline void ToJson(nlohmann::json& _data, std::string_view _key, const ValueType& _value)
	{
		ToJson<ValueType, 1>(_data, _key, &_value);
	}

	template<>
	inline void ToJson <Math::Rectangle>(nlohmann::json& _data, std::string_view _key, const Math::Rectangle& _value)
	{
		ToJson<float, 4>(_data, _key, (float*)&_value.x);
	}

	template<>
	inline void ToJson <Math::Color>(nlohmann::json& _data, std::string_view _key, const Math::Color& _value)
	{
		ToJson<float, 4>(_data, _key, &_value.x);
	}

	template<>
	inline void ToJson <Math::Vector4>(nlohmann::json& _data, std::string_view _key, const Math::Vector4& _value)
	{
		ToJson<float, 4>(_data, _key, &_value.x);
	}

	template<>
	inline void ToJson <Math::Vector3>(nlohmann::json& _data, std::string_view _key, const Math::Vector3& _value)
	{
		ToJson<float, 3>(_data, _key, &_value.x);
	}

	template<>
	inline void ToJson <Math::Vector2>(nlohmann::json& _data, std::string_view _key, const Math::Vector2& _value)
	{
		ToJson<float, 2>(_data, _key, &_value.x);
	}

	template<class ValueType, size_t Array>
	inline void ToValue(const nlohmann::json& _data, std::string_view _key, ValueType* _value)
	{
		if (!_data.contains(_key.data()))return;

		const auto& arrayData = _data.at(_key.data());
		for (size_t i = 0; i < Array; i++)
		{
			if (arrayData.is_array())
			{
				_value[i] = arrayData.at(i);
			}
		}
	}

	template<class ValueType>
	inline void ToValue(const nlohmann::json& _data, std::string_view _key, ValueType& _value)
	{
		ToValue<ValueType, 1>(_data, _key, &_value);
	}

	template<>
	inline void ToValue <Math::Rectangle>(const nlohmann::json& _data, std::string_view _key, Math::Rectangle& _value)
	{
		ToValue<float, 4>(_data, _key, (float*)&_value.x);
	}

	template<>
	inline void ToValue <Math::Color>(const nlohmann::json& _data, std::string_view _key, Math::Color& _value)
	{
		ToValue<float, 4>(_data, _key, &_value.x);
	}

	template<>
	inline void ToValue <Math::Vector4>(const nlohmann::json& _data, std::string_view _key, Math::Vector4& _value)
	{
		ToValue<float, 4>(_data, _key, &_value.x);
	}

	template<>
	inline void ToValue <Math::Vector3>(const nlohmann::json& _data, std::string_view _key, Math::Vector3& _value)
	{
		ToValue<float, 3>(_data, _key, &_value.x);
	}

	template<>
	inline void ToValue <Math::Vector2>(const nlohmann::json& _data, std::string_view _key, Math::Vector2& _value)
	{
		ToValue<float, 2>(_data, _key, &_value.x);
	}
}