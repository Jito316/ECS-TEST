#pragma once
#include "Core.h"

CORE_API inline void OutputJson(nlohmann::json _json, std::string _path)
{
	std::ofstream file(_path);
	if (file.is_open()) {
		file << _json.dump(4);
		file.close();
	};
}
CORE_API inline nlohmann::json InputJson(std::string _path)
{
	std::ifstream file(_path);
	nlohmann::json json;
	if (file.is_open())
	{
		file >> json;
		file.close();
	}
	return json;
}

CORE_API inline Math::Vector3 InPutVec3(nlohmann::json _json)
{
	return Math::Vector3
	(
		_json["x"],
		_json["y"],
		_json["z"]
	);
}
CORE_API inline nlohmann::json OutPutVec3(const JTN::Math::Vector3& _value)
{
	return nlohmann::json
	{
		{"x",_value.x},
		{"y",_value.y},
		{"z",_value.z}
	};
}

CORE_API inline Math::Color InPutColor(nlohmann::json _json)
{
	return Math::Color
	(
		_json["R"],
		_json["G"],
		_json["B"],
		_json["A"]
	);
}
CORE_API inline nlohmann::json OutPutColor(const JTN::Math::Color& _value)
{
	return nlohmann::json
	{
		{"R",_value.R()},
		{"G",_value.G()},
		{"B",_value.B()},
		{"A",_value.A()}
	};
}

CORE_API inline Math::Vector2 InPutVec2(nlohmann::json _json)
{
	return Math::Vector2
	(
		_json["x"],
		_json["y"]
	);
}
CORE_API inline nlohmann::json OutPutVec2(const Math::Vector2& _value)
{
	return nlohmann::json
	{
		{"x",_value.x},
		{"y",_value.y}
	};
}

CORE_API inline Math::Rectangle InputRect(nlohmann::json _json)
{
	return Math::Rectangle
	(
		_json["x"],
		_json["y"],
		_json["width"],
		_json["height"]
	);
}
CORE_API inline nlohmann::json OutPutRect(const Math::Rectangle& _value)
{
	return nlohmann::json
	{
		{"x",_value.x},
		{"y",_value.y},
		{"width",_value.width},
		{"height",_value.height}
	};
}

template<typename T, typename Key = T::key_type, typename Value = T::mapped_type>
concept is_Map = std::disjunction_v
<
	std::is_same <T, std::map<Key, Value>>
	, std::is_same <T, std::unordered_map<Key, Value>>
>;

template<is_Map Map>inline Map  InPutMap(nlohmann::json _json)
{
	Map map;
	for (nlohmann::json::const_iterator it = _json.begin()
		; it != _json.end()
		; it++
		)
	{
		map[it.key()] = *it;
	}
	return map;
}
template<is_Map Map>inline void InPutMap(Map& _map, nlohmann::json _json)
{
	for (nlohmann::json::const_iterator it = _json.begin()
		; it != _json.end()
		; it++
		)
	{
		_map[it.key()] = *it;
	}
}
template<is_Map Map>inline void InPutMap
(
	Map& _map
	, nlohmann::json _json
	, std::function<const typename Map::value_type(nlohmann::json::const_iterator)>_pair
)
{
	for (nlohmann::json::const_iterator it = _json.begin()
		; it != _json.end()
		; it++
		)
	{
		auto&[key,value] = _pair(it);
		_map[key] = value;
	}
}


template<is_Map Map>inline nlohmann::json OutPutMap(const Map& _value)
{
	nlohmann::json json;
	for (auto& [key, value] : _value)
	{
		json[key] = value;
	}
	return json;
}
template<is_Map Map>inline void OutPutMap(nlohmann::json& _json,const Map& _value)
{
	for (auto& [key, value] : _value)
	{
		_json[key] = value;
	}
}
template<is_Map Map>inline void OutPutMap
(
	nlohmann::json& _json
	, const Map& _value
	, std::function<void(const typename Map::value_type&, nlohmann::json&)> _fn
)
{
	for (auto it = _value.begin()
		; it != _value.end()
		; it++
		)
	{
		_fn(*it, _json);
	}
}