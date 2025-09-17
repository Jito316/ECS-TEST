#pragma once
#ifndef H_BINARYHELPER
#define H_BINARYHELPER

#include <string>
#include <fstream>
#include <vector>

inline bool BinaryWrite(
	const std::string& _path
	, void* _buffer
	, size_t _size
)
{
	//ファイル名からバイナリファイルで読み込む
	std::ofstream ifs(_path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (ifs.is_open())
	{
		ifs.write(reinterpret_cast<char*>(_buffer), _size);
		return true;
	}
	return false;
}

inline bool BinaryRead(
	const std::string& _path
	, void* _buffer
	, size_t _size
)
{
	//ファイル名からバイナリファイルで読み込む
	std::ifstream ifs(_path, std::ios::in | std::ios::binary);
	if (ifs.is_open())
	{
		ifs.read(reinterpret_cast<char*>(_buffer), _size);
		return true;
	}
	return false;
}

template <typename T>
inline bool BinaryWriteVector(const std::string& _path, const std::vector<T>& _data)
{
	std::ofstream ofs(_path, std::ios::out | std::ios::trunc | std::ios::binary);
	if (!ofs.is_open())
		return false;

	// 要素数を書き込み
	size_t size = _data.size();
	ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// 要素本体を書き込み
	if (!_data.empty()) {
		ofs.write(reinterpret_cast<const char*>(_data.data()), sizeof(T) * size);
	}

	return true;
}

template <typename T>
inline bool BinaryReadVector(const std::string& _path, std::vector<T>& _data)
{
	std::ifstream ifs(_path, std::ios::in | std::ios::binary);
	if (!ifs.is_open())
		return false;

	// 要素数を読み込み
	size_t size = 0;
	ifs.read(reinterpret_cast<char*>(&size), sizeof(size));

	// ベクターをリサイズしてデータを読み込む
	_data.resize(size);
	if (size > 0) {
		ifs.read(reinterpret_cast<char*>(_data.data()), sizeof(T) * size);
	}

	return true;
}

class BinaryFileWriter
{
public:
	BinaryFileWriter() = default;
	BinaryFileWriter(const std::string& _path) { Start(_path); }
	~BinaryFileWriter() { End(); }

	// ファイルを開く
	inline bool Start(const std::string& _path)
	{
		if (!m_ofs.is_open())
			m_ofs.open(_path, std::ios::out | std::ios::trunc | std::ios::binary);
		return m_ofs.is_open();
	}

	// 書き込み
	inline bool Write(void* _buffer, size_t _size)
	{
		if (!m_ofs.is_open()) return false;
		m_ofs.write(reinterpret_cast<char*>(_buffer), _size);
		return m_ofs.good();
	}

	// ファイルを閉じる
	inline void End()
	{
		if (m_ofs.is_open())
		{
			m_ofs.close();
		}
	}

private:
	std::ofstream m_ofs;
};

class BinaryFileReader
{
public:
	BinaryFileReader() = default;
	BinaryFileReader(const std::string& _path) { Start(_path); };
	~BinaryFileReader() { End(); }

	// 開始（読み込み用に開く）
	inline bool Start(const std::string& _path)
	{
		if (!m_ifs.is_open())
			m_ifs.open(_path, std::ios::in | std::ios::binary);
		return m_ifs.is_open();
	}

	// 読み込み
	inline bool Read(void* _buffer, size_t _size)
	{
		if (!m_ifs.is_open()) return false;
		m_ifs.read(reinterpret_cast<char*>(_buffer), _size);
		return m_ifs.good();
	}

	// 終了（閉じる）
	inline void End()
	{
		if (m_ifs.is_open())
		{
			m_ifs.close();
		}
	}

private:
	std::ifstream m_ifs;
};

class BinaryFileVecotrWriter
{
public:
	BinaryFileVecotrWriter() = default;
	BinaryFileVecotrWriter(const std::string& _path) { Start(_path); }
	~BinaryFileVecotrWriter() { End(); }

	// ファイルを開く
	inline bool Start(const std::string& _path)
	{
		if (!m_ofs.is_open())
			m_ofs.open(_path, std::ios::out | std::ios::trunc | std::ios::binary);
		return m_ofs.is_open();
	}

	// 書き込み
	template<class T>
	inline bool Write(std::vector<T>& _data)
	{
		if (!m_ofs.is_open())return false;

		// 要素数を書き込み
		size_t size = _data.size();
		m_ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// 要素本体を書き込み
		if (!_data.empty()) {
			m_ofs.write(reinterpret_cast<const char*>(_data.data()), sizeof(T) * size);
		}

		return m_ofs.good();
	}

	template<>
	inline bool Write<bool>(std::vector<bool>& _data)
	{
		if (!m_ofs.is_open())return false;

		// 要素数を書き込み
		size_t size = _data.size();
		m_ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// 要素本体を書き込み
		if (!_data.empty()) {
			std::vector<uint8_t> temp(,);
			m_ofs.write(reinterpret_cast<const char*>(temp.data()), sizeof(bool) * size);
		}

		return m_ofs.good();
	}

	// ファイルを閉じる
	inline void End()
	{
		if (m_ofs.is_open())
		{
			m_ofs.close();
		}
	}

private:
	std::ofstream m_ofs;
};

class BinaryFileVecotrReader
{
public:
	BinaryFileVecotrReader() = default;
	BinaryFileVecotrReader(const std::string& _path) { Start(_path); };
	~BinaryFileVecotrReader() { End(); }

	// 開始（読み込み用に開く）
	inline bool Start(const std::string& _path)
	{
		if (!m_ifs.is_open())
			m_ifs.open(_path, std::ios::in | std::ios::binary);
		return m_ifs.is_open();
	}

	template <typename T>
	inline bool Read(std::vector<T>& _data)
	{
		if (!m_ifs.is_open())return false;

		// 要素数を読み込み
		size_t size = 0;
		m_ifs.read(reinterpret_cast<char*>(&size), sizeof(size));

		// ベクターをリサイズしてデータを読み込む
		_data.resize(size);
		if (size > 0) {
			m_ifs.read(reinterpret_cast<char*>(_data.data()), sizeof(T) * size);
		}

		return  m_ifs.good();
	}
	
	template <>
	inline bool Read<bool>(std::vector<bool>& _data)
	{
		if (!m_ifs.is_open())return false;

		// 要素数を読み込み
		size_t size = 0;
		m_ifs.read(reinterpret_cast<char*>(&size), sizeof(size));

		// ベクターをリサイズしてデータを読み込む
		_data.resize(size);
		if (size > 0) {
			_data;
			std::vector<uint8_t>& temp = *reinterpret_cast<std::vector<uint8_t>*>(&_data);
			m_ifs.read(reinterpret_cast<char*>(temp.data()), sizeof(bool) * size);
		}

		return  m_ifs.good();
	}

	// 終了（閉じる）
	inline void End()
	{
		if (m_ifs.is_open())
		{
			m_ifs.close();
		}
	}

private:
	std::ifstream m_ifs;
};

#endif