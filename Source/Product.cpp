#include "Product.h"
#include <iostream>

Product::Product() 
    : id(0), name("Unknown Product"), description("No description available") {}

Product::Product(long long id, std::string name, std::string description)
    : id(id), name(name), description(description) {}

long long Product::getId() const {
    return id;
}

std::string Product::getName() const {
    return name;
}

void Product::setId(long long newId) {
    id = newId;
}

void Product::setName(const std::string& newName) {
    name = newName;
}

std::string Product::getDescription() const {
    return description;
}

void Product::setDescription(const std::string& newDescription) {
    description = newDescription;
}