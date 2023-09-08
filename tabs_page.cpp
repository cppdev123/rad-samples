#include "example.h"
#include <rad/ui/material3/button.h>
#include <rad/ui/material3/checkbox.h>
#include <rad/ui/material3/tabsview.h>
#include <rad/ui/layout.h>

using namespace rad;
using namespace ui;



std::unique_ptr<Item> demo::make_tabs_page() {
	auto tabsview = std::make_unique<material::TabsView>();

    for (auto i : rad::range(4)) {
        using namespace std::string_view_literals;
        const auto orders = std::array{ "first"sv, "second"sv, "third"sv, "fourth"sv };
        const auto names = std::array{ "Flights"sv, "Settings"sv, "Explore"sv, "Info"sv };
        const auto icons = std::array{ "baseline_flight_black_24dp.png"sv,
            "baseline_settings_black_18dp.png"sv, "outline_explore_black_24dp.png"sv,
            "baseline_info_black_18dp.png"sv };

        auto tab = std::make_unique<VerticalBox>();
        auto t1 = tab->add_child(std::make_unique<Text>());
        t1->text = SharedString("This text is in the " + std::string{ orders[i] } + " tab");
        t1->font_size = 20;
        auto b = tab->add_child(std::make_unique<material::Button>());
        b->text() = "Click the " + std::string{ orders[i] } + " tab !";
        auto c = tab->add_child(std::make_unique<material::CheckBox>());
        c->text() = if_else(c->selected, SharedString{ "Checked !" }, SharedString{ "Un Checked" });
        tabsview->add_tab(names[i], Pixmap{ icons[i] }, std::move(tab));
    }

	return tabsview;
}