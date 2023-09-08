#include "example.h"
#include <rad/ui/text.h>
#include <rad/ui/material3/checkbox.h>
#include <rad/ui/material3/textfield.h>
#include <rad/ui/material3/button.h>
#include <rad/ui/model_view/listview.h>
#include <rad/ui/layout.h>

using namespace rad::ui;

struct TodoModelItem {
	SharedString text;
	bool checked = false;
};

struct TodoModel : public VecModel<TodoModelItem> {
	void commit_checked_at(size_t i, bool checked) {
		if (i >= inner_vec().size())
			return;
		inner_vec()[i].checked = checked;
	}

	void remove_checked_items() {
		auto& vec = inner_vec();
		while (1) {
			auto it = std::find_if(vec.begin(), vec.end(), [](const TodoModelItem& item) { return item.checked; });
			if (it == vec.end())
				break;
			size_t row = static_cast<size_t>(std::distance(vec.begin(), it));
			remove_rows(row, row, [&] { vec.erase(it); });
		}
	}
};

struct TodoItem : public HorizontalLayout {
	material::CheckBox checked;

	TodoItem(bool is_checked, const SharedString& todo_text, ModelBase& model, ViewProperties& view) {
		checked.selected = is_checked;
		checked.text() = rad::ui::format("({}) " + rad::to_string(todo_text.view()), delegate().index_row() + 1);

		vertical_alignment = LayoutAlignment::center;
		vertical_stretch_factor = -1;
		padding.left = 20;
		//padding.top = if_else(delegate().index_row() == 0, 20, 0);
		//padding.bottom = if_else(delegate().index_row() == (view.count - 1), 20, 0);

		checked.selection_changed = [&model, this](bool checked) {
			static_cast<TodoModel&>(model).commit_checked_at(delegate().index().row(), checked);
		};

		add_child(checked);
	}
};

struct TodoItemDelegate : public DelegateBase {
	DelegateItemPtr make_item(const std::any& data, const ModelIndex& index,
		ModelBase& model, Item& parent, ViewProperties& view) override {
		if (const auto* p = std::any_cast<TodoModelItem>(&data)) {
			auto item = std::make_unique<material::CheckBox>(p->checked);
			item->visible = false; // bypass check animations
			//item->selected = p->checked;
			item->text() = rad::ui::format("({}) " + rad::to_string(p->text.view()), item->delegate().index_row() + 1);
			//item->x = 20;
			item->visible = true;
			item->selection_changed = [&model, i = item.get()](bool checked) {
				static_cast<TodoModel&>(model).commit_checked_at(i->delegate().index().row(), checked);
			};
			return item;
		}
		return nullptr;
	}
};

struct TodoPage : public VerticalBox {
	material::TextField text_input;
	material::Button add_btn;
	material::Button remove_btn;
	Text todos_count;
	TodoModel todo_model;
	TodoItemDelegate todo_delegate;
	ListView todo_list;

	TodoPage() {
		text_input.label_text() = "Todo Entry";
		add_btn.text() = "Add New Entry";
		remove_btn.text() = "Remove Done Items";
		remove_btn.type = material::ButtonType::filled;
		todos_count.font_size = 16;
		todos_count.text = format("items: {}, delegates: {}", todo_list.count(), todo_list.delegates_count());
		todos_count.vertical_alignment = TextVerticalAlignment::center;
		remove_btn.horizontal_stretch_factor = -1;

		todo_model.emplace_item(TodoModelItem{ .text = "Implement the .slint file" });
		todo_model.emplace_item(TodoModelItem{ .text = "Do the Rust part" });
		todo_model.emplace_item(TodoModelItem{ .text = "Make the C++ code" });
		todo_model.emplace_item(TodoModelItem{ .text = "Write some JavaScript code" });
		todo_model.emplace_item(TodoModelItem{ .text = "Test the application" });
		todo_model.emplace_item(TodoModelItem{ .text = "Ship to customer" });
		todo_model.emplace_item(TodoModelItem{ .text = "???" });
		todo_model.emplace_item(TodoModelItem{ .text = "Profit" });

		for (auto i : rad::range(50)) {
			todo_model.emplace_item(TodoModelItem{ .text = SharedString{ "item " + std::to_string(i + 1) } });
		}

		todo_list.padding.top = 20;
		todo_list.padding.bottom = 20;
		todo_list.padding.left = 20;
		todo_list.spacing = 20;
		todo_list.min_height = 100;
		todo_list.set_model(todo_model);
		todo_list.set_delegate(todo_delegate);

		add_btn.clicked = [this](ClickEvent) {
			const auto& todo_text = text_input.text()();
			if (todo_text.empty())
				return;
			todo_model.emplace_item(TodoModelItem{ .text = todo_text });
		};

		remove_btn.clicked = [this](ClickEvent) {
			todo_model.remove_checked_items();
		};

		auto hlay = add_child(std::make_unique<HorizontalBox>());
		hlay->vertical_stretch_factor = -1;
		hlay->vertical_alignment = LayoutAlignment::center;
		hlay->add_child(text_input);
		hlay->add_child(add_btn);

		add_child(todo_list);

		hlay = add_child(std::make_unique<HorizontalBox>());
		hlay->vertical_stretch_factor = -1;
		hlay->vertical_alignment = LayoutAlignment::center;
		hlay->horizontal_alignment = LayoutAlignment::end;
		hlay->add_child(todos_count);
		hlay->add_child(remove_btn);
	}
};

std::unique_ptr<Item> demo::make_todo_list() {
	return std::make_unique<TodoPage>();
}