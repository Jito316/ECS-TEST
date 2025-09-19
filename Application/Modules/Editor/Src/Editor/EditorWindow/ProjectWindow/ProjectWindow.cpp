#include "ProjectWindow.h"
#include "Editor/UndoRedo/UndoRedo.h"
#include "Editor/Utility/DragDrop/Object/DragDrop_Object.h"

void ProjectWindow::UpdateDirectoryTree()
{
	ImGui::BeginChild("DirectoryTree");
	{
		ImGui::BeginChild("favorite", {}, ImGuiChildFlags_AutoResizeY);
		if (ImGui::TreeNodeEx("favorite", ImGuiTreeNodeFlags_DefaultOpen | (m_favoritePathList.empty() * ImGuiTreeNodeFlags_Leaf)))
		{
			for (auto& set : m_favoritePathList)
			{
				ImGui::TreeNodeEx(std::filesystem::path(set).filename().string().c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf);
				if (EditFile(set))break;
			}
			ImGui::TreePop();
		}
		ImGui::EndChild();

		ImGui::Text("");

		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(".")))
		{
			if (entry.is_directory())
			{
				CreateFileTree(entry.path());
			}
		}
	}
	ImGui::EndChild();
}
void ProjectWindow::UpdateDirectoryView()
{
	ImGui::BeginChild("DirectoryContents", {}, ImGuiChildFlags_None, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			for (auto& it : m_hierarchy)
			{
				if (ImGui::MenuItem(it.filename().string().data()))
				{
					ChangeTarget(it);
					break;
				}

				if (ImGui::MenuItem(">"))
				{
					ImGui::OpenPopup(it.string().data());
				}

				if (ImGui::BeginPopup(it.string().data()))
				{
					bool changed = false;
					for (const auto& entry : std::filesystem::directory_iterator(it))
					{
						if (entry.is_directory())
						{
							if (ImGui::MenuItem(entry.path().filename().string().data()))
							{
								ChangeTarget(entry);
								changed = true;
								break;
							}
						}
					}
					ImGui::EndPopup();

					if (changed) 
					{
						break;
					}
				}
			}
			ImGui::Text(m_path.filename().string().data());
		}
		ImGui::EndMenuBar();

		//横に表示できる最大数を出す
		float cellSize = m_thumbnailSize + m_padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int m_columnCount = (int)(panelWidth / cellSize);
		if (m_columnCount < 1)m_columnCount = 1;

		//計算した表示数に改行までの許容数をする
		ImGui::Columns(m_columnCount, 0, false);

		//存在しないパスである場合はリターン
		if (!std::filesystem::exists(m_path))return;

		for (const auto& entry : std::filesystem::directory_iterator(m_path))
		{
			bool is_directory = entry.is_directory();
			std::string filename = entry.path().filename().string();

			ImGui::PushID(filename.c_str());
			ImGui::ImageButton((is_directory ? m_closeFolderIcon : m_iconDocumentIcon)->WorkSRView(), { m_thumbnailSize ,m_thumbnailSize });
			ImGui::PopID();

			std::filesystem::path itemPath = std::filesystem::relative(entry.path()).string();

			bool calledDragDrop = false;
			//ディレクトリ内のファイルの拡張子に合わせたソースを呼び出す
			std::unordered_map<std::string, std::function<bool(const std::filesystem::path&)>>::iterator Source = m_fileSource.find(entry.path().extension().string());
			if (Source != m_fileSource.end())calledDragDrop = Source->second(itemPath);

			//GameObjectの保存
			if (is_directory)
			{
				Utility::DragDrop::TargetGameObjectDataSave(itemPath.string());
			}
			if (!calledDragDrop)EditFile(entry.path());

			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
		}
		ImGui::Columns();
	}
	ImGui::EndChild();

	EditFile(m_path);
	Utility::DragDrop::TargetGameObjectDataSave(m_path.string());
	m_bOpenFileEditor = false;
}

void ProjectWindow::ChangeTarget(const std::filesystem::path& _path)
{
	if (!m_bDirty && m_path != _path)
	{
		m_path = _path;
		m_hierarchy.clear();
		if (m_path.has_parent_path())
		{
			std::filesystem::path parent = m_path;
			while (parent.has_parent_path())
			{
				parent = parent.parent_path();
				m_hierarchy.push_front(parent);
			}
			m_bDirty = true;
		}
	}
}

