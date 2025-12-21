#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>
#include "recipe_list.hpp"

class Storage {
public:
    static bool saveToFile(const RecipeList& list, const std::string& path);
    static bool loadFromFile(RecipeList& list, const std::string& path);
};

#endif // STORAGE_HPP
