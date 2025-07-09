#include "manager.hpp"
#include <gtkmm.h>

class CategoryModelColumns : public Gtk::TreeModel::ColumnRecord {
public:
  CategoryModelColumns() { add(col_name); }
  Gtk::TreeModelColumn<Glib::ustring> col_name;
};

void populate_tree_store(const std::vector<Category> &categories,
                         const Glib::RefPtr<Gtk::TreeStore> &tree_store,
                         CategoryModelColumns &columns);

class MyWindow : public Gtk::Window {
public:
  MyWindow(const std::vector<Category> &categories) {
    set_title("Package Categories");
    set_default_size(400, 300);

    tree_view.set_model(tree_store);
    tree_view.append_column("Name", columns.col_name);

    populate_tree_store(categories, tree_store, columns);

    tree_view.expand_all();

    // Enable button press events on the tree view
    tree_view.add_events(Gdk::BUTTON_PRESS_MASK);
    tree_view.signal_button_press_event().connect(
        sigc::mem_fun(*this, &MyWindow::on_tree_view_button_press));

    scrolled_window.add(tree_view);
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    add(scrolled_window);
    show_all_children();
  }

private:
  Gtk::TreeView tree_view;
  Gtk::ScrolledWindow scrolled_window;

  bool on_tree_view_button_press(GdkEventButton *event) {
    if (event->button == 1) { // right-click

      Gtk::TreeModel::Path path;
      if (tree_view.get_path_at_pos((int)event->x, (int)event->y, path)) {
        auto iter = tree_store->get_iter(path);
        if (!iter)
          return false;

        // Only show menu if it's a child (package), i.e., has a parent
        if (iter->parent()) {
          show_context_menu(iter, event->button, event->time);
          return true; // event handled
        }
      }
    }
    return false; // event not handled
  }

  void show_context_menu(const Gtk::TreeModel::iterator &iter, guint button,
                         guint32 activate_time) {
    Gtk::Menu *menu = Gtk::manage(new Gtk::Menu());

    Gtk::MenuItem *remove_item = Gtk::manage(new Gtk::MenuItem("Remove"));
    remove_item->signal_activate().connect(
        [this, iter]() { tree_store->erase(iter); });

    menu->append(*remove_item);
    menu->show_all();

    menu->popup_at_pointer(nullptr);
  }

  CategoryModelColumns columns;
  Glib::RefPtr<Gtk::TreeStore> tree_store = Gtk::TreeStore::create(columns);
};
