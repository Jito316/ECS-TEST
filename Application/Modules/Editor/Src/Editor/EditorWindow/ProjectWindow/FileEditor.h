#pragma once
namespace FileEditor
{
	inline void OpenFile(const std::filesystem::path& _path, bool& _edited, std::filesystem::path& _openPath, bool& _changed)
	{
		if (_path.has_extension())return;
		if (_openPath == _path)return;
		if (!ImGui::MenuItem("OpenFile"))return;
		_openPath = _path;
		_edited = _changed = true;
	}
	inline void ShowExplorer(const std::filesystem::path& _path, bool& _edited)
	{
		if (_path.has_extension())return;
		if (!ImGui::MenuItem("ShowExplorer"))return;
		std::filesystem::path path = std::filesystem::absolute(_path);
		ShellExecute(NULL, L"open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
		_edited = true;
	}
	inline void NewFile(const std::filesystem::path& _path, bool& _edited)
	{
		if (_path.has_extension())return;
		std::function<void(std::string)> CraeteFile = [&](std::string _name)
			{
				if (ImGui::MenuItem(_name.c_str()))
				{
					std::ofstream file(_path.string() + "/" + _name);
					if (file.is_open()) file.close();
					_edited = true;
				}
			};
		if (ImGui::BeginMenu("NewFile"))
		{
			CraeteFile("Script.h");
			CraeteFile("Script.cpp");
			ImGui::EndMenu();
		}
	}
	inline void RenameFile(const std::filesystem::path& _path, bool& _edited)
	{
		if (!ImGui::MenuItem("RenameFile"))return;
		_edited = true;
	}
	inline void RemoveFile(const std::filesystem::path& _path, bool& _edited, std::filesystem::path& _openPath)
	{
		if (_openPath == _path)return;
		if (!ImGui::MenuItem("RemoveFile"))return;
		std::filesystem::remove_all(_path);
		_edited = true;
	}
	inline void RegisterFavorite(const std::filesystem::path& _path, bool& _edited, std::list<std::string>& _list)
	{
		if (!std::filesystem::is_directory(_path))return;
		std::list<std::string>::iterator it = std::find(_list.begin(), _list.end(), _path);
		bool pushed = it != _list.end();
		if (!ImGui::MenuItem(pushed ? "RemoveFavorite" : "RegisterFavorite"))return;
		if (pushed) _list.erase(it);
		else if (!pushed || _list.empty()) _list.push_back(_path.string());
		_edited = true;
	}
}