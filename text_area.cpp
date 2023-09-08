#include "example.h"
#include <rad/ui/layout.h>
#include <rad/ui/material3/textfield.h>
#include <rad/ui/material3/checkbox.h>

using namespace rad;
using namespace ui;

struct TextAreaPage : public VerticalBox {
	material::TextField text_field;
	material::CheckBox outlined_check;

	TextAreaPage() {
		text_field.label_text() = "Text Area";
		text_field.placeholder_text() = "Type something here !";
		text_field.supporting_text() = "This is a multiline text area";
		text_field.max_chars_count() = 5000;
		text_field.single_line() = false;
		text_field.type = if_else(outlined_check.selected, material::TextFieldType::outlined, material::TextFieldType::filled);
		
		outlined_check.text() = "outlined";

		add_child(text_field);
		add_child(outlined_check);
	}
};

std::unique_ptr<Item> demo::make_text_area_page() {
	return std::make_unique<TextAreaPage>();
}