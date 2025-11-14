#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <map>
#include "Product.h"

class Order
{
private:
    std::string clientName;
    double latitude;
    double longitude;
    std::map<long long, int> productQuantities; 

public:
    Order(const std::string& name, double lat, double lon);

    std::string getClientName() const;
    double getLatitude() const;
    double getLongitude() const;
    const std::map<long long, int>& getProductQuantities() const;

    void setClientName(const std::string& newName);
    void setCoordinates(double newLat, double newLon);

    void addProduct(const Product& product, int quantity);
    void removeProduct(long long productId);
    int getTotalQuantity() const;
};

#endif // ORDER_H