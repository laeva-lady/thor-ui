// main.cpp
#include <gtkmm.h>

#include "manager.hpp"

class CategoryModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
  CategoryModelColumns() { add(col_name); }
  Gtk::TreeModelColumn<Glib::ustring> col_name;
};
void populate_tree_store(const std::vector<Category> &categories,
                         const Glib::RefPtr<Gtk::TreeStore> &tree_store,
                         CategoryModelColumns &columns) {
  for (const auto &category : categories) {
    Gtk::TreeModel::Row cat_row = *(tree_store->append());
    cat_row[columns.col_name] = category.name;

    for (const auto &pkg : category.pkg_names) {
      Gtk::TreeModel::Row pkg_row = *(tree_store->append(cat_row.children()));
      pkg_row[columns.col_name] = pkg;
    }
  }
}

class MyWindow : public Gtk::Window {
public:
  MyWindow(const std::vector<Category> &categories) {
    set_title("Package Categories");
    set_default_size(400, 300);

    tree_view.set_model(tree_store);
    tree_view.append_column("Name", columns.col_name);

    populate_tree_store(categories, tree_store, columns);

    tree_view.expand_all();

    scrolled_window.add(tree_view);
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    add(scrolled_window);
    show_all_children();
  }

private:
  Gtk::TreeView tree_view;
  Gtk::ScrolledWindow scrolled_window;

  CategoryModelColumns columns;
  Glib::RefPtr<Gtk::TreeStore> tree_store = Gtk::TreeStore::create(columns);
};

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.example.gtkmm");

  MyWindow window(manager::getCategories());
  return app->run(window);
}
