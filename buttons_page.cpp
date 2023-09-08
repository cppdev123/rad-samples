#include "example.h"
#include <rad/ui/material3/button.h>
#include <rad/ui/material3/icon_button.h>
#include <rad/ui/material3/fabutton.h>
#include <rad/ui/material3/switch.h>
#include <rad/ui/layout.h>

using namespace rad;
using namespace ui;

struct ButtonsPage : public VerticalBox {
	
	material::Switch enabled_switch;

	ButtonsPage() {
		using namespace std::string_view_literals;
		using material::ButtonType;
		using material::IconButtonType;
		using material::FABColor;

		const auto button_types = std::array{ ButtonType::elevated, ButtonType::filled, ButtonType::filled_tonal,
			ButtonType::outlined, ButtonType::text };

		const auto names = std::array{ "Elevated"sv, "Filled"sv, "Filled Tonal"sv, "Outlined"sv, "Text"sv };

		size_t name_i = 0;
		for (ButtonType btype : button_types) {
			auto hlay = add_child(std::make_unique<HorizontalLayout>());
			hlay->spacing = 10;
			hlay->padding.set(5);
			
			auto btn = hlay->add_child(std::make_unique<material::Button>());
			btn->type = btype;
			btn->enabled = enabled_switch.selected;
			btn->text() = std::format("{} Button Without Icons", names[name_i]);

			btn = hlay->add_child(std::make_unique<material::Button>());
			btn->type = btype;
			btn->enabled = enabled_switch.selected;
			btn->text() = std::format("{} Button With Icons", names[name_i]);
			btn->icon() = Pixmap{ "settings_FILL1_wght400_GRAD0_opsz24.png" };

			hlay->add_child(std::make_unique<HorizontalSpacer>());

			name_i += 1;
		}

		const auto icon_button_types = std::array{ IconButtonType::filled, IconButtonType::filled_tonal,
			IconButtonType::outlined, IconButtonType::standard };

		Pixmap outlined_settings{ "outline_settings_black_24dp.png" };
		Pixmap filled_settings{ "baseline_settings_black_24dp.png" };
		auto icons_hlay = add_child(std::make_unique<HorizontalLayout>());
		icons_hlay->spacing = 10;
		icons_hlay->padding.set(5);

		for (auto ibtype : icon_button_types) {
			auto ibtn1 = icons_hlay->add_child(std::make_unique<material::IconButton>());
			ibtn1->type = ibtype;
			ibtn1->enabled = enabled_switch.selected;
			ibtn1->icon() = outlined_settings;

			auto ibtn2 = icons_hlay->add_child(std::make_unique<material::IconButton>());
			ibtn2->type = ibtype;
			ibtn2->enabled = enabled_switch.selected;
			ibtn2->toggle = true;
			ibtn2->icon() = if_else(ibtn2->toggle && ibtn2->selected, filled_settings, outlined_settings);

			icons_hlay->add_child(std::make_unique<HorizontalSpacer>());
		}

		const auto fab_colors = std::array{ FABColor::normal , FABColor::surface, FABColor::secondary };

		Pixmap outlined_pencil{ "outline_create_black_36dp.png" };
		
		auto fabs_hlay = add_child(std::make_unique<HorizontalBox>());
		fabs_hlay->vertical_alignment = LayoutAlignment::center;

		for (auto fc : fab_colors) {
			auto fab1 = fabs_hlay->add_child(std::make_unique<material::FAButton>());
			fab1->icon() = outlined_pencil;
			fab1->type = material::FABType::normal;
			fab1->color_mapping = fc;
			
			auto fab2 = fabs_hlay->add_child(std::make_unique<material::FAButton>());
			fab2->icon() = outlined_pencil;
			fab1->type = material::FABType::small_btn;
			fab2->color_mapping = fc;
			
			auto fab3 = fabs_hlay->add_child(std::make_unique<material::FAButton>());
			fab3->icon() = outlined_pencil;
			fab3->type = material::FABType::large;
			fab3->color_mapping = fc;
		}

		enabled_switch.selected = true;
		add_child(enabled_switch);
	}
};

std::unique_ptr<Item> demo::make_buttons_page() {
	return std::make_unique<ButtonsPage>();
}