#include "../include/storage.hpp"
#include <fstream>
#include <sstream>
#include <limits>

bool Storage::saveToFile(const RecipeList& list, const std::string& path) {
    std::ofstream out(path);
    if (!out) {
        std::cout << "× No se pudo abrir el archivo para escritura.\n";
        return false;
    }

    out << list.size() << "\n";

    for (std::size_t i = 0; i < list.size(); ++i) {
        const Recipe& r = list.at(i);
        out << r.getName() << "\n";
        out << r.getAuthorName() << "\n";
        out << categoryToString(r.getCategory()) << "\n";
        out << r.getPrepTimeMinutes() << "\n";
        out << r.getProcedure() << "\n";

        int ingCount = r.getIngredientCount();
        out << ingCount << "\n";
        for (int j = 0; j < ingCount; ++j) {
            const Ingredient& ing = r.getIngredientAt(j);
            out << ing.getName() << "|" << ing.getQuantity()
                << "|" << ing.getUnit() << "\n";
        }
    }

    return true;
}

static bool splitLine(const std::string& line, char sep, std::string& a, std::string& b, std::string& c) {
    size_t p1 = line.find(sep);
    if (p1 == std::string::npos) return false;
    size_t p2 = line.find(sep, p1 + 1);
    if (p2 == std::string::npos) return false;
    a = line.substr(0, p1);
    b = line.substr(p1 + 1, p2 - p1 - 1);
    c = line.substr(p2 + 1);
    return true;
}

bool Storage::loadFromFile(RecipeList& list, const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        std::cout << "× No se pudo abrir el archivo '" << path << "' para lectura.\n";
        return false;
    }

    list.clear();  // o list.clearAll();

    int nRecetas = 0;
    if (!(in >> nRecetas)) {
        std::cout << "× Formato inválido: no se pudo leer el número de recetas.\n";
        return false;
    }
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 0; i < nRecetas; ++i) {
        std::string name, author, catStr, procedure;
        int prep = 0;

        if (!std::getline(in, name)) {
            std::cout << "× Fin de archivo inesperado al leer nombre de receta #" << (i+1) << ".\n";
            return false;
        }
        if (!std::getline(in, author)) {
            std::cout << "× Fin de archivo inesperado al leer autor de receta #" << (i+1) << ".\n";
            return false;
        }
        if (!std::getline(in, catStr)) {
            std::cout << "× Fin de archivo inesperado al leer categoría de receta #" << (i+1) << ".\n";
            return false;
        }

        if (!(in >> prep)) {
            std::cout << "× Formato inválido en tiempo de preparación de receta #" << (i+1) << ".\n";
            return false;
        }
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!std::getline(in, procedure)) {
            std::cout << "× Fin de archivo inesperado al leer procedimiento de receta #" << (i+1) << ".\n";
            return false;
        }

        Category cat = categoryFromString(catStr);
        Recipe r(name, author, cat, prep, procedure);

        int nIng = 0;
        if (!(in >> nIng)) {
            std::cout << "× Formato inválido al leer número de ingredientes de receta #" << (i+1) << ".\n";
            return false;
        }
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int j = 0; j < nIng; ++j) {
            std::string line;
            if (!std::getline(in, line)) {
                std::cout << "× Fin de archivo inesperado al leer ingrediente #" << (j+1)
                          << " de la receta #" << (i+1) << ".\n";
                return false;
            }
            if (line.empty()) {
                --j; // línea vacía, reintentar
                continue;
            }

            auto p1 = line.find('|');
            auto p2 = line.find('|', p1 + 1);

            if (p1 == std::string::npos || p2 == std::string::npos) {
                std::cout << "× Formato inválido en la línea de ingrediente: '" << line << "'.\n";
                return false;
            }

            std::string ingName = line.substr(0, p1);
            std::string qtyStr  = line.substr(p1 + 1, p2 - p1 - 1);
            std::string unit    = line.substr(p2 + 1);

            double qty = 0.0;
            try {
                qty = std::stod(qtyStr);
            } catch (const std::exception&) {
                std::cout << "× No se pudo convertir '" << qtyStr
                          << "' a número en un ingrediente de la receta #" << (i+1) << ".\n";
                return false;
            }

            r.addIngredientOrdered(Ingredient(ingName, qty, unit));
        }

        list.add(r);
    }

    return true;
}
