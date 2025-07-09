#include "app.hpp"

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
