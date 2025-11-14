#include "Order.h"
#include <iostream>
#include <numeric> // For std::accumulate

// Constructor
Order::Order(const std::string& name, double lat, double lon)
    : clientName(name), latitude(lat), longitude(lon) {}


std::string Order::getClientName() const {
    return clientName;
}

double Order::getLatitude() const {
    return latitude;
}

double Order::getLongitude() const {
    return longitude;
}

const std::map<long long, int>& Order::getProductQuantities() const {
    return productQuantities;
}

void Order::setClientName(const std::string& newName) {
    clientName = newName;
}

void Order::setCoordinates(double newLat, double newLon) {
    latitude = newLat;
    longitude = newLon;
}

void Order::addProduct(const Product& product, int quantity) {
    if (quantity > 0) {
        productQuantities[product.getId()] += quantity;
    }
}

void Order::removeProduct(long long productId) {
    productQuantities.erase(productId);
}

int Order::getTotalQuantity() const {
    int total = 0;
    for (const auto& pair : productQuantities) {
        total += pair.second;
    }
    return total;
}