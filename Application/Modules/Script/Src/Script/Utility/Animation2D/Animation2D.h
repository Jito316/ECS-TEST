#pragma once
class Animation2D
{
public:
	struct AnimeFrame
	{
		int w = 0;
		int h = 0;

		void InitJson(nlohmann::json _json)
		{
			w = _json["w"];
			h = _json["h"];
		}
		nlohmann::json Serialize()
		{
			nlohmann::json json;
			json["w"] = w;
			json["h"] = h;
			return json;
		}
	};
	struct AnimeSet
	{
		std::string name;
		int	interval = 0;
		std::vector<AnimeFrame> frames;
		int	nowFrame = 0;

		void Animation(Math::Rectangle& _rect);
		AnimeFrame Animation(ISquarePolygon* _asset);
		void ImGuiUpdate();

		void InitJson(nlohmann::json _json)
		{
			name = _json["name"];
			interval = _json["interval"];

			for (auto& frame : _json["frames"])
			{
				AnimeFrame temp;
				temp.InitJson(frame);
				frames.push_back(temp);
			}
		}
		nlohmann::json Serialize()
		{
			nlohmann::json json;
			json["name"] = name;
			json["interval"] = interval;
			for (auto& frame : frames)
			{
				json["frames"].push_back(frame.Serialize());
			}
			return json;
		}
		AnimeFrame addKoma;
	};
public:
	void PreUpdate(Math::Rectangle& _rect);
	void PreUpdate(ISquarePolygon* _asset);

	void ImGuiUpdate();

	void ChangeAnime(std::string _nowAnime);
	AnimeFrame GetAnimeFrame() const { return m_nowFrame; };


	nlohmann::json Serialize();
	void SetJson(nlohmann::json _json);
private:

	AnimeFrame			m_split;
	std::list<AnimeSet> m_animeList;
	AnimeFrame			m_nowFrame;

	std::string			m_nowAnime;
};
