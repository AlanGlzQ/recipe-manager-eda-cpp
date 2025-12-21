#ifndef RECIPE_HPP
#define RECIPE_HPP

#include <string>
#include <sstream>
#include "ingredient.hpp"
#include "category.hpp"
#include "constants.hpp"
#include "singly_linked_list.hpp"

class Recipe {
private:
    std::string name;
    std::string authorName;
    Category category {Category::Breakfast};
    int prepTimeMinutes {0};
    std::string procedure;

    SinglyLinkedList<Ingredient> ingredients;

public:
    //Constructores
    Recipe() = default;
    Recipe(const std::string& name,
           const std::string& authorName,
           Category category,
           int prepTimeMinutes,
           const std::string& procedure)
        : name(name),
          authorName(authorName),
          category(category),
          prepTimeMinutes(prepTimeMinutes),
          procedure(procedure) {}

    //Getters
    const std::string& getName() const { return name; }
    const std::string& getAuthorName() const { return authorName; }
    Category getCategory() const { return category; }
    int getPrepTimeMinutes() const { return prepTimeMinutes; }
    const std::string& getProcedure() const { return procedure; }

    //Setters
    void setProcedure(const std::string& text) { procedure = text; }

    //Acceso de solo lectura a ingredientes
    int getIngredientCount() const { return static_cast<int>(ingredients.size()); }
    const Ingredient& getIngredientAt(int idx) const { return ingredients.at(idx); }

    //Inserción ORDENADA por nombre
    bool addIngredientOrdered(const Ingredient& ing);

    //Eliminar ingrediente por nombre
    bool removeIngredientByName(const std::string& ingName);

    //Eliminar todos los ingredientes
    void clearIngredients();

    //Modificar cantidad de ingrediente
    bool updateIngredientQuantity(const std::string& ingName, double newQty);

    //Imprimir receta completa con ingredientes
    void printDetails() const;

    //toString básico
    std::string toString() const {
        std::ostringstream oss;
        oss << "Receta{nombre='" << name
            << "', autor='" << authorName
            << "', categoria='" << categoryToString(category)
            << "', preparacion=" << prepTimeMinutes << " min}";
        return oss.str();
    }
};

#endif // RECIPE_HPP
