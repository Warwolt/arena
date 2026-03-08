#include <rktest/rktest.h>

#include "engine/ui.h"

TEST_SETUP(UITests) {
	UI_initialize();
}

TEST(UITests, Begin_End_MustBePaired) {
	UI_begin();
	EXPECT_DEATH(UI_begin(), "*UI_begin() called while in ui frame. Missing call to UI_end()?");
}
