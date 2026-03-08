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

TEST(UITests, Menu_Begin_Without_End_Gives_Error) {
	UI_begin();
	{
		UI_menu_begin("test menu 1");
		UI_menu_end();

		UI_menu_begin("test menu 2");
	}
	EXPECT_DEATH(UI_end(), "*Menu \"test menu 2\" has missing UI_menu_end() call");
}

TEST(UITests, Menu_End_Without_Open_Gives_Error) {
	UI_begin();
	EXPECT_DEATH(UI_menu_end(), "*UI_menu_end() called without corresponding UI_menu_begin()");
}

// menu items can be accessed
