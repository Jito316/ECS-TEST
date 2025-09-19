#pragma once
#include "Core.h"

namespace JTN {
	namespace File {
		class CORE_API Config
		{
		public:
			bool Load(std::string_view _key, nlohmann::json& _result);
			void Save(std::string_view _key, const nlohmann::json& _data);
		};
	};
};