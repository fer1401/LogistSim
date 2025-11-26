#ifndef STATS_H
#define STATS_H

#include <iostream>

class Stats
{
public:
    Stats() = default;

    // Getters
    int getTotalOrdersReceived() const;
    int getTotalOrdersFulfilled() const;
    int getTotalOrdersShipped() const;
    double getTotalDistanceTraveled() const;
    int getTotalTripsMade() const;

    // Setters
    void setTotalOrdersReceived(int v);
    void setTotalOrdersFulfilled(int v);
    void setTotalOrdersShipped(int v);
    void setTotalDistanceTraveled(double v);
    void setTotalTripsMade(int v);

    // Convenience modifiers
    void incrementTotalOrdersReceived(int delta = 1);
    void incrementTotalOrdersFulfilled(int delta = 1);
    void addOrdersShipped(int delta = 1);
    void addDistanceTraveled(double distance);
    void addTripsMade(int delta = 1);

    // Derived metrics
    double averageFulfillmentRate() const;
    double shippedPackagePercent() const;
    double averageDistancePerTrip() const;

    // Print
    void print(std::ostream &os = std::cout) const;

private:
    int totalOrdersReceived = 0;
    int totalOrdersFulfilled = 0;
    int totalOrdersShipped = 0;

    double totalDistanceTraveled = 0.0;
    int totalTripsMade = 0;
};

#endif // STATS_H
