#pragma once
#include "Core.h"

namespace JTN {
	namespace File {
		class CORE_API IConfigSystem
		{
		public:
			IConfigSystem() = default;
			virtual ~IConfigSystem() = default;

		protected:
			friend class Config;
			virtual bool Load(std::string_view _key, nlohmann::json& _json) = 0;
			virtual void Save(std::string_view _key, const nlohmann::json& _json) = 0;
		};
	};
};