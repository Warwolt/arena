#include <rktest/rktest.h>

#include "engine/ui.h"

TEST_SETUP(UITests) {
	UI_initialize();
}

TEST(UITests, UI_BeginWithoutEnd_GivesError) {
	UI_begin((UIInput) { 0 });
	EXPECT_DEATH(UI_begin((UIInput) { 0 }), "*UI_begin() called while already in ui frame. Missing call to UI_end()?");
}

TEST(UITests, UI_EndWithoutBegin_GivesError) {
	EXPECT_DEATH(UI_end(), "*UI_end() called while outside ui frame. Missing call to UI_begin()?");
}

TEST(UITests, Menu_BeginWithoutEnd_GivesError) {
	UI_begin((UIInput) { 0 });
	{
		UI_menu_begin("test menu 1");
		UI_menu_end();

		UI_menu_begin("test menu 2");
	}
	EXPECT_DEATH(UI_end(), "*Menu \"test menu 2\" has missing UI_menu_end() call");
}

TEST(UITests, Menu_NestedBegin_GivesError) {
	UI_begin((UIInput) { 0 });
	{
		UI_menu_begin("test menu 1");
		EXPECT_DEATH(UI_menu_begin("test menu 2"), "*UI_menu_begin() called while menu \"test menu 1\" already open. Menus cannot be nested. Missing call to UI_menu_end()?");
	}
}

TEST(UITests, Menu_EndWithoutOpen_GivesError) {
	UI_begin((UIInput) { 0 });
	EXPECT_DEATH(UI_menu_end(), "*UI_menu_end() called without corresponding UI_menu_begin()");
}

TEST(UITests, Menu_ItemsAddedToView) {
	UI_begin((UIInput) { 0 });
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

TEST(UITests, Menu_Initially_FirstItemFocused) {
	UI_begin((UIInput) { 0 });
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
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[0].is_focused, true);
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[1].is_focused, false);
}

TEST(UITests, Menu_KeyDown_SecondItemFocused) {
	UIInput input[2] = {
		(UIInput) { 0 },
		(UIInput) { .down_pressed = true },
	};
	for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
		UI_begin(input[i]);
		{
			UI_menu_begin("test menu");
			{
				UI_menu_item("item 1");
				UI_menu_item("item 2");
			}
			UI_menu_end();
		}
		UI_end();
	}

	ASSERT_EQ(UI_view()->num_menus, 1);
	ASSERT_EQ(UI_view()->menus[0].num_items, 2);
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[0].is_focused, false);
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[1].is_focused, true);
}

TEST(UITests, Menu_ItemFocusWrapsAround) {
	UIInput input[3] = {
		(UIInput) { .down_pressed = true },
		(UIInput) { .down_pressed = true },
		(UIInput) { .down_pressed = true },
	};
	for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
		UI_begin(input[i]);
		{
			UI_menu_begin("test menu");
			{
				UI_menu_item("item 1");
				UI_menu_item("item 2");
			}
			UI_menu_end();
		}
		UI_end();
	}

	ASSERT_EQ(UI_view()->num_menus, 1);
	ASSERT_EQ(UI_view()->menus[0].num_items, 2);
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[0].is_focused, false);
	EXPECT_BOOL_EQ(UI_view()->menus[0].items[1].is_focused, true);
}

TEST(UITests, Menu_SelectFirstItem) {
	bool item_was_selected = false;
	UI_begin((UIInput) { .select_pressed = true });
	{
		UI_menu_begin("test menu");
		{
			if (UI_menu_item("item 1")) {
				item_was_selected = true;
			}
		}
		UI_menu_end();
	}
	UI_end();

	EXPECT_TRUE(item_was_selected);
}

TEST(UITests, Menu_SelectSecondItem) {
	bool item1_was_selected = false;
	bool item2_was_selected = false;
	UIInput input[2] = {
		(UIInput) { .down_pressed = true },
		(UIInput) { .select_pressed = true },
	};
	for (int i = 0; i < sizeof(input) / sizeof(input[0]); i++) {
		UI_begin(input[i]);
		{
			UI_menu_begin("test menu");
			{
				if (UI_menu_item("item 1")) {
					item1_was_selected = true;
				}
				if (UI_menu_item("item 2")) {
					item2_was_selected = true;
				}
			}
			UI_menu_end();
		}
		UI_end();
	}

	EXPECT_FALSE(item1_was_selected);
	EXPECT_TRUE(item2_was_selected);
}

TEST(UITests, Foo) {
	// Write some kind of test for item focus.
	// What should happen if we render "menu 1", then "menu 2", then "menu 1", should the state persist for menu 1?
	// If we render two menus in the same frame, how do we know which should receive keyboard input?
	FAIL();
}
