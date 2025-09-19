#pragma once
#include "Core.h"

struct CORE_API KdCSVData
{
	KdCSVData();
	KdCSVData(const std::string_view filename);
	~KdCSVData();

	bool Load(const std::string_view filename);

	const std::vector<std::vector<std::string>>& GetLData() const;

	const std::vector<std::string>& GetLine(size_t index) const;

	size_t GetLineSize() const;

private:
	void CommaSeparatedValue(std::string_view src, std::vector<std::string>& result);

	struct KdCSVPimple;
	KdCSVPimple* m_pPimple = nullptr;
};
