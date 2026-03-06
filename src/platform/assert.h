#pragma once

#include "platform/logging.h"

#include <stdio.h>

#define DEBUG_ASSERT(condition, ...)                                               \
	if (!(condition)) {                                                            \
		char msg[512];                                                             \
		int offset = sprintf_s(msg, 512, "DEBUG_ASSERT(%s) failed: ", #condition); \
		sprintf_s(msg + offset, 512 - offset, __VA_ARGS__);                        \
		LOG_FATAL("%s", msg);                                                      \
		debug_exit(msg);                                                           \
	}

void debug_exit(const char* message);
