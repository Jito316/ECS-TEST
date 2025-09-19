#pragma once
namespace DragDrop {
	bool SourceTexture(std::string _path);
	bool TargetTexture(std::string& _path);
		
	bool SourceModel(std::string _path);
	bool TargetModel(std::string& _path);

	bool SourceWav(std::string _path);
	bool TargetWav(std::string& _path);
}