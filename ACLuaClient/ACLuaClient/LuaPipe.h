#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Logger.h"

const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
	7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
	0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

std::vector<char> b64decode(const void* data, const size_t len)
{
	unsigned char* p = (unsigned char*)data;
	int pad = len > 0 && (len % 4 || p[len - 1] == '=');
	const size_t L = ((len + 3) / 4 - pad) * 4;
	std::vector<char> str(L / 4 * 3 + pad, '\0');

	for (size_t i = 0, j = 0; i < L; i += 4)
	{
		int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
		str[j++] = n >> 16;
		str[j++] = n >> 8 & 0xFF;
		str[j++] = n & 0xFF;
	}
	if (pad)
	{
		int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
		str[str.size() - 1] = n >> 16;

		if (len > L + 2 && p[L + 2] != '=')
		{
			n |= B64index[p[L + 2]] << 6;
			str.push_back(n >> 8 & 0xFF);
		}
	}
	return str;
}

struct ScriptData
{
	bool failed = false;
	DWORD bytes = 0;
	std::vector<char> script;
};

class LuaPipe
{
	LPCWSTR pipeName;
	HANDLE pipeHandle = INVALID_HANDLE_VALUE;

public:
	LuaPipe(LPCWSTR _pipeName)
	{
		pipeName = _pipeName;
	}

	void init()
	{
		while (pipeHandle == INVALID_HANDLE_VALUE)
			pipeHandle = CreateFileW(pipeName, GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		Logger::log("\f2Successfully attached");
	}

	DWORD getBytes()
	{
		DWORD bytes;
		PeekNamedPipe(pipeHandle, NULL, NULL, NULL, &bytes, NULL);

		return bytes;
	}

	ScriptData readScript()
	{
		ScriptData data;
		data.failed = false;

		DWORD bytes = getBytes();
		if (bytes == 0)
			return data;

		std::vector<char> buffer(bytes);
		memset(buffer.data(), 0, bytes);

		if (!ReadFile(pipeHandle, buffer.data(), bytes, NULL, NULL))
		{
			data.failed = true;
			return data;
		}

		std::vector<char> script = b64decode(buffer.data(), bytes);

		data.bytes = bytes;
		data.script = script;
		return data;
	}
};