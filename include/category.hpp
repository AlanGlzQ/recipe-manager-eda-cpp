#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include <string>

//Enum para categorias
enum class Category {
    Breakfast = 0,
    Lunch     = 1,
    Dinner    = 2,
    Christmas = 3
};

//Convierte Category a string
inline std::string categoryToString(Category c) {
    switch (c) {
        case Category::Breakfast: return "Desayuno";
        case Category::Lunch:     return "Almuerzo";
        case Category::Dinner:    return "Cena";
        case Category::Christmas: return "Navideño";
    }
    return "Desayuno";
}

inline Category categoryFromString(const std::string& s) {
    if (s == "Desayuno") return Category::Breakfast;
    if (s == "Almuerzo")     return Category::Lunch;
    if (s == "Cena")    return Category::Dinner;
    if (s == "Navideño") return Category::Christmas;
    return Category::Breakfast;
}

#endif // CATEGORY_HPP
