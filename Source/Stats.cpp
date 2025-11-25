#include "../Include/Stats.h"
#include <iomanip>

int Stats::getTotalOrdersReceived() const { return totalOrdersReceived; }
int Stats::getTotalOrdersFulfilled() const { return totalOrdersFulfilled; }
int Stats::getTotalOrdersShipped() const { return totalOrdersShipped; }
double Stats::getTotalDistanceTraveled() const { return totalDistanceTraveled; }
int Stats::getTotalTripsMade() const { return totalTripsMade; }

void Stats::setTotalOrdersReceived(int v) { totalOrdersReceived = v; }
void Stats::setTotalOrdersFulfilled(int v) { totalOrdersFulfilled = v; }
void Stats::setTotalOrdersShipped(int v) { totalOrdersShipped = v; }
void Stats::setTotalDistanceTraveled(double v) { totalDistanceTraveled = v; }
void Stats::setTotalTripsMade(int v) { totalTripsMade = v; }

void Stats::incrementTotalOrdersReceived(int delta) { totalOrdersReceived += delta; }
void Stats::incrementTotalOrdersFulfilled(int delta) { totalOrdersFulfilled += delta; }
void Stats::addOrdersShipped(int delta) { totalOrdersShipped += delta; }
void Stats::addDistanceTraveled(double distance) { totalDistanceTraveled += distance; }
void Stats::addTripsMade(int delta) { totalTripsMade += delta; }

double Stats::averageFulfillmentRate() const
{
    if (totalOrdersReceived <= 0) return 0.0;
    return (static_cast<double>(totalOrdersFulfilled) / totalOrdersReceived) * 100.0;
}

double Stats::shippedPackagePercent() const
{
    if (totalOrdersFulfilled <= 0) return 0.0;
    return (static_cast<double>(totalOrdersShipped) / totalOrdersFulfilled) * 100.0;
}

double Stats::averageDistancePerTrip() const
{
    if (totalTripsMade <= 0) return 0.0;
    return totalDistanceTraveled / totalTripsMade;
}

void Stats::print(std::ostream &os) const
{
    os << "Simulation Statistics:\n";
    os << "Total Orders Received: " << totalOrdersReceived << "\n";
    os << "Total Orders Fulfilled: " << totalOrdersFulfilled << "\n";
    os << "Total Orders Shipped: " << totalOrdersShipped << "\n";
    os << "Average Fulfillment Rate: " << averageFulfillmentRate() << "%\n";
    os << "Shipped Package %: " << shippedPackagePercent() << "%\n";
    os << "-----------------------------------\n";
    os << "Total Distance Traveled: " << totalDistanceTraveled << " units\n";
    os << "Total Trips Made: " << totalTripsMade << "\n";
    os << "Average Distance per Trip: " << averageDistancePerTrip() << " units\n";
}
