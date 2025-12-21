#include "../include/menu.hpp"
#include <iostream>
#include <limits>

//limpiar pantalla
void Menu::clearScreen() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

//pausar
void Menu::pause() const {
    std::cout << "Presiona ENTER para continuar...";
    std::cout.flush();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

//menu
void Menu::showOptions() const {
    std::cout << "======== RECETARIO ========\n"
              << " 1) Listar Recetas\n"
              << " 2) Listar por Categoría\n"
              << " 3) Buscar Receta (por nombre)\n"
              << " 4) Agregar Receta\n"
              << " 5) Eliminar Receta (por nombre)\n"
              << " 6) Eliminar TODAS las Recetas\n"
              << " 7) Ordenar por Nombre\n"
              << " 8) Ordenar por Tiempo de Preparación\n"
              << " 9) Agregar Ingrediente (ordenado)\n"
              << "10) Eliminar Ingrediente\n"
              << "11) Modificar Cantidad de Ingrediente\n"
              << "12) Modificar Procedimiento\n"
              << "13) Guardar a Disco\n"
              << "14) Leer de Disco\n"
              << " 0) Salir\n\n"
              << "Opción: ";
}

//Categoria
Category Menu::askCategory() const {
    std::cout << "Categoría [0=Desayuno, 1=Almuerzo, 2=Cena, 3=Navidad]: ";
    int c; std::cin >> c;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        c = 0;
    }
    if (c < 0 || c > 3) c = 0;
    return static_cast<Category>(c);
}

//Listar recetas
void Menu::actionListAll() const {
    clearScreen();
    if (recipeList.size() == 0) {
        std::cout << "No hay recetas registradas.\n";
        return;
    }
    for (std::size_t i = 0; i < recipeList.size(); ++i ) {
        recipeList.at(i).printDetails();//imprimir recetas con ingredientes
    } 
    
}

//Listar por categorias
void Menu::actionListByCategory() const {
    clearScreen();
    Category c = askCategory();
    bool found = false;
    for (std::size_t i = 0; i < recipeList.size(); ++i) {
        if (recipeList.at(i).getCategory() == c) {
            recipeList.at(i).printDetails();
            found = true;
        }
        
    }
    if(!found){
        std::cout << "No hay recetas en esta categoría.\n";
    }
    
}

//Agregar receta
void Menu::actionAddRecipe() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string name, author, procedure;
    int prep;

    std::cout << "Nombre de la receta: ";
    std::getline(std::cin, name);
    std::cout << "Nombre del autor: ";
    std::getline(std::cin, author);
    Category cat = askCategory();
    std::cout << "Tiempo de preparación (min): ";
    std::cin >> prep;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Procedimiento (una línea): ";
    std::getline(std::cin, procedure);

    Recipe r(name, author, cat, prep, procedure);
    std::string resp;
    std::cout << "¿Desea agregar ingredientes ahora? (s/n): ";
    std::getline(std::cin, resp);
    
    if (!resp.empty() && (resp[0] == 's' || resp[0] == 'S')) {
        int n;
        std::cout << "¿Cuántos ingredientes desea agregar?: ";
        std::cin >> n;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int i = 0; i < n; ++i) {
            std::string ingName, unit;
            double qty;
            std::cout << "Ingrediente #" << (i + 1) << " - Nombre: ";
            std::getline(std::cin, ingName);
            std::cout << "Cantidad: ";
            std::cin >> qty;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Unidad: ";
            std::getline(std::cin, unit);
            r.addIngredientOrdered(Ingredient(ingName, qty, unit));
        }
    }
    
    if (recipeList.add(r)) std::cout << "Receta agregada.\n";
    else std::cout << "La lista de recetas está llena.\n";
}

//Eliminar receta
void Menu::actionDeleteRecipe() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string name;
    std::cout << "Nombre de la receta a eliminar: ";
    std::getline(std::cin, name);
    if (recipeList.removeByName(name)) std::cout << "Receta eliminada.\n";
    else std::cout << "No se encontró la receta.\n";
}

//Vaciar recetario
void Menu::actionClearAll() {
    char confirma;

    std::cout << "¿Estás seguro de eliminar TODAS las recetas? (s/n): ";
    if (!(std::cin >> confirma)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada inválida. Operación cancelada.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirma == 's' || confirma == 'S') {
        recipeList.clear();   // O clearAll(), según como lo tengas
        std::cout << "→ Todas las recetas han sido eliminadas.\n";
    } else {
        std::cout << "Operación cancelada.\n";
    }
}


//Ordenar por nombre
void Menu::actionSortByName() {
    clearScreen();
    recipeList.sortByNameRecursive();
    std::cout << "Recetas ordenadas por nombre.\n";
}

//Ordenar por tiempo de preparacion 
void Menu::actionSortByTime() {
    clearScreen();
    recipeList.sortByPrepTimeRecursive();
    std::cout << "Recetas ordenadas por tiempo de preparación.\n";
}

