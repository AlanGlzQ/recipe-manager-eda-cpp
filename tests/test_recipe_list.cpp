#include <gtest/gtest.h>
#include "recipe_list.hpp"
#include "recipe.hpp"
#include "ingredient.hpp"
#include "category.hpp"

// Helper para crear recetas rápido
static Recipe makeRecipe(const std::string& name,
                          Category cat = Category::Breakfast,
                          int prepTime = 10) {
    return Recipe(name, "Autor Test", cat, prepTime, "Procedimiento de prueba");
}

// ─── Recipe: construcción y getters ──────────────────────────────────────────

TEST(Recipe, GettersRetornanValoresCorrectos) {
    Recipe r("Tacos", "Chef Luis", Category::Lunch, 20, "Calentar tortillas");
    EXPECT_EQ(r.getName(), "Tacos");
    EXPECT_EQ(r.getAuthorName(), "Chef Luis");
    EXPECT_EQ(r.getCategory(), Category::Lunch);
    EXPECT_EQ(r.getPrepTimeMinutes(), 20);
    EXPECT_EQ(r.getProcedure(), "Calentar tortillas");
}

TEST(Recipe, RecetaVaciaTieneZeroIngredientes) {
    Recipe r = makeRecipe("Ensalada");
    EXPECT_EQ(r.getIngredientCount(), 0);
}

// ─── Recipe: ingredientes ─────────────────────────────────────────────────────

TEST(Recipe, AddIngredientOrdenedInsertaOrdenado) {
    Recipe r = makeRecipe("Sopa");
    r.addIngredientOrdered(Ingredient("zanahoria", 2.0, "pzas"));
    r.addIngredientOrdered(Ingredient("ajo",       3.0, "dientes"));
    r.addIngredientOrdered(Ingredient("cebolla",   1.0, "pza"));

    ASSERT_EQ(r.getIngredientCount(), 3);
    EXPECT_EQ(r.getIngredientAt(0).getName(), "ajo");
    EXPECT_EQ(r.getIngredientAt(1).getName(), "cebolla");
    EXPECT_EQ(r.getIngredientAt(2).getName(), "zanahoria");
}

TEST(Recipe, RemoveIngredientByNameElimina) {
    Recipe r = makeRecipe("Guiso");
    r.addIngredientOrdered(Ingredient("sal",   1.0, "cdita"));
    r.addIngredientOrdered(Ingredient("pimienta", 0.5, "cdita"));
    bool removed = r.removeIngredientByName("sal");
    EXPECT_TRUE(removed);
    EXPECT_EQ(r.getIngredientCount(), 1);
    EXPECT_EQ(r.getIngredientAt(0).getName(), "pimienta");
}

TEST(Recipe, RemoveIngredientByNameRetornaFalseSiNoExiste) {
    Recipe r = makeRecipe("Arroz");
    r.addIngredientOrdered(Ingredient("agua", 2.0, "tazas"));
    bool removed = r.removeIngredientByName("cilantro");
    EXPECT_FALSE(removed);
    EXPECT_EQ(r.getIngredientCount(), 1);
}

TEST(Recipe, ClearIngredientsDejaCero) {
    Recipe r = makeRecipe("Pozole");
    r.addIngredientOrdered(Ingredient("maiz", 500.0, "g"));
    r.addIngredientOrdered(Ingredient("chile", 3.0, "pzas"));
    r.clearIngredients();
    EXPECT_EQ(r.getIngredientCount(), 0);
}

TEST(Recipe, UpdateIngredientQuantityModificaCantidad) {
    Recipe r = makeRecipe("Pasta");
    r.addIngredientOrdered(Ingredient("sal", 1.0, "cdita"));
    bool updated = r.updateIngredientQuantity("sal", 2.5);
    EXPECT_TRUE(updated);
    EXPECT_DOUBLE_EQ(r.getIngredientAt(0).getQuantity(), 2.5);
}

TEST(Recipe, UpdateIngredientQuantityRetornaFalseSiNoExiste) {
    Recipe r = makeRecipe("Pasta");
    r.addIngredientOrdered(Ingredient("sal", 1.0, "cdita"));
    bool updated = r.updateIngredientQuantity("oregano", 1.0);
    EXPECT_FALSE(updated);
}

// ─── RecipeList: add y size ───────────────────────────────────────────────────

