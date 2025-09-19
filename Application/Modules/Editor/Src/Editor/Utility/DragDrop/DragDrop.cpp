#include "DragDrop.h"
#include "Editor/UndoRedo/UndoRedo.h"
bool DragDrop::SourceTexture(std::string _path)
{
	return Utility::ImGuiHelper::DragDropSource("PicturePath", _path);
}
bool DragDrop::TargetTexture(std::string& _path)
{
	return Utility::ImGuiHelper::DragDropTarget("PicturePath", _path);
}

bool DragDrop::SourceModel(std::string _path)
{
	return  Utility::ImGuiHelper::DragDropSource("ModelPath", _path);
}
bool DragDrop::TargetModel(std::string& _path)
{
	return  Utility::ImGuiHelper::DragDropTarget("ModelPath", _path);
}

bool DragDrop::SourceWav(std::string _path)
{
	return  Utility::ImGuiHelper::DragDropSource("WavPath", _path);
}
bool DragDrop::TargetWav(std::string& _path)
{
	return  Utility::ImGuiHelper::DragDropTarget("WavPath", _path);
}