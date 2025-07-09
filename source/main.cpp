// main.cpp
//
#include "app.hpp"

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create(argc, argv, "org.example.gtkmm");

  MyWindow window(manager::getCategories());
  return app->run(window);
}
