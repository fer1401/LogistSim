#include "Inventory.h"
#include <iostream>

Inventory::Inventory() {}

int Inventory::getStockLevel(long long productId) const {
    auto it = stockLevels.find(productId);
    if (it != stockLevels.end()) {
        return it->second;
    }
    return 0;
}

const std::map<long long, int>& Inventory::getAllStockLevels() const {
    return stockLevels;
}

void Inventory::addStock(long long productId, int quantity) {
    if (quantity > 0) {
        stockLevels[productId] += quantity;
    }
}

void Inventory::removeStock(long long productId, int quantity) {
    if (quantity > 0) {
        auto it = stockLevels.find(productId);
        if (it != stockLevels.end()) {
            it->second -= quantity;
            if (it->second < 0) {
                it->second = 0; // Prevent negative stock
            }
        }
    }
}

void Inventory::setStock(long long productId, int quantity) {
    if (quantity >= 0) {
        stockLevels[productId] = quantity;
    }
}

bool Inventory::canFulfillOrder(const Order& order) const {
    const auto& orderQuantities = order.getProductQuantities();

    for (const auto& pair : orderQuantities) {
        long long productId = pair.first;
        int requiredQuantity = pair.second;

        auto it = stockLevels.find(productId);
        
        if (it == stockLevels.end() || it->second < requiredQuantity) {
            return false;
        }
    }

    return true;
}