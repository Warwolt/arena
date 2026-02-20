#pragma once

typedef enum LogLevel LogLevel;
enum LogLevel {
	LogLevel_Debug = 0,
	LogLevel_Info = 1,
	LogLevel_Warning = 2,
	LogLevel_Error = 3,
	LogLevel_Fatal = 4,
};

void initialize_logging();
void debug_log(LogLevel level, const char* filename, int line, const char* fmt, ...);
