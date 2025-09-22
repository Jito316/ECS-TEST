#include "Animation2D.h"
using namespace Utility;

void Animation2D::PreUpdate(Math::Rectangle& _rect)
{
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			anime.Animation(_rect);
			break;
		}
	}
}
void Animation2D::PreUpdate(ISquarePolygon* _asset)
{
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			m_nowFrame = anime.Animation(_asset);
			break;
		}
	}
}

void Animation2D::ImGuiUpdate()
{
	if (EDITOR_COMMAND("TreeNode", "Amination"))
	{
		EDITOR_COMMAND("InputText", "nowAnime", &m_nowAnime);

		std::string name;
		EDITOR_COMMAND("InputText", "Name", &name);
		if (EDITOR_COMMAND("Button", "add"))
		{
			AnimeSet temp;
			temp.name = name;
			m_animeList.push_back(temp);
		}
		auto anime = m_animeList.begin();
		while (anime != m_animeList.end())
		{
			if (EDITOR_COMMAND("TreeNode", anime->name.c_str()))
			{
				anime->ImGuiUpdate();

				EDITOR_COMMAND("TreePop");
			}

			if (EDITOR_COMMAND("SameLine"); EDITOR_COMMAND("SmallButton", "Remove"))
			{
				anime = m_animeList.erase(anime);
				continue;
			}
			anime++;
		}

		EDITOR_COMMAND("TreePop");
	}
}

void Animation2D::ChangeAnime(std::string _nowAnime)
{
	m_nowAnime = _nowAnime;
	for (auto& anime : m_animeList)
	{
		if (anime.name == m_nowAnime)
		{
			anime.nowFrame = 0;
			break;
		}
	}
}

nlohmann::json Animation2D::Serialize()
{
	nlohmann::json json;

	json["nowAnime"] = m_nowAnime;
	json["animeList"] = nlohmann::json::array();
	for (auto& anime : m_animeList) json["animeList"].push_back(anime.Serialize());
	return json;
}

void Animation2D::SetJson(nlohmann::json _json)
{
	m_nowAnime = _json["nowAnime"];
	for (auto& animeDate : _json["animeList"])
	{
		AnimeSet anime;
		anime.InitJson(animeDate);
		m_animeList.push_back(anime);
	}
}

void Animation2D::AnimeSet::Animation(Math::Rectangle& _rect)
{
	if (frames.empty())return;
	//　TODO : 時間の管理
	// if (Kernel::GetEngine()->m_timer.GetAlarm(interval))
	{
		nowFrame *= ++nowFrame < (int)frames.size();
	}

	_rect.x = frames[nowFrame].w * _rect.width;
	_rect.y = frames[nowFrame].h * _rect.height;
}

Animation2D::AnimeFrame Animation2D::AnimeSet::Animation(ISquarePolygon* _asset)
{
	if (frames.empty())return Animation2D::AnimeFrame();
	//　TODO : 時間の管理
	//if (Kernel::GetEngine()->m_timer.GetAlarm(interval))
	{
		nowFrame *= ++nowFrame < (int)frames.size();
	}
	_asset->SetUVRect(frames[nowFrame].w, frames[nowFrame].h);
	return frames[nowFrame];
}

void Animation2D::AnimeSet::ImGuiUpdate()
{
	EDITOR_COMMAND("InputInt", "interval", &interval);
	EDITOR_COMMAND("InputInt2", "AnimeFrame w/h", &addKoma.w);
	if (EDITOR_COMMAND("Button", "Add"))frames.push_back(addKoma);

	EDITOR_COMMAND("BeginChild", "frame");
	std::vector<AnimeFrame>::iterator frame = frames.begin();
	int i = 0;
	while (frame != frames.end())
	{
		EDITOR_COMMAND("DragInt2", std::to_string(i++).c_str(), &frame->w);
		if (EDITOR_COMMAND("SameLine"); EDITOR_COMMAND("Button", "Remove"))
		{
			frames.erase(frame);
			continue;
		}
		frame++;
	}
	EDITOR_COMMAND("EndChild");
}