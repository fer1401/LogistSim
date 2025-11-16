#ifndef CITY_H
#define CITY_H

#include <graph.hpp>
#include "CountHandler.h"

class City
{
private:

    Designar::Digraph<MapPoint, Street> graph;

public:

    City();

    const Designar::Digraph<MapPoint, Street> &getGraph() const;
    void setGraph(Designar::Digraph<MapPoint, Street> g);
};

#endif // CITY_H
