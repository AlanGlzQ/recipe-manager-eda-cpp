#include "../include/recipe.hpp"

#include <functional>
#include <iostream>

bool Recipe::addIngredientOrdered(const Ingredient& ing) {
    // Comparador explícito: ordena por nombre de ingrediente
    auto comp = [](const Ingredient& a, const Ingredient& b) {
        return a.getName() < b.getName();
    };
    return ingredients.insertOrdered(ing, comp);
}

bool Recipe::removeIngredientByName(const std::string& ingName) {
    return ingredients.removeFirstIf(
        [&ingName](const Ingredient& ing) {
            return ing.getName() == ingName;
        }
    );
}

void Recipe::clearIngredients() {
    ingredients.clear();
}

bool Recipe::updateIngredientQuantity(const std::string& ingName, double newQty) {
    bool updated = false;
    ingredients.forEach(
        [&updated, &ingName, newQty](Ingredient& ing) {
            if (ing.getName() == ingName) {
                ing.setQuantity(newQty);
                updated = true;
            }
        }
    );
    return updated;
}

void Recipe::printDetails() const {
    std::cout << "\n=============================\n";
    std::cout << "Nombre: " << name << "\n";
    std::cout << "Autor: " << authorName << "\n";
    std::cout << "Categoría: " << categoryToString(category) << "\n";
    std::cout << "Tiempo de preparación: " << prepTimeMinutes << " min\n";
    std::cout << "Procedimiento: " << procedure << "\n";

    if (ingredients.size() == 0) {
        std::cout << "Ingredientes: ninguno.\n";
    } else {
        std::cout << "Ingredientes (" << ingredients.size() << "):\n";
        ingredients.forEach(
            [](const Ingredient& ing) {
                std::cout << "  - " << ing.toString() << "\n";
            }
        );
    }
}