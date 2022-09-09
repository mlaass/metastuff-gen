#pragma once

#include <map>
#include <string>
#include <vector>

struct Definition {
  std::string type;
  std::map<std::string, std::string> attributes;
  std::string code;

  void print() {
    std::cout << "def " << type <<" ";
    for (auto const &a : attributes) {
      std::cout << a.first << " = '" << a.second << "', ";
    }
    std::cout << " :\n" << code << "\nend" << std::endl;

  }
};

struct Templ8 {
  std::string name;
  std::vector<Definition> definitions;

  void print() {
    std::cout << name << ":" << std::endl;
    for (auto d : definitions)
      d.print();
  }
};