#pragma once

inline bool ButtonWindowCenter(std::string label)
{
	// ボタンの幅と高さを取得
	ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
	button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
	button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

	// ウィンドウの幅を取得
	float window_width = ImGui::GetWindowSize().x;

	// ボタンを中央揃えにするためのx位置を計算
	float x_pos = (window_width - button_size.x) / 2.0f;

	// 位置を設定してボタンを描画
	ImGui::SetCursorPosX(x_pos);
	return ImGui::Button(label.c_str());
}

inline bool SmallButtonWindowCenter(std::string label)
{
	// ボタンの幅と高さを取得
	ImVec2 button_size = ImGui::CalcTextSize(label.c_str());
	button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
	button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;

	// ウィンドウの幅を取得
	float window_width = ImGui::GetWindowSize().x;

	// ボタンを中央揃えにするためのx位置を計算
	float x_pos = (window_width - button_size.x) / 2.0f;

	// 位置を設定してボタンを描画
	ImGui::SetCursorPosX(x_pos);
	return ImGui::SmallButton(label.c_str());
}

inline bool CenterButton(std::string label, ImVec2 size = {})
{
	//ボタンのサイズを取得
	ImVec2 button_size = { size.x,size.y };
	if (size == ImVec2())
	{
		button_size = ImGui::CalcTextSize(label.c_str());
		button_size.x += ImGui::GetStyle().FramePadding.x * 2.0f;
		button_size.y += ImGui::GetStyle().FramePadding.y * 2.0f;
	}

	// ツリーノードの幅を取得
	float tree_node_width = ImGui::GetContentRegionAvail().x;

	// ボタンを中央揃えにするためのx位置を計算
	float x_pos = (tree_node_width - button_size.x) / 2.0f;

	// 位置を設定してボタンを描画
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + x_pos);
	return ImGui::Button(label.c_str(), size);
}

inline void ImageWindowCenter(ID3D11ShaderResourceView* _tex, ImVec2 _size, ImVec2* _resultPos = nullptr)
{
	// 画面中央に固定するためのオフセット計算
	ImVec2 imagePos = ((ImGui::GetContentRegionAvail() - _size) * 0.5f);

	// 位置を設定
	ImGui::SetCursorPos(imagePos);

	ImGui::Image(_tex, _size);

	if (_resultPos)*_resultPos = imagePos;
}
/**
 * @fn
 * @brief Itemを親ウィンドウの中心に描画する
 * @param (_size) これから表示するItemのサイズ
 */
inline void SetCenterCursorPos(ImVec2 _size = ImVec2(), ImVec2* _resultPos = nullptr)
{
	// 画面中央に固定するためのオフセット計算
	ImVec2 imagePos = ((ImGui::GetContentRegionAvail() - _size) * 0.5f);

	// 位置を設定
	ImGui::SetCursorPos(imagePos);

	if (_resultPos)*_resultPos = imagePos;
}

inline void SetCenterCursorWidth(float _size = 0.0f)
{
	// 画面中央に固定するためのオフセット計算
	float imageX = ((ImGui::GetContentRegionAvail().x - _size) * 0.5f);
	// 位置を設定
	ImGui::SetCursorPosX(imageX);
}

inline bool CheckBoxBit(std::string _name, UINT& _ID, UINT _checkID)
{
	bool flg = _ID & _checkID;
	bool change = ImGui::Checkbox(_name.c_str(), &flg);
	if (flg) _ID |= _checkID;
	else _ID &= (~_checkID);
	return change;
}

template<class T> struct  DragDrop
{
	std::list<std::function<bool(T)>>  source;
	std::list<std::function<bool(T&)>> target;
	bool CallSource(T _value)
	{
		bool flg = false;
		for (auto& Fn : source)flg |= Fn(_value);
		return flg;
	}
	bool CallTarget(T& _value)
	{
		bool flg = false;
		for (auto& Fn : target)flg |= Fn(_value);
		return flg;
	}
};
template<class T>
inline bool DragDropSource(std::string_view _tag, T _payload)
{
	bool flg = false;
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload(_tag.data(), &_payload, sizeof(T), ImGuiCond_Once);
		ImGui::Text(_tag.data());

		flg = true;

		ImGui::EndDragDropSource();
	}
	return flg;
}
inline bool DragDropSource(std::string_view _tag, std::string _str, ImGuiDragDropFlags_ _flag = ImGuiDragDropFlags_::ImGuiDragDropFlags_None)
{
	bool flg = false;
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::SetDragDropPayload(_tag.data(), _str.c_str(), _str.size() + 1, _flag);
		ImGui::Text(_tag.data());

		flg = true;

		ImGui::EndDragDropSource();
	}
	return flg;
}

