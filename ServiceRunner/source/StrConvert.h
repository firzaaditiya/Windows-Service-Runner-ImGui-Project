#include <string>

#pragma once

struct StrConvert {
	static std::wstring ConvertToWideString(const std::string& str);
	static std::string toLowcase(const std::string& str);
};