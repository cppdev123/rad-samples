#pragma once
#include <rad/ui/item.h>

namespace rad::ui::demo {
	std::unique_ptr<Item> make_increment_btn_page();

	std::unique_ptr<Item> make_form_page();

	std::unique_ptr<Item> make_todo_list();

	std::unique_ptr<Item> make_progress_and_sliders();

	std::unique_ptr<Item> make_radio_buttons_page();

	std::unique_ptr<Item> make_buttons_page();

	std::unique_ptr<Item> make_text_area_page();

	std::unique_ptr<Item> make_tabs_page();
}