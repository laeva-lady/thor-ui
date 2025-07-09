#include "manager.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

#define DEBUG true

std::string manager::getPkgPath() {
  auto path = getenv("PKG_PATH_JSON");
  if (DEBUG) {
    std::cout << path << std::endl;
  }
  return path;
}

std::vector<Category> manager::getCategories() {
  auto path = getPkgPath();
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open JSON file\n";
  }

  json j;
  try {
    file >> j;
  } catch (json::parse_error &e) {
    std::cerr << "Parse error: " << e.what() << "\n";
  }

  std::vector<std::string> keyList;
  for (auto it = j.begin(); it != j.end(); ++it) {
    keyList.push_back(it.key());
  }

  std::vector<Category> cats;
  for (const auto key : keyList) {

    Category cat;
    if (j.contains(key)) {
      cat.name = key;
      for (const auto &pkg : j[key]) {
        cat.pkg_names.push_back(pkg);
      }
    }
    cats.push_back(cat);
  }

  return cats;
}

void manager::print(std::vector<Category> cats) {

  for (const auto cat : cats) {
    std::cout << "🗂️" << cat.name << std::endl;
    for (const auto pkg : cat.pkg_names)
      std::cout << "\t" <<pkg << std::endl;
  }
}
