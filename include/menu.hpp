#ifndef MENU_HPP
#define MENU_HPP

#include "recipe_list.hpp"
#include "storage.hpp"

class Menu {
private:
    RecipeList& recipeList;

    void clearScreen() const;
    void pause() const;

    //Acciones
    void showOptions() const;
    void actionListAll() const;
    void actionListByCategory() const;
    void actionAddRecipe();
    void actionDeleteRecipe();
    void actionClearAll();
    void actionSortByName();
    void actionSortByTime();
    void actionAddIngredient();
    void actionRemoveIngredient();
    void actionUpdateIngredientQty();
    void actionUpdateProcedure();
    void actionSave();
    void actionLoad();

    //Auxiliares
    Category askCategory() const;

public:
    explicit Menu(RecipeList& list) : recipeList(list) {}
    void start();
};

#endif // MENU_HPP
