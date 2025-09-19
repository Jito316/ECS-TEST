#include "KdCSVData.h"

struct KdCSVData::KdCSVPimple {
	std::vector<std::vector<std::string>> dataLines;

	std::string filePass;

	static const std::vector<std::string> c_nullDataList;
};

const std::vector<std::string> KdCSVData::KdCSVPimple::c_nullDataList;

KdCSVData::KdCSVData()
{
	m_pPimple = new KdCSVPimple;
}

KdCSVData::KdCSVData(const std::string_view filename)
{
	m_pPimple = new KdCSVPimple;
	Load(filename);
}
KdCSVData::~KdCSVData()
{
	delete m_pPimple;
}

bool KdCSVData::Load(const std::string_view filename)
{
	if (filename.empty()) { return false; }

	m_pPimple->filePass = filename.data();

	std::ifstream ifs(m_pPimple->filePass);

	if (!ifs)
	{ 
		assert(0 && "CSVDataが見つかりません");

		return false;
	}

	// 行ごとに分けてデータ格納
	while (1)
	{
		std::string rawLineData;
		if (!getline(ifs, rawLineData)) { break; }

		// [,]で分けて単語ごとにデータ格納
		std::vector<std::string> lineData;
		CommaSeparatedValue(rawLineData, lineData);

		m_pPimple->dataLines.push_back(lineData);
	}

	return true;
}

const std::vector<std::vector<std::string>>& KdCSVData::GetLData() const 
{
	return m_pPimple->dataLines;
}

// 行データを取得
const std::vector<std::string>& KdCSVData::GetLine(size_t index) const
{
	if (index >= m_pPimple->dataLines.size()) { return KdCSVPimple::c_nullDataList; }

	return m_pPimple->dataLines[index];
}

size_t KdCSVData::GetLineSize() const 
{
	return m_pPimple->dataLines.size();
}

// [,]で分けて単語リスト作成
void KdCSVData::CommaSeparatedValue(std::string_view line, std::vector<std::string>& result)
{
	std::istringstream stream(line.data());
	std::string element;

	while (getline(stream, element, ','))
	{
		result.push_back(element);
	}
}