void ProjectWindow::CreateFileTree(const std::filesystem::path& _path)
{
	// 閲覧対象の場合
	bool isTarget = m_path == _path;

	// 自分が持っているディレクトリを洗い出す
	std::vector<std::filesystem::path> subdirs;
	for (const auto& entry : std::filesystem::directory_iterator(_path))
	{
		if (entry.is_directory())
		{
			subdirs.push_back(entry.path());
		}
	}

	// ツリーのフラグの処理
	ImGuiTreeNodeFlags treeFlg = ImGuiTreeNodeFlags_OpenOnArrow;
	treeFlg |= (subdirs.empty() ? ImGuiTreeNodeFlags_Leaf : ImGuiTreeNodeFlags_None);

	// 閲覧対象が変更された場合
	// 閲覧対象までのツリーを開く
	if (m_bDirty)
	{
		if (isTarget)
		{
			m_bDirty = false;
		}
		else if (m_path.string().find(_path.string()) == 0)
		{
			ImGui::SetNextItemOpen(true);
		}
	}

	//閲覧中のディレクトリの強調表示
	ImGui::PushStyleColor(ImGuiCol_Text, isTarget ? ImVec4(0.5f, 0.5f, 1.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	bool open = ImGui::TreeNodeEx(_path.filename().string().data(), treeFlg);
	ImGui::PopStyleColor();

	//階層移動
	EditFile(_path);
	//Pathを抜き出す
	Utility::DragDrop::TargetGameObjectDataSave(_path.string());

	if (open)
	{
		//洗い出したディレクトリを再帰的に表示
		for (const auto& subdir : subdirs)
		{
			CreateFileTree(subdir);
		}
		ImGui::TreePop();
	}
}

bool ProjectWindow::EditFile(const std::filesystem::path& _path)
{
	bool edited = false;

	if (!m_bOpenFileEditor)
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && (!ImGui::IsItemToggledOpen()) && std::filesystem::is_directory(_path))
		{
			ChangeTarget(_path);
		}

		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup(("EditFile" + _path.string()).c_str());
			m_bOpenFileEditor = true;
		}

		if (ImGui::BeginPopup(("EditFile" + _path.string()).c_str()))
		{
			for (auto& Fn : m_rightClickedFileEdit)Fn(_path, edited);
			ImGui::EndPopup();
		}
	}

	return edited;
}

#include "Editor/EditorWindow/SetUpScene/SetUpScene.h"
#include "Editor/EditorWindow/ProjectWindow/FileEditor.h"
#include "Editor/Utility/DragDrop/DragDrop.h"
void ProjectWindow::Init()
{
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::OpenFile(_path, _edited, m_path, m_bDirty); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::NewFile(_path, _edited); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::ShowExplorer(_path, _edited); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::RenameFile(_path, _edited); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::RegisterFavorite(_path, _edited, m_favoritePathList); });
	m_rightClickedFileEdit.push_back([&](const std::filesystem::path& _path, bool& _edited) {FileEditor::RemoveFile(_path, _edited, m_path); });

	m_fileSource[".prefab"] = [&](const std::filesystem::path& _path) {return Utility::DragDrop::SourceGameObjectDataPath(_path.string()); };
	m_fileSource[".png"] = [&](const std::filesystem::path& _path) {return DragDrop::SourceTexture(_path.string()); };
	m_fileSource[".gltf"] = [&](const std::filesystem::path& _path) {return DragDrop::SourceModel(_path.string()); };
	m_fileSource[".wav"] = [&](const std::filesystem::path& _path) {return DragDrop::SourceWav(_path.string()); };
	m_fileSource[".scene"] = [&](const std::filesystem::path& _path) {return MyDragDrop::SourceScene(_path.string()); };

	m_iconDocumentIcon = GETMODULE(IResource)->GetTexture("Asset/Textures/UI/DocumentIcon.png");
	m_closeFolderIcon = GETMODULE(IResource)->GetTexture("Asset/Textures/UI/CloseFolderIcon.png");

	ConfigLoad();
}
void ProjectWindow::Release()
{
	ConfigSave();
}

void ProjectWindow::UpdateContents()
{
	if (ImGui::BeginTable("ProjectWindow", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		UpdateDirectoryTree();

		ImGui::TableSetColumnIndex(1);
		UpdateDirectoryView();
		ImGui::EndTable();
	}
}

void ProjectWindow::ConfigLoadContents(nlohmann::json& _json)
{
	ChangeTarget(_json.value("OpenDirectoryPath", "."));
	for (auto& path : _json["FavoritePaths"])
	{
		m_favoritePathList.push_back(path);
	}
}
void ProjectWindow::ConfigSaveContents(nlohmann::json& _json)
{
	_json["OpenDirectoryPath"] = m_path;
	_json["FavoritePaths"] = nlohmann::json::array();
	for (auto& path : m_favoritePathList)
	{
		_json["FavoritePaths"].push_back(path);
	}
}