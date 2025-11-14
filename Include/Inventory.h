#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <string>
#include "Product.h"
#include "Order.h"

class Inventory
{
private:
    std::map<long long, int> stockLevels;

public:
    Inventory();

    int getStockLevel(long long productId) const;
    const std::map<long long, int>& getAllStockLevels() const;

    void addStock(long long productId, int quantity);
    void removeStock(long long productId, int quantity);
    void setStock(long long productId, int quantity);

    bool canFulfillOrder(const Order& order) const;
};

#endif // INVENTORY_H