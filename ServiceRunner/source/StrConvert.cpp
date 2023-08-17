#include "StrConvert.h"
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <string>

std::wstring StrConvert::ConvertToWideString(const std::string& str) {
	int wideStrLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    if (wideStrLength == 0)
    {
        // Gagal mengonversi, tangani kesalahan di sini
        return L"";
    }

    std::wstring wideStr(wideStrLength, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], wideStrLength);

    // Menghilangkan null-terminator tambahan yang ditambahkan oleh MultiByteToWideChar
    wideStr.resize(wideStrLength - 1);

    return wideStr;
}

std::string StrConvert::toLowcase(const std::string& str) {
	std::string resultBuffer = str;
	std::transform(resultBuffer.begin(), resultBuffer.end(), resultBuffer.begin(), [](unsigned char c) { return std::tolower(c); });

	return resultBuffer;
}