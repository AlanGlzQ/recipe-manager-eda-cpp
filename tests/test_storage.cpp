#include <gtest/gtest.h>
#include "storage.hpp"
#include "recipe_list.hpp"
#include "recipe.hpp"
#include "ingredient.hpp"
#include "category.hpp"
#include <cstdio>
#include <string>
#include <fstream>

// Archivo temporal usado en todos los tests
static const std::string TEST_FILE = "test_storage_tmp.txt";

// Limpia el archivo temporal antes y después de cada test
class StorageTest : public ::testing::Test {
protected:
    void SetUp()    override { std::remove(TEST_FILE.c_str()); }
    void TearDown() override { std::remove(TEST_FILE.c_str()); }
};

// Helper: receta completa con ingredientes
static Recipe makeFullRecipe(const std::string& name) {
    Recipe r(name, "Autor Test", Category::Lunch, 30, "Procedimiento de prueba");
    r.addIngredientOrdered(Ingredient("sal",      1.0, "cdita"));
    r.addIngredientOrdered(Ingredient("pimienta", 0.5, "cdita"));
    return r;
}

// ─── saveToFile ───────────────────────────────────────────────────────────────

TEST_F(StorageTest, SaveRetornaTrueEnArchivoValido) {
    RecipeList list;
    list.add(makeFullRecipe("Tacos"));
    EXPECT_TRUE(Storage::saveToFile(list, TEST_FILE));
}

TEST_F(StorageTest, SaveCreaElArchivo) {
    RecipeList list;
    list.add(makeFullRecipe("Sopa"));
    Storage::saveToFile(list, TEST_FILE);
    std::ifstream f(TEST_FILE);
    EXPECT_TRUE(f.good());
}

TEST_F(StorageTest, SaveListaVaciaRetornaTrue) {
    RecipeList list;
    EXPECT_TRUE(Storage::saveToFile(list, TEST_FILE));
}

TEST_F(StorageTest, SaveRetornaFalseEnRutaInvalida) {
    RecipeList list;
    list.add(makeFullRecipe("Tacos"));
    // Ruta que no puede existir
    bool result = Storage::saveToFile(list, "/ruta/que/no/existe/archivo.txt");
    EXPECT_FALSE(result);
}

// ─── loadFromFile ─────────────────────────────────────────────────────────────

TEST_F(StorageTest, LoadRetornaFalseArchivoInexistente) {
    RecipeList list;
    EXPECT_FALSE(Storage::loadFromFile(list, "no_existe.txt"));
}

TEST_F(StorageTest, LoadEnListaVaciaRetornaTrue) {
    RecipeList list;
    Storage::saveToFile(list, TEST_FILE);
    RecipeList loaded;
    EXPECT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));
    EXPECT_EQ(loaded.size(), 0u);
}

// ─── save + load (round-trip) ─────────────────────────────────────────────────

TEST_F(StorageTest, RoundTripUnaRecetaSinIngredientes) {
    RecipeList original;
    original.add(Recipe("Ensalada", "Chef Ana", Category::Lunch, 10, "Mezclar todo"));

    Storage::saveToFile(original, TEST_FILE);

    RecipeList loaded;
    ASSERT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));

    ASSERT_EQ(loaded.size(), 1u);
    EXPECT_EQ(loaded.at(0).getName(),           "Ensalada");
    EXPECT_EQ(loaded.at(0).getAuthorName(),     "Chef Ana");
    EXPECT_EQ(loaded.at(0).getCategory(),       Category::Lunch);
    EXPECT_EQ(loaded.at(0).getPrepTimeMinutes(), 10);
    EXPECT_EQ(loaded.at(0).getProcedure(),      "Mezclar todo");
    EXPECT_EQ(loaded.at(0).getIngredientCount(), 0);
}

TEST_F(StorageTest, RoundTripUnaRecetaConIngredientes) {
    RecipeList original;
    original.add(makeFullRecipe("Pozole"));

    Storage::saveToFile(original, TEST_FILE);

    RecipeList loaded;
    ASSERT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));

    ASSERT_EQ(loaded.size(), 1u);
    EXPECT_EQ(loaded.at(0).getName(), "Pozole");
    // Ingredientes se insertan ordenados: pimienta < sal
    ASSERT_EQ(loaded.at(0).getIngredientCount(), 2);
    EXPECT_EQ(loaded.at(0).getIngredientAt(0).getName(), "pimienta");
    EXPECT_EQ(loaded.at(0).getIngredientAt(1).getName(), "sal");
    EXPECT_DOUBLE_EQ(loaded.at(0).getIngredientAt(1).getQuantity(), 1.0);
    EXPECT_EQ(loaded.at(0).getIngredientAt(1).getUnit(), "cdita");
}

TEST_F(StorageTest, RoundTripVariasRecetas) {
    RecipeList original;
    original.add(makeFullRecipe("Tacos"));
    original.add(makeFullRecipe("Birria"));
    original.add(makeFullRecipe("Enchiladas"));

    Storage::saveToFile(original, TEST_FILE);

    RecipeList loaded;
    ASSERT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));

    ASSERT_EQ(loaded.size(), 3u);
    EXPECT_EQ(loaded.at(0).getName(), "Tacos");
    EXPECT_EQ(loaded.at(1).getName(), "Birria");
    EXPECT_EQ(loaded.at(2).getName(), "Enchiladas");
}

TEST_F(StorageTest, RoundTripPreservaCategorias) {
    RecipeList original;
    original.add(Recipe("Hotcakes", "A", Category::Breakfast, 15, "..."));
    original.add(Recipe("Tacos",    "B", Category::Lunch,     20, "..."));
    original.add(Recipe("Cena",     "C", Category::Dinner,    25, "..."));

    Storage::saveToFile(original, TEST_FILE);

    RecipeList loaded;
    ASSERT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));

    EXPECT_EQ(loaded.at(0).getCategory(), Category::Breakfast);
    EXPECT_EQ(loaded.at(1).getCategory(), Category::Lunch);
    EXPECT_EQ(loaded.at(2).getCategory(), Category::Dinner);
}

TEST_F(StorageTest, LoadLimpiaListaExistente) {
    // La lista ya tiene datos antes del load
    RecipeList list;
    list.add(makeFullRecipe("Datos viejos"));

    RecipeList toSave;
    toSave.add(makeFullRecipe("Datos nuevos"));
    Storage::saveToFile(toSave, TEST_FILE);

    Storage::loadFromFile(list, TEST_FILE);

    ASSERT_EQ(list.size(), 1u);
    EXPECT_EQ(list.at(0).getName(), "Datos nuevos");
}

TEST_F(StorageTest, RoundTripPreservaTiempoDePreparacion) {
    RecipeList original;
    original.add(Recipe("Mole", "Chef", Category::Dinner, 120, "Proceso largo"));

    Storage::saveToFile(original, TEST_FILE);

    RecipeList loaded;
    ASSERT_TRUE(Storage::loadFromFile(loaded, TEST_FILE));
    EXPECT_EQ(loaded.at(0).getPrepTimeMinutes(), 120);
}
