#include "example.h"
#include <rad/ui/material3/radio_button.h>
#include <rad/ui/radio_buttons_group.h>
#include <rad/ui/layout.h>

using namespace rad;
using namespace ui;

struct RadioButtonsPage : public VerticalBox {
	Text text1;
	Text text2;
	
	static void style_text(Text& t) {
		t.font_family = material::md.sys.typescale.title_large.font;
		t.font_size = material::md.sys.typescale.title_large.size;
		t.font_wieght = material::md.sys.typescale.title_large.weight;
	}

	RadioButtonsPage() {
		horizontal_alignment = LayoutAlignment::center;
		vertical_alignment = LayoutAlignment::center;

		text1.text = "Radio Buttons Without Group";
		text2.text = "Radio Buttons Inside Group";
		style_text(text1);
		style_text(text2);

		add_child(text1);

		auto hlay = add_child(std::make_unique<HorizontalBox>());
		for (auto i : range(4)) {
			auto rbutton = hlay->add_child(std::make_unique<material::RadioButton>());
			rbutton->text() = std::format("radio button {}", i + 1);
		}

		add_child(text2);

		auto radios_group = RadioButtonsGroup::make();
		hlay = add_child(std::make_unique<HorizontalBox>());
		hlay->padding.bottom = 30;
		for (auto i : range(4)) {
			std::ignore = i;
			auto rbutton = hlay->add_child(std::make_unique<material::RadioButton>(radios_group));
			rbutton->text() = std::format("radio button {}", i + 1);
		}
	}
};


std::unique_ptr<Item> demo::make_radio_buttons_page() {
	return std::make_unique<RadioButtonsPage>();
}