#ifndef INGREDIENT_HPP
#define INGREDIENT_HPP

#include <string>
#include <sstream>

//Clase simple de Ingrediente
class Ingredient {
private:
    std::string name;
    double quantity {0.0};
    std::string unit;

public:
    //Constructores
    Ingredient() = default;
    Ingredient(const std::string& name, double quantity, const std::string& unit)
        : name(name), quantity(quantity), unit(unit) {}

    //Getters
    const std::string& getName() const { return name; }
    double getQuantity() const { return quantity; }
    const std::string& getUnit() const { return unit; }

    //Setters
    void setQuantity(double q) { quantity = q; }

    std::string toString() const {
        std::ostringstream oss;
        oss << name << " : " << quantity << " " << unit;
        return oss.str();
    }
};

#endif // INGREDIENT_HPP