template<class T>
inline bool DragDropTarget(std::string_view _tag, T& _payload)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_tag.data()))
		{
			IM_ASSERT(payload->DataSize == sizeof(T));
			_payload = *(T*)payload->Data;
			flg = true;
		}
		ImGui::EndDragDropTarget();
	}
	return flg;
}
inline  bool DragDropTarget(std::string_view _tag, std::string& _str)
{
	bool flg = false;
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(_tag.data()))
		{
			const char* payloadData = static_cast<const char*>(payload->Data);
			std::string receivedStr(payloadData);
			_str = receivedStr;
			flg = true;
		}
		ImGui::EndDragDropTarget();
	}
	return flg;
}

inline bool IsDragging(std::string_view _id)
{
	auto payload = ImGui::GetDragDropPayload();
	if (!payload)return false;
	if (!payload->IsDataType(_id.data()))return false;
	return true;
}
template<class T>
inline bool GetDragData(std::string_view _id, T& _result)
{
	auto payload = ImGui::GetDragDropPayload();
	if (!payload)return false;
	if (payload->IsDataType(_id.data())) {
		IM_ASSERT(payload->DataSize == sizeof(T));
		_result = *(T*)payload->Data;
		return true;
	}
	return false;
}

inline bool IsItemMouseHovering()
{
	auto payload = ImGui::GetDragDropPayload();
	return ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()) && payload ? ImGui::GetItemID() != payload->SourceId : false;
}

#ifdef NEARGYE_MAGIC_ENUM_HPP
template<typename Enum, std::integral Number>
inline bool CheckBoxAllBit(Number& _ID)
{
	static_assert(std::is_enum_v<Enum>, "指定した型がEnumではありませんでした");

	bool change = false;
	for (auto& it : magic_enum::enum_names<Enum>())
	{
		Number checkID = (Number)magic_enum::enum_cast<Enum>(it).value();

		bool bitFlg = _ID & checkID;
		if (ImGui::Checkbox(it.data(), &bitFlg))
		{
			if (bitFlg) _ID |= checkID;
			else _ID &= (~checkID);

			change = true;
		}
	}


	return change;
}
template<typename Enum>
inline bool CheckBoxAllBit(Enum& _ID)
{
	static_assert(std::is_enum_v<Enum>, "指定した型がEnumではありませんでした");

	bool change = false;
	for (auto& it : magic_enum::enum_names<Enum>())
	{
		Enum checkID = magic_enum::enum_cast<Enum>(it).value();

		bool bitFlg = _ID & checkID;
		if (ImGui::Checkbox(it.data(), &bitFlg))
		{
			if (bitFlg) _ID = (Enum)(_ID | checkID);
			else		_ID = (Enum)(_ID & (~checkID));

			change = true;
		}
	}


	return change;
}

template<typename Enum, std::integral Number>
inline bool BitEditor(Number& _ID, std::string_view _name)
{
	static_assert(std::is_enum_v<Enum>, "指定した型がEnumではありませんでした");
	bool change = false;

	std::string ID = "##" + std::to_string((int)&_ID);
	if (ImGui::BeginCombo(ID.data(), _name.data()))
	{
		change = CheckBoxAllBit<Enum>(_ID);
		ImGui::EndCombo();
	}

	return change;
}
template<typename Enum>
inline bool BitEditor(Enum& _ID, std::string_view _name)
{
	static_assert(std::is_enum_v<Enum>, "指定した型がEnumではありませんでした");
	bool change = false;

	std::string ID = "##" + std::to_string((int)&_ID);
	if (ImGui::BeginCombo(ID.data(), _name.data()))
	{
		change =
			CheckBoxAllBit<Enum>(_ID);
		ImGui::EndCombo();
	}

	return change;
}

template<typename T>
inline bool ComboEnum(std::string_view _lable, T& _enum)
{
	static_assert(std::is_enum_v<T>, "指定した型がEnum型ではありません");

	bool edited = false;

	if (ImGui::BeginCombo(_lable.data(), magic_enum::enum_name(_enum).data()))
	{
		for (auto& it : magic_enum::enum_names<T>())
		{
			if (ImGui::Selectable(it.data()))
			{
				_enum = magic_enum::enum_cast<T>(it).value();
				edited = true;
				break;
			}
		}
		ImGui::EndCombo();
	}

	return false;
}
#endif

