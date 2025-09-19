#pragma once
#include "Interface/IConfigSystem.h"

namespace JTN {
	namespace File {
		class ConfigSystem : public IConfigSystem
		{
		public:
			ConfigSystem()
				: m_configData(nullptr)
				, m_configPath("Asset/Data/.config")
			{
				std::filesystem::path path = m_configPath;
				std::filesystem::create_directories(path.parent_path());
			}

			~ConfigSystem()override = default;

			bool Load();
			void Save();

		protected:
			bool Load(std::string_view _key, nlohmann::json& _json)override;
			void Save(std::string_view _key, const nlohmann::json& _json)override;

			// 入力/出力用
			nlohmann::json* m_configData;
			std::string m_configPath;
		};
	};
};