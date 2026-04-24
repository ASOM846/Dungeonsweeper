#pragma once

#include <cassert>
#include <cstdio>
#include <ctime>
#include <raylib.h>

namespace util {
template <typename T> T GetRandom(T min, T max) {
	return static_cast<T>(
		GetRandomValue(static_cast<int>(min), static_cast<int>(max)));
}

template <typename T> T GetRandom(int min, T max) {
	return static_cast<T>(GetRandomValue(min, static_cast<int>(max)));
}

void Log(int msgType, const char *text, va_list args) {
	char timeStr[64] = {0};
	time_t now = time(NULL);
	struct tm *tm_info = localtime(&now);

	strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
	printf("[%s] ", timeStr);

	switch (msgType) {
	case LOG_INFO:
		printf("[INFO] : ");
		break;
	case LOG_ERROR:
		printf("[ERROR]: ");
		break;
	case LOG_WARNING:
		printf("[WARN] : ");
		break;
	case LOG_DEBUG:
		printf("[DEBUG]: ");
		break;
	default:
		break;
	}

	vprintf(text, args);
	printf("\n");
}

void LogMsg(int msgType, const char *text, ...) {
	va_list args;
	va_start(args, text);

	Log(msgType, text, args);

	va_end(args);
}
} // namespace util