template<typename Vector_OR_List, typename ValueType = typename Vector_OR_List::value_type>
concept IsVector_OR_List = std::disjunction_v<
	std::is_same<std::list<ValueType>, Vector_OR_List>,
	std::is_same<std::vector<ValueType>, Vector_OR_List>
>;

/**
 * @fn
 * @brief 要素数が単一のコンテナクラスから文字列を取り出してコンボに並べる
 * @param container 要素型が一種類のコンテナ
 * @param ImGuiのタグ
 * @param selected_value 変更対象
 * @param callback_fn コンテナの要素型から文字列を取り出すファンクション
 * @return selected_valueに変更があったか
 * @detail ListとVectorにしか対応してない
 */
template<IsVector_OR_List Vector_OR_List>
inline bool Combo(
	const Vector_OR_List& container,
	std::string_view label,
	std::string& selected_value,
	std::function<std::string_view(const typename Vector_OR_List::value_type&)> callback_fn
)
{
	bool is_edited = false;

	if (ImGui::BeginCombo(label.data(), selected_value.c_str())) {
		for (const auto& item : container) {
			std::string_view str = callback_fn(item);
			if (ImGui::Selectable(str.data())) {
				selected_value = str.data();
				is_edited = true;
				break;
			}
		}
		ImGui::EndCombo();
	}

	return is_edited;
};
inline bool Combo(const std::list<std::string>& _list, std::string_view _tag, std::string& _value)
{
	bool is_edited = false;

	if (ImGui::BeginCombo(_tag.data(), _value.c_str()))
	{
		for (auto& it : _list)
		{
			if (ImGui::Selectable(it.c_str()))
			{
				_value = it;
				is_edited = true;
				break;
			}
		}
		ImGui::EndCombo();
	}
	return is_edited;
}

/**
 * @fn
 * @brief 指定範囲のストリングを格納したイテレーターでComboを作る
 * @param label ImGuiのタグ
 * @param begin
 * @param end
 *  * @param callback_fn コンテナの要素型から文字列を取り出すファンクション
 * @param selected_value 変更対象
 * @return selected_valueに変更があったか
 */

template<class T>  inline bool Combo(
	std::string_view label,
	T _begin,
	T _end,
	std::function<std::string(T)> callback_fn,
	std::string& selected_value
)
{
	bool is_edited = false;

	if (ImGui::BeginCombo(label.data(), selected_value.c_str())) {
		while (_begin != _end)
		{
			std::string str = callback_fn(_begin);
			if (str.empty()) { _begin++; continue; }
			if (ImGui::Selectable(str.data())) {
				selected_value = str.data();
				is_edited = true;
				break;
			}
			_begin++;
		}
		ImGui::EndCombo();
	}

	return is_edited;
};

/**
 * @fn
 * @brief  マップのKeyの一覧と要素の削除
 * @param  ImGuiのタグ
 * @param  マップ
 * @param  変更対象
 * @return 変更があったか
 * @detail std::Map型のkeyがstring限定
 */
template<class ValueType>  inline bool MapTable(
	std::string_view _lable,
	std::map<std::string, ValueType>& _map,
	ValueType** selected_value
)
{
	bool flg = false;
	if (_map.size() && ImGui::BeginTable(_lable.data(), 2))
	{
		for (auto it = _map.begin();
			it != _map.end();
			) {
			ImGui::TableNextRow();

			ImGui::TableNextColumn();
			std::string str = it->first.data();
			if (ImGui::Selectable(it->first.data()))ImGui::OpenPopup((_lable.data() + str).data());

			if (ImGui::IsItemClicked()) { *selected_value = &it->second; }

			ImGui::TableNextColumn();
			if (ImGui::Selectable(("Delete##" + std::to_string((int)&it->first)).data()))
			{
				if (*selected_value == &it->second)*selected_value = nullptr;
				it = _map.erase(it);
				continue;
			}

			bool edied = false;
			if (ImGui::BeginPopup((str + _lable.data()).data()))
			{
				edied = ImGui::InputText(_lable.data(), str.data());
				ImGui::EndPopup();
			}

			if (edied)
			{
				_map[str] = it->second;
				_map.erase(it);
				flg = true;
				break;
			}

			it++;
		}

		ImGui::EndTable();
	}
	return flg;
}