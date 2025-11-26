#ifndef CITY_H
#define CITY_H

#include <graph.hpp>
#include "CountHandler.h"

using CityGraph = Designar::Digraph<MapPoint, Street>;

// Collect active routes and convert to Designar::Path placeholders
// Define distance (edge cost) and heuristic for A*
struct CityEdgeDistance
{
    using Type = double;
    static constexpr Type ZERO = 0.0;
    Type operator()(const CityGraph::Arc *a) const
    {
        return a->get_info().getLength();
    }
};

// Helper: haversine distance (meters)
inline double haversine (double lat1, double lon1, double lat2, double lon2)
{
    static const double R = 6371000.0; // Earth radius in meters
    const double toRad = M_PI / 180.0;
    double dLat = (lat2 - lat1) * toRad;
    double dLon = (lon2 - lon1) * toRad;
    double a = std::sin(dLat / 2) * std::sin(dLat / 2) + std::cos(lat1 * toRad) * std::cos(lat2 * toRad) * std::sin(dLon / 2) * std::sin(dLon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    return R * c;
};

struct CityHeuristic
{
    using Type = double;
    static constexpr Type ZERO = 0.0;
    Type operator()(const CityGraph::Node *current, const CityGraph::Node *target) const
    {
        double lat1 = current->get_info().getLatitude();
        double lon1 = current->get_info().getLongitude();
        double lat2 = target->get_info().getLatitude();
        double lon2 = target->get_info().getLongitude();
        return haversine(lat1, lon1, lat2, lon2);
    }
};

// Helper: find nearest graph node to given coordinates
inline CityGraph::Node* find_nearest_node(CityGraph &city, double lat, double lon)
{
    CityGraph::Node *best = nullptr;
    double bestd = std::numeric_limits<double>::infinity();
    for (const auto &node : city.nodes())
    {
        double d = haversine(lat, lon, node->get_info().getLatitude(), node->get_info().getLongitude());
        if (d < bestd)
        {
            bestd = d;
            best = node;
        }
    }
    return best;
}

// Helper to compute path distance by summing arc lengths
inline double path_distance(const Designar::Path<CityGraph> &p) {
    double sum = 0.0;
    p.for_each([&sum](auto node, auto arc)
    {
        if (arc)
            sum += arc->get_info().getLength();
    });
    return sum;
};

class City
{
private:

    CityGraph graph;
    
public:

    City();

    const CityGraph &getGraph() const;
    CityGraph &getGraph();
    void setGraph(Designar::Digraph<MapPoint, Street> g);
};

#endif // CITY_H
