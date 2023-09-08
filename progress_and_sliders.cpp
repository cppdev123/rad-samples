#include "example.h"
#include <rad/ui/material3/progress_bar.h>
#include <rad/ui/material3/slider.h>
#include <rad/ui/material3/checkbox.h>
#include <rad/ui/layout.h>

using namespace rad;
using namespace ui;

struct ProgressSlidersPage : public VerticalBox {
	material::Slider slider;
	material::ProgressBar progress_bar;
	material::CircularProgress progress_circle;
	material::CheckBox indeterminate_check;

	ProgressSlidersPage() {
		horizontal_alignment = LayoutAlignment::center;
		vertical_alignment = LayoutAlignment::center;

		progress_bar.indeterminate = indeterminate_check.selected;
		progress_circle.indeterminate = indeterminate_check.selected;

		progress_bar.value = slider.value;
		progress_circle.value = slider.value;

		indeterminate_check.text() = "indeterminate";

		add_child(slider);
		add_child(progress_bar);
		add_child(progress_circle);
		//add_child(std::make_unique<material::Slider>())->orientation = Orientation::vertical;
		add_child(indeterminate_check);
	}
};

std::unique_ptr<Item> demo::make_progress_and_sliders() {
	return std::make_unique<ProgressSlidersPage>();
}
