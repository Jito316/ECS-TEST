#include "ConfigSystem.h"
#include "Engine/Engine.h"

namespace JTN {
	namespace File {
		bool JTN::File::ConfigSystem::Load()
		{
			m_configData = new nlohmann::json;
			*m_configData = Utility::JsonHelper::InputJson(m_configPath);
			return m_configData;
		}
		void ConfigSystem::Save()
		{
			Utility::JsonHelper::OutputJson(*m_configData, m_configPath);
			delete m_configData;
		}

		bool ConfigSystem::Load(std::string_view _key, nlohmann::json& _json)
		{
			_json = (*m_configData)[_key];
			bool failed = _json.is_null();
			return !failed;
		}

		void ConfigSystem::Save(std::string_view _key, const nlohmann::json& _json)
		{
			(*m_configData)[_key] = _json;
		}
	};
};