#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product
{
private:
    long long id;
    std::string name;
    std::string description;
    float popularityScore;

public:
    Product();
    Product(long long id, std::string name, std::string description, float popularityScore);

    long long getId() const;
    std::string getName() const;
    std::string getDescription() const;
    float getPopularityScore() const;

    void setId(long long newId);
    void setName(const std::string& newName);
    void setDescription(const std::string& newDescription);
    void setPopularityScore(float newPopularityScore);
};

#endif // PRODUCT_H
