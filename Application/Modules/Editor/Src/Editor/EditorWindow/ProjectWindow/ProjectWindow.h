#pragma once
#include "Interface/EditorWindow/IEditorWindow.h"

class ProjectWindow
	:public IEditorWindow
{
private:
	void UpdateDirectoryTree();
	void UpdateDirectoryView();

	//監視ディレクトリの変更
	void ChangeTarget(const std::filesystem::path& _path);

	//相対パスの底から全てのディレクトリを追う奴
	void CreateFileTree(const std::filesystem::path& _path);
	//ファイルをいじる用の関数
	bool EditFile(const std::filesystem::path& _path);

	std::list<std::string> m_favoritePathList;
	std::list<std::function<void(const std::filesystem::path&, bool&)>>m_rightClickedFileEdit;
	bool m_bOpenFileEditor = false;

	std::filesystem::path m_path;
	std::list<std::filesystem::path> m_hierarchy;
	std::unordered_map<std::string, std::function<bool(const std::filesystem::path&)>> m_fileSource;

	bool m_bDirty = false;

	float m_thumbnailSize = 64.0f;
	float m_padding = 16.0f;
	std::shared_ptr<ITexture> m_iconDocumentIcon;
	std::shared_ptr<ITexture> m_closeFolderIcon;
protected:
	void Init()override;
	void Release()override;
	
	void UpdateContents() override;

	void ConfigLoadContents(nlohmann::json& _json)override;
	void ConfigSaveContents(nlohmann::json& _json)override;
};