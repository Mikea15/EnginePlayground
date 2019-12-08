
#pragma once

#define NOMINMAX
#include <windows.h>

enum class LEVEL : int
{
	NORMAL = 0,
	INFO,
	WARNING,
	ERR,
	FATAL
};

#define LOG(text, ...)			Logger::LOG(LEVEL::NORMAL,	text, __VA_ARGS__);
#define LOG_INFO(text, ...)		Logger::LOG(LEVEL::INFO,	text, __VA_ARGS__);
#define LOG_WARNING(text, ...)	Logger::LOG(LEVEL::WARNING, text, __VA_ARGS__);
#define LOG_ERROR(text, ...)	Logger::LOG(LEVEL::ERR,		text, __VA_ARGS__);
#define LOG_FATAL(text, ...)	Logger::LOG(LEVEL::FATAL,	text, __VA_ARGS__);

class Logger
{
private:
	Logger() = delete;

public:
	static void LOG(LEVEL level, const char* szFormat, ...)
	{
		static const unsigned int bufferSize = 1024;

		char szBuff[bufferSize];
		va_list arg;
		va_start(arg, szFormat);
		_vsnprintf_s(szBuff, bufferSize, szFormat, arg);
		va_end(arg);

		char warning[bufferSize];
		switch (level)
		{
		case LEVEL::INFO:
			strcpy_s(warning, "[Info] ");
			break;
		case LEVEL::WARNING:
			strcpy_s(warning, "[Warning] ");
			break;
		case LEVEL::ERR:
			strcpy_s(warning, "[Error] ");
			break;
		case LEVEL::FATAL:
			strcpy_s(warning, "[Fatal Error] ");
			break;
		case LEVEL::NORMAL:
		default:
			strcpy_s(warning, "[Log] ");
			break;
		}

		char finalBuffer[bufferSize];
		strcpy_s(finalBuffer, warning);
		strcat_s(finalBuffer, szBuff);
		strcat_s(finalBuffer, "\n");

		OutputDebugString(finalBuffer);
	}
};
