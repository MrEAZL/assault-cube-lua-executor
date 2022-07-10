#pragma once
#include "AC.h"
#include <string>

namespace Logger
{
	FILE* out;

	std::string prefix = "[Unnamed Executor] ";

	void log(const char* data)
	{
		std::string message(prefix);
		message.append(data);

		AC::chat(message.c_str(), nullptr);
		std::cout << message << std::endl;
	}

	void clog(const char* data)
	{
		std::string message(prefix);
		message.append(data);

		std::cout << message << std::endl;
	}

	void clog(void* data)
	{
		std::string message(prefix);
		message.append(std::to_string((DWORD)data));

		std::cout << message << std::endl;
	}

	template <typename T>
	void clog(T data)
	{
		std::cout << prefix << data << std::endl;
	}

	void setupConsole()
	{
		AllocConsole();
		freopen_s(&out, "conout$", "w", stdout);
	}
}