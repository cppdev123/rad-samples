#include "example.h"
#include <rad/ui/material3/button.h>
#include <rad/ui/material3/textfield.h>
#include <rad/ui/material3/select_box.h>
#include <rad/ui/material3/checkbox.h>
#include <rad/ui/layout.h>

using namespace rad::ui;

enum class FormError : uint64_t {
	none,
	required_first_name = 1 << 0,
	required_last_name = 1 << 1,
	required_email = 1 << 2,
	invalid_email = 1 << 3,
	required_password = 1 << 4,
	weak_password = 1 << 5,
	required_job = 1 << 6,
};

OVERLOAD_ENUM_OPERATORS(FormError);

struct AccountForm : public VerticalBox {
	material::Button submit_button;
	material::TextField first_name_field;
	material::TextField last_name_field;
	material::TextField email_field;
	material::TextField password_field;
	material::SelectBox job_select;
	VecModel<material::ComboBoxMenuItemModelData> job_items;
	material::CheckBox outlined_field_check;
	Property<FormError> form_error;

	static void set_text_outline(material::TextField& text_field, const Property<bool>& checked) {
		text_field.type = if_else(checked, material::TextFieldType::outlined, material::TextFieldType::filled);
	}

	AccountForm() {
		horizontal_alignment = LayoutAlignment::center;
		//vertical_alignment = LayoutAlignment::center;
		spacing = 30;
		padding.set(20);

		Pixmap person_filled{ "person_FILL0_wght400_GRAD0_opsz48.png" };
		Pixmap key_filled{ "key_FILL0_wght400_GRAD0_opsz24.png" };
		Pixmap mail_filled{ "mail_FILL0_wght400_GRAD0_opsz24.png" };
		Pixmap job_filled{ "work_FILL0_wght400_GRAD0_opsz24.png" };
		Pixmap drop_down_filled{ "arrow_drop_down_FILL0_wght400_GRAD0_opsz48.png" };

		first_name_field.label_text() = "First name";
		first_name_field.placeholder_text() = "Type your first name!";
		first_name_field.max_chars_count() = 30;
		first_name_field.has_error = form_error & FormError::required_first_name;
		first_name_field.supporting_text() = if_else(first_name_field.has_error, "required field"_ss, ""_ss);
		first_name_field.leading_icon() = person_filled;

		last_name_field.label_text() = "Last name";
		last_name_field.max_chars_count() = 30;
		last_name_field.has_error = form_error & FormError::required_last_name;
		last_name_field.supporting_text() = if_else(last_name_field.has_error, "required field"_ss, ""_ss);
		last_name_field.leading_icon() = person_filled;

		email_field.label_text() = "Email";
		email_field.supporting_text() = "enter a valid email address";
		email_field.has_error = (form_error & FormError::required_email) || (form_error & FormError::invalid_email);
		email_field.supporting_text() = if_else(email_field.has_error, 
			if_else(form_error & FormError::required_email, "required field"_ss, "invalid email address"_ss), ""_ss);
		email_field.leading_icon() = mail_filled;

		password_field.label_text() = "Password";
		password_field.has_error = (form_error & FormError::required_password) || (form_error & FormError::weak_password);
		password_field.supporting_text() = if_else(password_field.has_error,
			if_else(form_error & FormError::required_password, "required field"_ss, "weak password"_ss), ""_ss);
		password_field.leading_icon() = key_filled;

		job_select.label_text() = "Job";
		job_select.text_field().has_error = form_error & FormError::required_job;
		job_select.text_field().supporting_text() = if_else(job_select.text_field().has_error,
			"required field"_ss , ""_ss);
		job_select.text_field().leading_icon() = job_filled;
		job_select.text_field().trailing_icon() = drop_down_filled;

		job_select.set_model(job_items);
		job_items.push_item(material::ComboBoxMenuItemModelData{ .text = "c++ programmer" });
		job_items.push_item(material::ComboBoxMenuItemModelData{ .text = "rust programmer" });
		job_items.push_item(material::ComboBoxMenuItemModelData{ .placeholder = "none" });

		submit_button.text() = "Submit";
		submit_button.clicked = [this](ClickEvent) {
			validate_form();
		};

		outlined_field_check.text() = "outlined";
		

		set_text_outline(first_name_field, outlined_field_check.selected);
		set_text_outline(last_name_field, outlined_field_check.selected);
		set_text_outline(email_field, outlined_field_check.selected);
		set_text_outline(password_field, outlined_field_check.selected);
		set_text_outline(job_select.text_field(), outlined_field_check.selected);

		add_child(first_name_field);
		auto hlay = add_child(std::make_unique<HorizontalLayout>());
		hlay->spacing = 20;
		hlay->add_child(last_name_field);
		//hlay->add_child(std::make_unique<HorizontalSpacer>());
		hlay->add_child(job_select);
		hlay->vertical_stretch_factor = -1;

		add_child(email_field);
		add_child(password_field);

		add_child(submit_button);

		hlay = add_child(std::make_unique<HorizontalLayout>());
		hlay->add_child(outlined_field_check);
		hlay->vertical_stretch_factor = -1;
	}

	void validate_form() {
		FormError result = FormError::none;
		if (first_name_field.text()().empty())
			result |= FormError::required_first_name;
		if (last_name_field.text()().empty())
			result |= FormError::required_last_name;
		if (email_field.text()().empty())
			result |= FormError::required_email;
		if (password_field.text()().empty())
			result |= FormError::required_password;
		if (job_select.text_field().text()().empty())
			result |= FormError::required_job;
		if (is_weak_password(password_field.text()().view()))
			result |= FormError::weak_password;
		
		form_error = result;
	}

	bool is_weak_password(std::u16string_view text) {
		if (text.empty())
			return false;
		if (text.size() < 8)
			return true;
		bool has_capitals = false, has_smalls = false, has_digits = false;
		for (auto ch : text) {
			if (ch >= u'A' && ch <= 'Z')
				has_capitals = true;
			else if (ch >= u'a' && ch <= u'z')
				has_smalls = true;
			else if (ch >= u'0' && ch <= u'9')
				has_digits = true;
		}
		return !has_smalls || !has_capitals || !has_digits;
	}
};

std::unique_ptr<Item> demo::make_form_page() {
	return std::make_unique<AccountForm>();
}