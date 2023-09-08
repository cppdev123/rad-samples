#include "example.h"
#include <rad/ui/app.h>
#include <rad/ui/backends/windows/window.h>
#include <rad/ui/rectangle.h>
#include <rad/ui/material3/top_app_bar.h>
#include <rad/ui/material3/drawer.h>
#include <rad/ui/swipeview.h>

using namespace rad::ui;

namespace rad::ui::demo {
    struct MaterialApp : public Item {

        MaterialApp() {
            add_child(swipe);
            add_child(top_bar);
            add_child(drawer);

            top_bar.width = width;
            swipe.y = top_bar.height;
            drawer.fill_parent();

            min_width = max(max(top_bar.min_width, swipe.min_width), 400);
            min_height = top_bar.min_height + swipe.min_height;// +100;

            top_bar.headline() = "Example App";
            top_bar.leading_icon() = Pixmap{ "menu_FILL0_wght400_GRAD0_opsz24.png" };
            top_bar.leading_icon_clicked() = [this](ClickEvent ev) {
                if (ev.button != MouseButtons::left)
                    return;
                drawer.open();
            };

            swipe.width = width;
            swipe.height = height - swipe.y;

            drawer.add_item("Button And Counter", Pixmap{ "counter_1_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Account Form", Pixmap{ "account_box_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Todo List", Pixmap{ "menu_book_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Progress And Sliders", Pixmap{ "sliders_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Radio Buttons", Pixmap{ "radio_button_checked_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Buttons", Pixmap{ "dialpad_FILL1_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Text Area", Pixmap{ "dialpad_FILL1_wght400_GRAD0_opsz24.png" });
            drawer.add_item("Tabs", Pixmap{ "tabs_FILL0_wght400_GRAD0_opsz24.png" });
            drawer.on_item_clicked = [this](size_t i) {
                swipe.current_index = static_cast<int>(i);
            };

            add_swipe_tab(demo::make_increment_btn_page());
            add_swipe_tab(demo::make_form_page());
            add_swipe_tab(demo::make_todo_list());
            add_swipe_tab(demo::make_progress_and_sliders());
            add_swipe_tab(demo::make_radio_buttons_page());
            add_swipe_tab(demo::make_buttons_page());
            add_swipe_tab(demo::make_text_area_page());
            add_swipe_tab(demo::make_tabs_page());
        }

        void add_swipe_tab(std::unique_ptr<Item> tab_content) {
            auto tab = std::make_unique<SwipeViewTab>();
            tab->set_layout(std::move(tab_content));
            swipe.add_tab(std::move(tab));
        }

        material::TopAppBar top_bar;
        SwipeView swipe;
        material::Drawer drawer;
    };
}

std::vector<uint8_t> read_all_file(const std::string& path) {
    using namespace rad::io::files;
    file f{ path, open_mode::existing, access::read };
    std::vector<uint8_t> buff;
    buff.resize(f.size());
    f.read_all(rad::buffer(buff));
    return buff;
}

#include <iostream>

int main() {
    try {
        Application::inst().add_font(read_all_file("Roboto-Regular.ttf"));

        WindowImpl window;
        auto bg = window.add_child(std::make_unique<rad::ui::Rectangle>());
        bg->fill_parent();
        bg->color = material::md.sys.color.surface;

        auto take_focus = bg->add_child(std::make_unique<MouseArea>());
        take_focus->on_pressed = [b = bg.get()](const Point2DF&) { b->focus(); };

        auto demo_app = window.set_layout(std::make_unique<demo::MaterialApp>());
        window.show();

        Application::inst().exec();
    }
    catch (const std::exception& ex) {
        std::cout << "application has failed ! " << ex.what() << "\npress any key to exit ...";
        (void)getchar();
    }
	return 0;
}