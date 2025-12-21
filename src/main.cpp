#include "../include/menu.hpp"
#include <iostream>

#ifdef _WIN32
    #include <Windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // solo en Windows
#endif

    RecipeList list;
    Menu menu(list);
    menu.start();
    return 0;
}
