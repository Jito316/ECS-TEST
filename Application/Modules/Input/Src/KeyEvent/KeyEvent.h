#pragma once

static std::string_view KEYSTAG = "Window";

class KeyEvent
{
public:
	KeyEvent(std::string_view _name, std::string_view _id = "") 
		: m_name(_name.data())
		, m_buttonID(_id)
	{
		static KeyRegister keyRegister;
	};

	const bool IsFree() const;
	const bool IsPress() const;
	const bool IsHold() const;
	const bool IsRelease() const;

	void Serialize(nlohmann::json& _json);
	void Deserialize(const nlohmann::json& _json);
	void ImGuiUpdate();

private:
	std::string  m_name;
	std::string m_buttonID;

	struct KeyRegister
	{
		KeyRegister()
		{
			std::unique_ptr<KdInputCollector> pWindowsInput = std::make_unique<KdInputCollector>();
			std::shared_ptr<KdInputButtonForWindows> defaultKey;
			std::vector<int> keyCode = {};
			//ボタン
			{
				keyCode = { 'Q' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("Q", defaultKey);
			}
			{
				keyCode = { 'W' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("W", defaultKey);
			}
			{
				keyCode = { 'E' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("E", defaultKey);
			}
			{
				keyCode = { 'R' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("R", defaultKey);
			}
			{
				keyCode = { 'T' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("T", defaultKey);
			}
			{
				keyCode = { 'Y' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("Y", defaultKey);
			}
			{
				keyCode = { 'U' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("U", defaultKey);
			}
			{
				keyCode = { 'I' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("I", defaultKey);
			}
			{
				keyCode = { 'O' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("O", defaultKey);
			}
			{
				keyCode = { 'P' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("P", defaultKey);
			}
			{
				keyCode = { 'A' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("A", defaultKey);
			}
			{
				keyCode = { 'S' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("S", defaultKey);
			}
			{
				keyCode = { 'D' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("D", defaultKey);
			}
			{
				keyCode = { 'F' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("F", defaultKey);
			}
			{
				keyCode = { 'G' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("G", defaultKey);
			}
			{
				keyCode = { 'H' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("H", defaultKey);
			}
			{
				keyCode = { 'J' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("J", defaultKey);
			}
			{
				keyCode = { 'K' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("K", defaultKey);
			}
			{
				keyCode = { 'L' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("L", defaultKey);
			}
			{
				keyCode = { 'Z' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("Z", defaultKey);
			}
			{
				keyCode = { 'X' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("X", defaultKey);
			}
			{
				keyCode = { 'C' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("C", defaultKey);
			}
			{
				keyCode = { 'V' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("V", defaultKey);
			}
			{
				keyCode = { 'B' };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("B", defaultKey);
			}
			{
				keyCode = { 'N' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("N", defaultKey);
			}
			{
				keyCode = { 'M' };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("M", defaultKey);
			}
			{
				keyCode = { VK_SPACE };
				defaultKey = std::make_shared<KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("SPACE", defaultKey);
			}
			{
				keyCode = { VK_RETURN };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("ENTER", defaultKey);
			}
			{
				keyCode = { VK_BACK };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("BACK", defaultKey);
			}
			{
				keyCode = { VK_ESCAPE };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("ESC", defaultKey);
			}
			{
				keyCode = { VK_UP };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("UP", defaultKey);
			}
			{
				keyCode = { VK_DOWN };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("DOWN", defaultKey);
			}
			{
				keyCode = { VK_LEFT };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("LEFT", defaultKey);
			}
			{
				keyCode = { VK_RIGHT };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("RIGHT", defaultKey);
			}
			{
				keyCode = { VK_SHIFT };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("SHIFT", defaultKey);
			}
			{
				keyCode = { VK_CONTROL };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("CTRL", defaultKey);
			}
			{
				keyCode = { VK_TAB };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("TAB", defaultKey);
			}
			{
				keyCode = { VK_LBUTTON };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("LBUTTON", defaultKey);
			}
			{
				keyCode = { VK_RBUTTON };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("RBUTTON", defaultKey);
			}
			{
				keyCode = { VK_MBUTTON };
				defaultKey = std::make_shared < KdInputButtonForWindows>(keyCode);
				pWindowsInput->AddButton("MBUTTON", defaultKey);
			}



			//十字登録

			std::shared_ptr<KdInputAxisForWindows> spMove =
				std::make_shared<KdInputAxisForWindows>(
					pWindowsInput->GetButton("W"),
					pWindowsInput->GetButton("D"),
					pWindowsInput->GetButton("S"),
					pWindowsInput->GetButton("A"));
			pWindowsInput->AddAxis("move", spMove);


			 Kernel::GetEngine()->m_kdInputManager.AddDevice(KEYSTAG, pWindowsInput);
		}
	};
};

class KeyEvents
{
public:
	std::shared_ptr<const KeyEvent> operator[](std::string_view key);


	void Serialize(nlohmann::json& _json);
	void Deserialize(const nlohmann::json& _json);

	void ImGuiUpdate();

private:
	std::map<std::string, std::shared_ptr<KeyEvent>> m_keyMap;
};