TEST(RecipeList, ListaNuevaEstaVacia) {
    RecipeList list;
    EXPECT_EQ(list.size(), 0u);
    EXPECT_TRUE(list.empty());
}

TEST(RecipeList, AddAumentaSize) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.add(makeRecipe("Enchiladas"));
    EXPECT_EQ(list.size(), 2u);
}

// ─── RecipeList: findByName ───────────────────────────────────────────────────

TEST(RecipeList, FindByNameRetornaIndiceCorecto) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.add(makeRecipe("Pozole"));
    int idx = list.findByName("Pozole");
    EXPECT_EQ(idx, 1);
}

TEST(RecipeList, FindByNameRetornaMenosUnoSiNoExiste) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    int idx = list.findByName("Pizza");
    EXPECT_EQ(idx, -1);
}

TEST(RecipeList, FindByNameEnListaVaciaRetornaMenosUno) {
    RecipeList list;
    EXPECT_EQ(list.findByName("Tacos"), -1);
}

// ─── RecipeList: removeByName ─────────────────────────────────────────────────

TEST(RecipeList, RemoveByNameEliminaCorrectamente) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.add(makeRecipe("Sopa"));
    bool removed = list.removeByName("Tacos");
    EXPECT_TRUE(removed);
    EXPECT_EQ(list.size(), 1u);
    EXPECT_EQ(list.findByName("Tacos"), -1);
    EXPECT_EQ(list.findByName("Sopa"), 0);
}

TEST(RecipeList, RemoveByNameRetornaFalseSiNoExiste) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    bool removed = list.removeByName("Birria");
    EXPECT_FALSE(removed);
    EXPECT_EQ(list.size(), 1u);
}

TEST(RecipeList, RemoveUnicaRecetaDejaListaVacia) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.removeByName("Tacos");
    EXPECT_TRUE(list.empty());
}

// ─── RecipeList: findFirstByCategory ─────────────────────────────────────────

TEST(RecipeList, FindFirstByCategoryEncuentraCategoria) {
    RecipeList list;
    list.add(makeRecipe("Hotcakes",  Category::Breakfast));
    list.add(makeRecipe("Enchiladas", Category::Lunch));
    int idx = list.findFirstByCategory(Category::Lunch);
    EXPECT_EQ(idx, 1);
}

TEST(RecipeList, FindFirstByCategoryRetornaMenosUnoSiNoHay) {
    RecipeList list;
    list.add(makeRecipe("Hotcakes", Category::Breakfast));
    int idx = list.findFirstByCategory(Category::Dinner);
    EXPECT_EQ(idx, -1);
}

// ─── RecipeList: sortByNameRecursive ─────────────────────────────────────────

TEST(RecipeList, SortByNameOrdenaNombres) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.add(makeRecipe("Enchiladas"));
    list.add(makeRecipe("Birria"));
    list.sortByNameRecursive();
    EXPECT_EQ(list.at(0).getName(), "Birria");
    EXPECT_EQ(list.at(1).getName(), "Enchiladas");
    EXPECT_EQ(list.at(2).getName(), "Tacos");
}

TEST(RecipeList, SortByNameMantieneSizeCorrect) {
    RecipeList list;
    list.add(makeRecipe("Z"));
    list.add(makeRecipe("A"));
    list.add(makeRecipe("M"));
    list.sortByNameRecursive();
    EXPECT_EQ(list.size(), 3u);
}

// ─── RecipeList: sortByPrepTimeRecursive ──────────────────────────────────────

TEST(RecipeList, SortByPrepTimeOrdenaPorTiempo) {
    RecipeList list;
    list.add(makeRecipe("Lenta",   Category::Dinner,    60));
    list.add(makeRecipe("Rapida",  Category::Breakfast, 5));
    list.add(makeRecipe("Media",   Category::Lunch,     30));
    list.sortByPrepTimeRecursive();
    EXPECT_EQ(list.at(0).getPrepTimeMinutes(), 5);
    EXPECT_EQ(list.at(1).getPrepTimeMinutes(), 30);
    EXPECT_EQ(list.at(2).getPrepTimeMinutes(), 60);
}

// ─── RecipeList: clearAll ─────────────────────────────────────────────────────

TEST(RecipeList, ClearAllVaciaLaLista) {
    RecipeList list;
    list.add(makeRecipe("Tacos"));
    list.add(makeRecipe("Sopa"));
    list.clearAll();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.size(), 0u);
}
