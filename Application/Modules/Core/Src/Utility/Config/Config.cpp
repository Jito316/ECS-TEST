#include "Config.h"
#include "Interface/IEngine.h"
#include "Interface/IConfigSystem.h"

namespace JTN {
	namespace File {
		bool Config::Load(std::string_view _key, nlohmann::json& _result)
		{
			auto cManager = GetEngine()->GetConfig();
			return cManager->Load(_key.data(), _result);
		}

		void Config::Save(std::string_view _key, const nlohmann::json& _data)
		{
			auto cManager = GetEngine()->GetConfig();
			cManager->Save(_key.data(), _data);
		}
	};
};