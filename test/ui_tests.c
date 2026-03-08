#include <rktest/rktest.h>

#include "engine/ui.h"

TEST_SETUP(UITests) {
	UI_initialize();
}

TEST(UITests, UI_Begin_Without_End_Gives_Error) {
	UI_begin();
	EXPECT_DEATH(UI_begin(), "*UI_begin() called while already in ui frame. Missing call to UI_end()?");
}

TEST(UITests, UI_End_Without_Begin_Gives_Error) {
	EXPECT_DEATH(UI_end(), "*UI_end() called while outside ui frame. Missing call to UI_begin()?");
}

TEST(UITests, Menu_Unclosed_Gives_Error) {
	UI_begin();
	{
		UI_menu_begin("test menu");
	}
	EXPECT_DEATH(UI_end(), "*Menu 0 has missing UI_menu_end() call");
}

// UI_menu_end() gives error
TEST(UITests, Menu_Closed_Without_Open_Gives_Error) {
	EXPECT_DEATH(UI_menu_end(), "*UI_menu_end() called without UI_menu_begin()");
}
