#ifndef RECIPE_LIST_HPP
#define RECIPE_LIST_HPP

#include <string>
#include <iostream>
#include "recipe.hpp"
#include "constants.hpp"
#include "doubly_linked_list.hpp"

//Lista de recetas
class RecipeList : public DoublyLinkedList<Recipe> {
public:
    bool add(const Recipe& recipe) {
        pushBack(recipe);
        return true;
    }

    bool removeByName(const std::string& name) {
        return removeFirstIf(
            [&name](const Recipe& r) {
                return r.getName() == name;
            }
        );
    }

void clearAll() {
        // Vacía la lista usando el clear de la lista doblemente ligada
        DoublyLinkedList<Recipe>::clear();
    }

    void clear() {
        // Por si en otros lados llaman clear() directamente
        DoublyLinkedList<Recipe>::clear();
    }

    int findByName(const std::string& name) const {
        return findIndexIf(
            [&name](const Recipe& r) {
                return r.getName() == name;
            }
        );
    }

    int findFirstByCategory(Category cat) const {
        return findIndexIf(
            [cat](const Recipe& r) {
                return r.getCategory() == cat;
            }
        );
    }

    void listAll() const {
        if (size() == 0) {
            std::cout << "No hay recetas registradas.\n";
            return;
        }
        for (std::size_t i = 0; i < size(); ++i) {
            at(i).printDetails();
        }
    }

    void listByCategory(Category cat) const {
        bool found = false;
        for (std::size_t i = 0; i < size(); ++i) {
            if (at(i).getCategory() == cat) {
                at(i).printDetails();
                found = true;
            }
        }
        if (!found) std::cout << "No hay recetas en esa categoría.\n";
    }

    void sortByNameRecursive() {
        auto comp = [](const Recipe& a, const Recipe& b) {
            return a.getName() < b.getName();
        };
        sortRecursive(comp);
    }

    void sortByPrepTimeRecursive() {
        auto comp = [](const Recipe& a, const Recipe& b) {
            return a.getPrepTimeMinutes() < b.getPrepTimeMinutes();
        };
        sortRecursive(comp);
    }

};
#endif // RECIPE_LIST_HPP
