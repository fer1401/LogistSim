#ifndef CITY_H
#define CITY_H

#include <graph.hpp>
#include "CountHandler.h"

using CityGraph = Designar::Digraph<MapPoint, Street>;

class City
{
private:

    CityGraph graph;
    
public:

    City();

    const CityGraph &getGraph() const;
    void setGraph(Designar::Digraph<MapPoint, Street> g);
};

#endif // CITY_H
