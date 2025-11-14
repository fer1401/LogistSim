#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product
{
private:
    long long id;
    std::string name;
    std::string description;

public:
    Product();
    Product(long long id, std::string name, std::string description);

    long long getId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setId(long long newId);
    void setName(const std::string& newName);
    void Product::setDescription(const std::string& newDescription)
};

#endif // PRODUCT_H