void Menu::actionAddIngredient() {
    // Limpia el '\n' que quedó después de leer la opción del menú
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string recipeName;
    std::cout << "Nombre de la receta a la que deseas agregar un ingrediente: ";
    std::getline(std::cin, recipeName);

    int idx = recipeList.findByName(recipeName);
    if (idx == -1) {
        std::cout << "× No se encontró la receta.\n";
        return;
    }

    Recipe& r = recipeList.at(idx);

    std::string ingName, unit;
    double qty;

    std::cout << "Nombre del ingrediente: ";
    std::getline(std::cin, ingName);

    std::cout << "Cantidad (numérica): ";
    while (!(std::cin >> qty)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Valor inválido. Ingresa un número: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Unidad: ";
    std::getline(std::cin, unit);

    if (r.addIngredientOrdered(Ingredient(ingName, qty, unit))) {
        std::cout << "→ Ingrediente agregado en orden.\n";
    } else {
        std::cout << "× No se pudo agregar el ingrediente.\n";
    }
}


//Eliminar ingrediente
void Menu::actionRemoveIngredient() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string recipeName, ingName;
    std::cout << "Nombre de la receta: ";
    std::getline(std::cin, recipeName);
    int idx = recipeList.findByName(recipeName);
    if (idx == -1) { std::cout << "Receta no encontrada.\n"; return; }

    std::cout << "Ingrediente a eliminar: ";
    std::getline(std::cin, ingName);
    if (recipeList.at(idx).removeIngredientByName(ingName))
        std::cout << "Ingrediente eliminado.\n";
    else
        std::cout << "Ingrediente no encontrado.\n";
}

//Modificar cantidad de ingrediente
void Menu::actionUpdateIngredientQty() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string recipeName, ingName;
    double qty;
    std::cout << "Nombre de la receta: ";
    std::getline(std::cin, recipeName);
    int idx = recipeList.findByName(recipeName);
    if (idx == -1) { std::cout << "Receta no encontrada.\n"; return; }

    std::cout << "Nombre del ingrediente: ";
    std::getline(std::cin, ingName);
    std::cout << "Nueva cantidad: ";
    std::cin >> qty;
    if (recipeList.at(idx).updateIngredientQuantity(ingName, qty))
        std::cout << "Cantidad actualizada.\n";
    else
        std::cout << "Ingrediente no encontrado.\n";
}

//Modificar proceso
void Menu::actionUpdateProcedure() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string recipeName, proc;
    std::cout << "Nombre de la receta: ";
    std::getline(std::cin, recipeName);
    int idx = recipeList.findByName(recipeName);
    if (idx == -1) { std::cout << "Receta no encontrada.\n"; return; }
    std::cout << "Nuevo procedimiento (una línea): ";
    std::getline(std::cin, proc);
    recipeList.at(idx).setProcedure(proc);
    std::cout << "Procedimiento actualizado.\n";
}

//Guardar archivo
void Menu::actionSave() {
    clearScreen();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string path;
    std::cout << "Ruta de archivo para guardar: ";
    std::getline(std::cin, path);
    if (Storage::saveToFile(recipeList, path)) std::cout << "Guardado.\n";
    else std::cout << "Error al guardar el archivo.\n";
}

//Cargar archivo
void Menu::actionLoad() {
    clearScreen();
    // Limpia el '\n' que quedó después de leer la opción del menú
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string path;
    std::cout << "Nombre del archivo a leer: ";
    std::getline(std::cin, path);

    if (path.empty()) {
        std::cout << "× Nombre de archivo vacío.\n";
        return;
    }

    try {
        if (Storage::loadFromFile(recipeList, path)) {
            std::cout << "→ Recetario cargado correctamente desde '" << path << "'.\n";
        } else {
            std::cout << "× No se pudo cargar el recetario (archivo inexistente o formato inválido).\n";
        }
    } catch (const std::exception& e) {
        std::cout << "× Error al leer desde el archivo: " << e.what() << "\n";
    }
}


//Iniciar menu
void Menu::start() {
    int op = -1;
    while (true) {
        clearScreen();     // limpia antes de mostrar el menú
        showOptions();
        std::cin >> op;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida.\n";
            pause();
            continue;
        }

        switch (op) {
            case 1: actionListAll(); break;
            case 2: actionListByCategory(); break;
            case 3: {
                //Buscar receta
                clearScreen();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string name;
                std::cout << "Nombre de la receta a buscar: ";
                std::getline(std::cin, name);
                int idx = recipeList.findByName(name);
                if (idx != -1) std::cout << "Encontrada: " << recipeList.at(idx).toString() << "\n";
                else std::cout << "No se encontró.\n";
                break;
            }
            case 4: actionAddRecipe(); break;
            case 5: actionDeleteRecipe(); break;
            case 6: actionClearAll(); break;
            case 7: actionSortByName(); break;
            case 8: actionSortByTime(); break;
            case 9: actionAddIngredient(); break;
            case 10: actionRemoveIngredient(); break;
            case 11: actionUpdateIngredientQty(); break;
            case 12: actionUpdateProcedure(); break;
            case 13: actionSave(); break;
            case 14: actionLoad(); break;
            case 0: std::cout << "¡Hasta luego!\n"; return;
            default: std::cout << "Opción inválida.\n"; break;
        }
        std::cout << "\n\n";
        pause(); // espera antes de refrescar el menú
    }
}
