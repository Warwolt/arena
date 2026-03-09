#include <rktest/rktest.h>

#include "engine/ui.h"

TEST_SETUP(UITests) {
	UI_initialize();
}

TEST(UITests, UI_BeginWithoutEnd_GivesError) {
	UI_begin();
	EXPECT_DEATH(UI_begin(), "*UI_begin() called while already in ui frame. Missing call to UI_end()?");
}

TEST(UITests, UI_EndWithoutBegin_GivesError) {
	EXPECT_DEATH(UI_end(), "*UI_end() called while outside ui frame. Missing call to UI_begin()?");
}

TEST(UITests, Menu_BeginWithoutEnd_GivesError) {
	UI_begin();
	{
		UI_menu_begin("test menu 1");
		UI_menu_end();

		UI_menu_begin("test menu 2");
	}
	EXPECT_DEATH(UI_end(), "*Menu \"test menu 2\" has missing UI_menu_end() call");
}

TEST(UITests, Menu_NestedBegin_GivesError) {
	UI_begin();
	{
		UI_menu_begin("test menu 1");
		EXPECT_DEATH(UI_menu_begin("test menu 2"), "*UI_menu_begin() called while menu \"test menu 1\" already open. Menus cannot be nested. Missing call to UI_menu_end()?");
	}
}

TEST(UITests, Menu_EndWithoutOpen_GivesError) {
	UI_begin();
	EXPECT_DEATH(UI_menu_end(), "*UI_menu_end() called without corresponding UI_menu_begin()");
}

TEST(UITests, Menu_ItemsAddedToView) {
	UI_begin();
	{
		UI_menu_begin("test menu");
		{
			UI_menu_item("label 1");
			UI_menu_item("label 2");
			UI_menu_item("label 3");
		}
		UI_menu_end();

		UI_menu_begin("test menu 2");
		{
			UI_menu_item("label 4");
			UI_menu_item("label 5");
		}
		UI_menu_end();
	}
	UI_end();

	ASSERT_EQ(UI_view()->num_menus, 2);
	ASSERT_EQ(UI_view()->menus[0].num_items, 3);
	ASSERT_EQ(UI_view()->menus[1].num_items, 2);

	EXPECT_STREQ(UI_view()->menus[0].label, "test menu");
	EXPECT_STREQ(UI_view()->menus[0].items[0].label, "label 1");
	EXPECT_STREQ(UI_view()->menus[0].items[1].label, "label 2");
	EXPECT_STREQ(UI_view()->menus[0].items[2].label, "label 3");

	EXPECT_STREQ(UI_view()->menus[1].label, "test menu 2");
	EXPECT_STREQ(UI_view()->menus[1].items[0].label, "label 4");
	EXPECT_STREQ(UI_view()->menus[1].items[1].label, "label 5");
}

TEST(UITests, Menu_FirstItem_InitiallyFocused) {
	UI_begin();
	{
		UI_menu_begin("test menu");
		{
			UI_menu_item("item 1");
			UI_menu_item("item 2");
		}
		UI_menu_end();
	}
	UI_end();

	ASSERT_EQ(UI_view()->num_menus, 1);
	ASSERT_EQ(UI_view()->menus[0].num_items, 2);
	EXPECT_TRUE(UI_view()->menus[0].items[0].is_focused);
	EXPECT_FALSE(UI_view()->menus[0].items[1].is_focused);
}

// menu item interactivity
// how do we make that testable?
