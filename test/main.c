#define DEFINE_RKTEST_IMPLEMENTATION
#include <rktest/rktest.h>

#include "core/debug/logging.h"
#include "platform/assert.h"


int main(int argc, const char* argv[]) {
	initialize_unit_test_logging();
	enable_unit_test_debug_exit();
	return rktest_main(argc, argv);
}
