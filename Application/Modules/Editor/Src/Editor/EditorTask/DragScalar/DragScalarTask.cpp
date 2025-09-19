#include "DragScalarTask.h"
#include "Editor/EditorTask/EditorTask.h"
#include "Editor/EditorWindow/DebugLog/DebugLog.h"
#include "Editor/UndoRedo/UndoRedo.h"

template <typename T> struct DataType {};
// 符号付き整数型
template <> struct DataType<char> { static constexpr ImGuiDataType value = ImGuiDataType_S8; };
template <> struct DataType<signed char> { static constexpr ImGuiDataType value = ImGuiDataType_S8; };
template <> struct DataType<short> { static constexpr ImGuiDataType value = ImGuiDataType_S16; };

template <> struct DataType<int> { static constexpr ImGuiDataType value = ImGuiDataType_S32; };

template <> struct DataType<long> { static constexpr ImGuiDataType value = ImGuiDataType_S32; };
template <> struct DataType<long long> { static constexpr ImGuiDataType value = ImGuiDataType_S64; };

// 符号なし整数型
template <> struct DataType<unsigned char> { static constexpr ImGuiDataType value = ImGuiDataType_U8; };
template <> struct DataType<unsigned short> { static constexpr ImGuiDataType value = ImGuiDataType_U16; };
template <> struct DataType<unsigned int> { static constexpr ImGuiDataType value = ImGuiDataType_U32; };
template <> struct DataType<unsigned long long> { static constexpr ImGuiDataType value = ImGuiDataType_U64; };

// 浮動小数点型
template <> struct DataType<float> { static constexpr ImGuiDataType value = ImGuiDataType_Float; };
template <> struct DataType<double> { static constexpr ImGuiDataType value = ImGuiDataType_Double; };

template<class T, size_t components = 1>
class Drag : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		auto temp = (T*)(values[1]);
		std::vector<T>postValues;

		for (size_t i = 0; i < components;i++) {
			postValues.push_back(temp[i]);
		}
		bool flg = ImGui::DragScalarN(lable.data(), DataType<T>::value, values[1], components);
		if(flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<T, components>>((T*)(values[1]), &(*postValues.begin())));
		return flg;
	}
};

template<class T, size_t components = 1>
class Input : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		auto temp = (T*)(values[1]);
		std::vector<T>postValues;

		for (size_t i = 0; i < components; i++) {
			postValues.push_back(temp[i]);
		}
		bool flg = ImGui::InputScalarN(lable.data(), DataType<T>::value, values[1], components);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<T, components>>((T*)(values[1]), &(*postValues.begin())));
		return flg;
	}
};

template<class T, size_t components = 1>
class Slider : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		auto temp = (T*)(values[1]);
		std::vector<T>postValues;

		for (size_t i = 0; i < components; i++) {
			postValues.push_back(temp[i]);
		}

		bool flg = ImGui::SliderScalarN(
			lable.data()
			, DataType<T>::value
			, values
			, components
			, values[2]
			, values[3]
		);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<T, components>>((T*)(values[1]), &(*postValues.begin())));
		return flg;
	}
};

class ColorEdit : public BaseEditorTask
{
public:
	bool TaskContents(void** values)
	{
		std::string_view lable((const char*)values[0]);
		Math::Color* value = (Math::Color*)values[1];
		Math::Color postValue = *value;
		bool flg = ImGui::ColorEdit4(lable.data(), &value->x);
		if (flg)GETEDITOR.m_spUndoRedo->Add(std::make_unique<UndoRedoAction::ValueEdit<Math::Color>>(value, &postValue));
		return flg;
	}
};


void RegisterDragScalar(std::shared_ptr<JTN::Message::CommandWorker::TaskMap> taskMap)
{
	taskMap->emplace("DragFloat", std::make_shared<Drag<float>>());
	taskMap->emplace("DragFloat2", std::make_shared<Drag<float, 2>>());
	taskMap->emplace("DragFloat3", std::make_shared<Drag<float, 3>>());
	taskMap->emplace("DragFloat4", std::make_shared<Drag<float, 4>>());
	taskMap->emplace("DragInt", std::make_shared<Drag<int>>());
	taskMap->emplace("DragInt2", std::make_shared<Drag<int, 2>>());
	taskMap->emplace("DragInt3", std::make_shared<Drag<int, 3>>());
	taskMap->emplace("DragInt4", std::make_shared<Drag<int, 4>>());
	taskMap->emplace("DragLong", std::make_shared<Drag<long>>());
	taskMap->emplace("DragLong2", std::make_shared<Drag<long, 2>>());
	taskMap->emplace("DragLong3", std::make_shared<Drag<long, 3>>());
	taskMap->emplace("DragLong4", std::make_shared<Drag<long, 4>>());

	taskMap->emplace("InputFloat", std::make_shared<Input<float>>());
	taskMap->emplace("InputFloat2", std::make_shared<Input<float, 2>>());
	taskMap->emplace("InputFloat3", std::make_shared<Input<float, 3>>());
	taskMap->emplace("InputFloat4", std::make_shared<Input<float, 4>>());
	taskMap->emplace("InputInt", std::make_shared<Input<int>>());
	taskMap->emplace("InputInt2", std::make_shared<Input<int, 2>>());
	taskMap->emplace("InputInt3", std::make_shared<Input<int, 3>>());
	taskMap->emplace("InputInt4", std::make_shared<Input<int, 4>>());
	taskMap->emplace("InputLong", std::make_shared<Input<long>>());
	taskMap->emplace("InputLong2", std::make_shared<Input<long, 2>>());
	taskMap->emplace("InputLong3", std::make_shared<Input<long, 3>>());
	taskMap->emplace("InputLong4", std::make_shared<Input<long, 4>>());
	
	taskMap->emplace("SliderFloat", std::make_shared<Slider<float>>());
	taskMap->emplace("SliderFloat2", std::make_shared<Slider<float, 2>>());
	taskMap->emplace("SliderFloat3", std::make_shared<Slider<float, 3>>());
	taskMap->emplace("SliderFloat4", std::make_shared<Slider<float, 4>>());
	taskMap->emplace("SliderInt", std::make_shared<Slider<int>>());
	taskMap->emplace("SliderInt2", std::make_shared<Slider<int, 2>>());
	taskMap->emplace("SliderInt3", std::make_shared<Slider<int, 3>>());
	taskMap->emplace("SliderInt4", std::make_shared<Slider<int, 4>>());
	taskMap->emplace("SliderLong", std::make_shared<Slider<long>>());
	taskMap->emplace("SliderLong2", std::make_shared<Slider<long, 2>>());
	taskMap->emplace("SliderLong3", std::make_shared<Slider<long, 3>>());
	taskMap->emplace("SliderLong4", std::make_shared<Slider<long, 4>>());

	taskMap->emplace("ColorEdit", std::make_shared<ColorEdit>());
}


