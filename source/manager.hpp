#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

struct Category {
  std::string name;
  std::vector<std::string> pkg_names;
};

namespace manager {
std::string getPkgPath();
std::vector<Category> getCategories();


void print(std::vector<Category> cats);

} // namespace manager
