#include "example.h"
#include <rad/ui/layout.h>
#include <rad/ui/text.h>
#include <rad/ui/material3/fabutton.h>

using namespace rad::ui;

struct IncrementBtnPage : public VerticalLayout {
	Property<int> plus_counter;
	Property<int> minus_counter;
	Property<int> net_counter;
	Text text;
	material::FAButton inc_button;
	material::FAButton dec_button;

	IncrementBtnPage() {
		spacing = 20;
		padding.set(20);
		horizontal_alignment = LayoutAlignment::center;
		vertical_alignment = LayoutAlignment::center;

		text.text = format("Pressed plus {} times !\nPressed minus {} times !\nNet press {} !",
			plus_counter, minus_counter, net_counter);

		text.font_size = 20;
		text.horizontal_alignment = TextHorizontalAlignment::center;
		text.vertical_alignment = TextVerticalAlignment::center;

		inc_button.clicked = [this](ClickEvent ev) {
			if (ev.button != MouseButtons::left)
				return;
			plus_counter = plus_counter() + 1;
		};
		inc_button.icon() = Pixmap{ "add_FILL0_wght400_GRAD0_opsz24.png" };
		inc_button.color_mapping = material::FABColor::surface;

		dec_button.clicked = [this](ClickEvent ev) {
			if (ev.button != MouseButtons::left)
				return;
			minus_counter = minus_counter() + 1;
		};
		dec_button.icon() = Pixmap{ "remove_FILL0_wght400_GRAD0_opsz24.png" };

		net_counter = plus_counter - minus_counter;

		add_child(text);
		auto hlay = add_child(std::make_unique<HorizontalLayout>());
		hlay->spacing = 20;
		hlay->padding.set(20);
		hlay->add_child(dec_button);
		hlay->add_child(std::make_unique<HorizontalSpacer>());
		hlay->add_child(inc_button);
	}
};

std::unique_ptr<Item> demo::make_increment_btn_page() {
	return std::make_unique<IncrementBtnPage>();
}